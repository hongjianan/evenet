/*
 * main_server.cpp
 *
 *  Created on: 2018-1-30
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>

#include "common/log.h"
#include "common/cevent.h"
#include "common/connection.h"
#include "common/ipwl.h"
#include "common/flow_stat.h"
#include "config.h"
#include "debug/signal.h"

#include "srvmgr/listen_mgr.h"
#include "srvmgr/service_mgr.h"

static void default_timer_cb(evutil_socket_t, short, void *)
{
//    linfo("timer_cb\n");
}

int main_server(int argc, char* argv[])
{
//    if (2 != argc) {
//        printf("usage: server_port\n");
//        return -1;
//    }
//
//    int server_port = atoi(argv[1]);

    /* init config */
    config_load(&g_proc_conf, "../conf/evenet.conf");
//    g_proc_conf.server_port = server_port;

    /* init white list */
    ipwl_load(&g_ipwl_mgr, "../conf/ipwl.conf");

    if (0 != cevent_base_init()) {
        lerror("cevent_base_init error");
        return -1;
    }

    /* init signal */
    signal_init();

    /* service init */
    service_mgr_init();

    /* init listener */
    listen_mgr_add(&g_service_ping.listener);

    listen_mgr_init(g_event_base);

    cevent_timer_add(1000 * 3600, default_timer_cb, NULL);

    /* base loop */
    cevent_base_loop();

    /* exit */
    listen_mgr_finish();
    cevent_base_cleanup();
    lerror("evenet exit");

    return 0;
}

