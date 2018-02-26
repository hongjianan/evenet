/*
 * client_mgr.cpp
 *
 *  Created on: 2018-1-29
 *      Author: Administrator
 */

#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "common/log.h"
#include "client_mgr.h"
#include "client/client_ping.h"

int climgr_init()
{
    clihnd_ping_init(&g_clihnd_ping);

    return 0;
}

cli_ping_t* climgr_create_ping(struct event_base *base)
{
    cli_ping_t* client = (cli_ping_t*)malloc(sizeof(cli_ping_t));
    connection_t* conn = (connection_t*)malloc(sizeof(connection_t));
    cli_ping_init(client, conn);

    connector_t* ctor = &g_clihnd_ping.conntor;

    conn_handler_t* handler = &ctor->handler;
    struct bufferevent* bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, handler->readcb, handler->writecb, handler->eventcb, client);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    conn_init(conn, base, bev, ctor->server_ip, ctor->server_port, handler, CONN_CLIENT, CONN_CONNECTING);

    struct sockaddr_in* psin = &g_clihnd_ping.conntor.sin;

    if (0 < bufferevent_socket_connect(bev, (struct sockaddr *)psin, sizeof(*psin))) {
        bufferevent_free(bev);
        return NULL;
    }

    return client;
}
