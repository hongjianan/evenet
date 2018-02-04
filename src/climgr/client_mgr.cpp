/*
 * client_mgr.cpp
 *
 *  Created on: 2018年1月29日
 *      Author: Administrator
 */

#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "common/log.h"
#include "client/clihnd_ping.h"
#include "client_mgr.h"

int client_mgr_init()
{
    clihnd_ping_init(&g_clihnd_ping);

    return 0;
}

cli_ping* client_mgr_create_cli_ping(struct event_base *base)
{
    cli_ping* client = (cli_ping*)malloc(sizeof(cli_ping));
    connection* conn = (connection*)malloc(sizeof(connection));
    cli_ping_init(client, conn);

    connector* ctor = &g_clihnd_ping.conntor;

    conn_handler* handler = &ctor->handler;
    struct bufferevent* bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, handler->readcb, handler->writecb, handler->eventcb, client);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    conn_init(conn, base, bev, ctor->server_ip, ctor->server_port, handler, SERVICE_PING, CONN_CONNECTING);

    struct sockaddr_in* psin = &g_clihnd_ping.conntor.sin;

    if (0 < bufferevent_socket_connect(bev, (struct sockaddr *)psin, sizeof(*psin))) {
        bufferevent_free(bev);
        return NULL;
    }

    return client;
}
