/*
 * connection.cpp
 *
 *  Created on: 2018-2-3
 *      Author: Administrator
 */

#include "cevent.h"
#include "packer.h"
#include "connection.h"
#include "uri.h"

void conn_init(connection_t* self, struct event_base* evbase, struct bufferevent* bev,
               uint32_t ip, uint16_t port, conn_handler_t* handler, conn_type_t type, conn_status_t status)
{
    self->evbase = evbase;
    self->bev    = bev;
    self->ping_timer = NULL;

    self->req_sno = 0;
    self->rsp_sno = 0;

    self->rip    = ip;
    self->rport  = port;
    self->status = status;
    self->type   = type;

    self->rxpb    = NULL;
    self->rxpblen = 0;

    self->sendbytes = 0;
    self->recvbytes = 0;
    self->last_recv_time = 0;

    self->handler = *handler;
}

void conn_release(connection_t* self)
{
    if (self->bev) {
        bufferevent_free(self->bev);
        self->bev = NULL;
    }

    // rx protobuf
    if (self->rxpb) {
        free(self->rxpb);
        self->rxpb    = NULL;
        self->rxpblen = 0;
    }

    if (self->ping_timer) {
        cevent_free(self->ping_timer);
        self->ping_timer = NULL;
    }

    self->status = CONN_CLOSE;
}

static int conn_create_probuf(connection_t* self, uint8_t** buf, uint32_t* buflen, uint32_t len)
{
    assert(self);
    assert(!*buf);

    *buf = (uint8_t*)malloc(sizeof(uint8_t) * len);
    if (NULL == *buf) {
        lerror("%s malloc len=%u fail, errno=[%d][%s]", __FUNCTION__, len, errno, strerror(errno));
        return -1;
    }

    *buflen = len;
    return 0;
}

int conn_create_rxbuf(connection_t* self, uint32_t len)
{
    return conn_create_probuf(self, &self->rxpb, &self->rxpblen, len);
}

int conn_event(connection_t* self, short events)
{
    if (events & BEV_EVENT_CONNECTED) {
        linfo("Server:%p connection success.", svr);
        return;
    } else if (events & BEV_EVENT_EOF) {
        lwarn("Server:%p connection closed.", svr);
    } else if (events & BEV_EVENT_TIMEOUT) {
        lwarn("Server:%p connection timeout.", svr);
    } else if (events & BEV_EVENT_ERROR) {
        lerror("Server:%p connection error:[%d][%s].", svr, errno, strerror(errno));
    }
}

void conn_do_ping_req(connection_t* self)
{
    Ping__PingReq req = PING__PING_REQ__INIT;
    req.sno = self->req_sno++;
    ldebug("connection_t:%p do ping request sno:%u.", self, req.sno);

    // send data to server
    uint8_t* outbuf = NULL;
    size_t   outlen = 0;
    uint32_t rsp_uri = 0;
    message_pack(ping__ping_req, req, outbuf, outlen, rsp_uri, SERVICE_PING_REQ);
    message_pack_write(self->bev, outbuf, outlen, rsp_uri);
}

void conn_ping_req_handler(connection_t* self, Ping__PingReq* msg, Ping__PingRsp* rsp)
{
    ldebug("connection_t:%p ping request sno:%u.", self, msg->sno);
    rsp->sno = msg->sno;

    self->req_sno = msg->sno;
    self->rsp_sno = msg->sno;
}

void conn_ping_rsp_handler(connection_t* self, Ping__PingRsp* msg)
{
    ldebug("connection_t:%p ping response sno:%u.", self, msg->sno);
    self->rsp_sno = msg->sno;
}

