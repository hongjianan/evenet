/*
 * handler_ping.cpp
 *
 *  
 *
 *  Created on: 2018-2-4
 *      Author: Hong
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#include <event2/event.h>
#include <event2/buffer.h>

#include "common/log.h"
#include "common/header.h"
#include "common/uri.h"
#include "common/cevent.h"
#include "common/packer.h"
#include "clihnd_ping.h"

clihnd_ping_t g_clihnd_ping;

static void clihnd_ping_conn_readcb (struct bufferevent *bev, void *arg);
static void clihnd_ping_conn_writecb(struct bufferevent *bev, void *arg);
static void clihnd_ping_conn_eventcb(struct bufferevent *bev, short events, void *arg);

void clihnd_ping_init(clihnd_ping_t* self)
{
    connector_t* contor = &self->conntor;
    strncpy(contor->name, SERVICE_PING_NAME, MAX_SERVICE_NAME_LEN);
    contor->type = SERVICE_PING;

    contor->server_ip = inet_addr("127.0.0.1");
    contor->server_port = 12345;

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = contor->server_ip;
    sin.sin_port = htons(contor->server_port);

    contor->sin = sin;

    contor->handler.readcb  = clihnd_ping_conn_readcb;
    contor->handler.writecb = clihnd_ping_conn_writecb;
    contor->handler.eventcb = clihnd_ping_conn_eventcb;
}


static void clihnd_ping_conn_readcb(struct bufferevent *bev, void *arg)
{
    cli_ping_t* client = (cli_ping_t*)arg;

    if (0 != conn_readcb(client->conn, (conn_request_handler_t)cli_ping_request_handler, arg)) {
        cli_ping_release(client);
        free(client);
    }
}

static void clihnd_ping_conn_writecb(struct bufferevent *bev, void *arg)
{
    cli_ping_t* client = (cli_ping_t*)arg;

    if (0 != conn_writecb(client->conn, arg)) {
        cli_ping_release(client);
        free(client);
    }
}

static void clihnd_ping_conn_eventcb(struct bufferevent *bev, short events, void *arg)
{
    cli_ping_t* client = (cli_ping_t*)arg;
    ldebug("echosrv_conn_eventcb, events:%u args:%p", events, arg);

    if (0 != conn_eventcb(client->conn, events, arg, 0)) {
        cli_ping_release(client);
        free(client);
        cevent_base_loopexit();
    }
}

