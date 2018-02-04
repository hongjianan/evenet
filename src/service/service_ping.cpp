/*
 * echo_server.cpp
 *
 *  Created on: 2018年1月25日
 *      Author: Administrator
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include "common/log.h"
#include "common/header.h"
#include "common/uri.h"
#include "service_ping.h"

service_ping g_service_ping;

static void service_ping_conn_listencb(struct evconnlistener* listener, evutil_socket_t fd,
                                       struct sockaddr* addr, int socklen, void* arg);
static void service_ping_conn_readcb (struct bufferevent *bev, void *arg);
static void service_ping_conn_writecb(struct bufferevent *bev, void *arg);
static void service_ping_conn_eventcb(struct bufferevent *bev, short events, void *arg);

void service_ping_init(service_ping* self)
{
    strncpy(self->listener.name, SERVICE_PING_NAME, MAX_SERVICE_NAME_LEN);
    self->listener.type = SERVICE_PING;

    self->listener.ip   = 0;
    self->listener.port = SERVICE_PING_PORT;

    self->listener.listencb        = service_ping_conn_listencb;
    self->listener.handler.readcb  = service_ping_conn_readcb;
    self->listener.handler.writecb = service_ping_conn_writecb;
    self->listener.handler.eventcb = service_ping_conn_eventcb;
}

static void service_ping_conn_listencb(struct evconnlistener* listener, evutil_socket_t fd,
                                       struct sockaddr* addr, int socklen, void* arg)
{
    connection*  conn = (connection*)arg;
    svr_ping*    svr  = (svr_ping*)malloc(sizeof(svr_ping));
    svr_ping_init(svr, conn);

    // 创建协议解析缓存
    if (0 != conn_create_rxbuf(conn, DEFAULT_CONN_PROBUF_LEN)) {
        lwarn("conn_create_rxbuf error");
    }

    bufferevent_setcb(conn->bev, service_ping_conn_readcb, service_ping_conn_writecb, service_ping_conn_eventcb, svr);
}


static void service_ping_conn_readcb(struct bufferevent *bev, void *arg)
{
    svr_ping* svr = (svr_ping*)arg;
    struct evbuffer *ebuf = bufferevent_get_input(bev);

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
        if (svr->conn->rxpblen < pack_len) {
            content = (uint8_t*)malloc(pack_len);
        } else {
            content = svr->conn->rxpb;
        }

        evbuffer_remove(ebuf, content, pack_len);
        message_header* header = (message_header*)content;

        if (0 != message_header_check(header)) {    // 协议异常，断开连接
            svr_ping_release(svr);
            free(svr);
            if (svr->conn->rxpblen < pack_len) {
                free(content);
            }
            return;
        }

        svr_ping_request_handler(svr, content + sizeof(*header), pack_len - sizeof(*header), header->uri);

        if (svr->conn->rxpblen < pack_len) {
            free(content);
        }
    }
}

static void service_ping_conn_writecb(struct bufferevent *bev, void *arg)
{
}

static void service_ping_conn_eventcb(struct bufferevent *bev, short events, void *arg)
{
    svr_ping* svr = (svr_ping*)arg;
    ldebug("[%s], events:%u args:%p", __FUNCTION__, events, arg);

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

    svr_ping_release(svr);
    free(svr);
}

