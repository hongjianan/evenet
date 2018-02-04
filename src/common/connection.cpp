/*
 * connection.cpp
 *
 *  Created on: 2018年2月3日
 *      Author: Administrator
 */

#include "connection.h"

void conn_init(connection* self, struct event_base* evbase, struct bufferevent* bev,
               uint32_t ip, uint16_t port, conn_handler* handler, conn_type type, conn_status status)
{
    self->evbase = evbase;
    self->bev    = bev;
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

void conn_release(connection* self)
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

    self->status = CONN_CLOSE;
}

static int conn_create_probuf(connection* self, uint8_t** buf, uint32_t* buflen, uint32_t len)
{
    assert(self);
    assert(*buf);

    *buf = (uint8_t*)malloc(sizeof(uint8_t) * len);
    if (NULL == *buf) {
        lerror("%s malloc len=%u fail, errno=[%d][%s]", __FUNCTION__, len, errno, strerror(errno));
        return -1;
    }

    *buflen = len;
    return 0;
}

int conn_create_rxbuf(connection* self, uint32_t len)
{
    return conn_create_probuf(self, &self->rxpb, &self->rxpblen, len);
}