int32_t conn_ping_request_handler(connection_t* self, uint8_t* inbuf, size_t length, uint32_t uri)
{
    uint8_t* outbuf = NULL;
    size_t   outlen = 0;
    uint32_t rsp_uri = 0;

    switch (uri)
    {
    case SERVICE_PING_REQ:
    {
        Ping__PingReq *req = ping__ping_req__unpack(NULL, length, inbuf);
        Ping__PingRsp rsp = PING__PING_RSP__INIT;
        /* handler */
        conn_ping_req_handler(self, req, &rsp);
        ping__ping_req__free_unpacked(req, NULL);
        /* pack response */
        message_pack(ping__ping_rsp, rsp, outbuf, outlen, rsp_uri, SERVICE_PING_RSP);
    }
        break;
    case SERVICE_PING_RSP:
    {
        Ping__PingRsp *msg = ping__ping_rsp__unpack(NULL, length, inbuf);

        /* handler */
        conn_ping_rsp_handler(self, msg);
        ping__ping_rsp__free_unpacked(msg, NULL);
    }
        break;
    default:
        lerror("%s error message type:%u %u:%u.", __FUNCTION__, uri, uri/(1<<16), uri%(1<<16));
        return -1;
    }

    if (outbuf) {
        message_pack_write(self->bev, outbuf, outlen, rsp_uri);
    }

    return 0;
}


void conn_ping_timer(evutil_socket_t fd, short event, void* arg)
{
    connection_t* conn = (connection_t*)arg;

    /* check ping status */
    if (conn->req_sno != conn->rsp_sno) {
        lerror("connection_t:%p ping status error.", conn);
        conn->status = CONN_BROKEN;
        return;
    }

    conn_do_ping_req(conn);
}

struct event* conn_create_ping_timer(connection_t* self, int64_t msec)
{
    if (0 <= msec) {
        msec = DEFAULT_CONN_PING_TIME;
    }
    self->ping_timer = cevent_timer_add(msec, conn_ping_timer, self);

    return self->ping_timer;
}

int conn_readcb(connection_t* self, conn_request_handler_t request_handler, void* arg)
{
    struct evbuffer *ebuf = bufferevent_get_input(self->bev);

    int pack_len = 0;
    for (int buf_len = evbuffer_get_length(ebuf); buf_len >= HEADER_SIZE; buf_len -= pack_len)
    {
        evbuffer_copyout(ebuf, &pack_len, sizeof(pack_len));
        if (pack_len > buf_len) {
            ldebug("message incomplete pack_len:%d buf_len:%d", pack_len, buf_len);
            break;
        }

        //TODO:: count package length
        uint8_t* content = NULL;
        if (self->rxpblen < pack_len) {
            content = (uint8_t*)malloc(pack_len);
        } else {
            content = self->rxpb;
        }

        evbuffer_remove(ebuf, content, pack_len);
        message_header* header = (message_header*)content;

        if (0 != message_header_check(header)) {    // 协议异常，断开连接
            if (self->rxpblen < pack_len) {
                free(content);
            }
            return -1;
        }

        if (SERVICE_PING_REQ == header->uri) {
            conn_ping_request_handler(self, content + sizeof(*header), pack_len - sizeof(*header), header->uri);
        } else {
            request_handler(arg, content + sizeof(*header), pack_len - sizeof(*header), header->uri);
        }

        if (self->rxpblen < pack_len) {
            free(content);
        }
    }

    return 0;
}

int conn_writecb(connection_t* self, void *arg)
{
    return 0;
}

int conn_eventcb(connection_t* self, short events, void *arg)
{
    ldebug("[%s], events:%u arg:%p", __FUNCTION__, events, arg);

    if (events & BEV_EVENT_CONNECTED) {
        linfo("connection:%p success.", self);
        return 0;
    } else if (events & BEV_EVENT_TIMEOUT) {
        lwarn("connection:%p timeout.", self);
    } else if (events & BEV_EVENT_EOF) {
        lwarn("connection:%p closed.", self);
    } else if (events & BEV_EVENT_ERROR) {
        lerror("connection:%p error:[%d][%s].", self, errno, strerror(errno));
    }

    return -1;
}


