/*
 * echo_server.cpp
 *
 *  Created on: 2018-1-25
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

service_ping_t g_service_ping;

static void service_ping_conn_listencb(struct evconnlistener* listener, evutil_socket_t fd,
                                       struct sockaddr* addr, int socklen, void* arg);
static void service_ping_conn_readcb (struct bufferevent *bev, void *arg);
static void service_ping_conn_writecb(struct bufferevent *bev, void *arg);
static void service_ping_conn_eventcb(struct bufferevent *bev, short events, void *arg);

void service_ping_init(service_ping_t* self)
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
    connection_t*  conn = (connection_t*)arg;
    svr_ping_t*    svr  = (svr_ping_t*)malloc(sizeof(svr_ping_t));
    svr_ping_init(svr, conn);

    // 创建接受缓存减少内存申请
    if (0 != conn_create_rxbuf(conn, DEFAULT_CONN_PROBUF_LEN)) {
        lwarn("conn_create_rxbuf error");
    }

    bufferevent_setcb(conn->bev, service_ping_conn_readcb, service_ping_conn_writecb, service_ping_conn_eventcb, svr);
}


static void service_ping_conn_readcb(struct bufferevent *bev, void *arg)
{
    svr_ping_t* svr = (svr_ping_t*)arg;

    if (0 != conn_readcb(svr->conn, (conn_request_handler_t)svr_ping_request_handler, arg)) {
        svr_ping_release(svr);
        free(svr);
    }
}

static void service_ping_conn_writecb(struct bufferevent *bev, void *arg)
{
    svr_ping_t* svr = (svr_ping_t*)arg;

    if (0 != conn_writecb(svr->conn, arg)) {
        svr_ping_release(svr);
        free(svr);
    }
}

static void service_ping_conn_eventcb(struct bufferevent *bev, short events, void *arg)
{
    svr_ping_t* svr = (svr_ping_t*)arg;

    if (0 != conn_eventcb(svr->conn, events, arg)) {
        svr_ping_release(svr);
        free(svr);
    }
}

