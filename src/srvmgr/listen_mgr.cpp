/*
 * listener.cpp
 *
 *  Created on: 2018年1月16日
 *      Author: Administrator
 */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <srvmgr/listen_mgr.h>
#include <sys/socket.h>

#include "common/log.h"
#include "common/connection.h"

std::vector<service_listener*>   g_listeners;

int32_t listen_mgr_init(struct event_base* evbase)
{
    assert(evbase);

    for (std::vector<service_listener*>::const_iterator cit = g_listeners.begin();
         cit != g_listeners.end(); ++cit)
    {
        service_listener* listener = *cit;
        linfo("service:%s init, port:%u.", listener->name, listener->port);

        listener->evbase = evbase;

        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = listener->ip;
        sin.sin_port = htons(listener->port);

        listener->evlistener = evconnlistener_new_bind(evbase, listen_mgr_listener_cb, (void*)listener,
                LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                (struct sockaddr*)&sin, sizeof(sin));

        if (!listener->evlistener) {
            lerror("server:%s listen error.", listener->name);
            return -1;
        }
    }

    return 0;
}

int32_t listen_mgr_finish()
{
    for (std::vector<service_listener*>::const_iterator cit = g_listeners.begin();
         cit != g_listeners.end(); ++cit)
    {
        service_listener* server = *cit;
        evconnlistener_free(server->evlistener);
    }

    return 0;
}

void listen_mgr_add(service_listener* server)
{
    g_listeners.push_back(server);
}

void listen_mgr_listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
                            struct sockaddr *sa, int socklen, void *user_data)
{
    service_listener* server = (service_listener*)user_data;

    struct sockaddr_in* addr = (struct sockaddr_in*)sa;
    ldebug("client connected: %s:%u.", inet_ntoa(addr->sin_addr), addr->sin_port);

    struct bufferevent* bev = bufferevent_socket_new(server->evbase, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        lerror("Error constructing bufferevent!");
        event_base_loopbreak(server->evbase);
        return;
    }

    connection* conn = (connection*)malloc(sizeof(connection));
    conn_init(conn, server->evbase, bev, addr->sin_addr.s_addr, addr->sin_port, &server->handler, CONN_SERVER, CONN_OK);

    bufferevent_setcb(bev, server->handler.readcb, server->handler.writecb, server->handler.eventcb, conn);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    struct timeval tv = {DEFAULT_CONN_TIMEOUT, 0};
    bufferevent_set_timeouts(bev, &tv, &tv);

    if (server->listencb) {
        server->listencb(listener, fd, sa, socklen, conn);
    }
}

