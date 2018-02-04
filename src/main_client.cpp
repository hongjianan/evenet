/*
 * main_client.cpp
 *
 *  Created on: 2018年1月30日
 *      Author: Administrator
 */

#include <climgr/client_mgr.h>
#include <srvmgr/listen_mgr.h>
#include <srvmgr/service_mgr.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/log.h"
#include "common/cevent.h"
#include "common/connection.h"
#include "common/ipwl.h"
#include "common/flow_stat.h"
#include "config.h"
#include "debug/signal.h"
#include "client/client_ping.h"


static void default_timer_cb(evutil_socket_t, short, void *)
{
//    ldebug("timer_cb");
}

int main_client(int argc, char* argv[])
{
    if (2 != argc) {
        printf("usage: server port\n");
        return -1;
    }

    /* init config */
    config_load(&g_proc_conf, "../conf/evenet.conf");

    /* init white list */
    ipwl_load(&g_ipwl_mgr, "../conf/ipwl.conf");

    if (0 != cevent_base_init()) {
        lerror("cevent_base_init error");
        return -1;
    }

    /* init signal */
    signal_init();

    /* service init */
    client_mgr_init();

    cevent_timer_add(1000 * 3600, default_timer_cb, NULL);

    /* base loop */
    cevent_base_loop();

    /* exit */
    cevent_base_cleanup();
    lerror("evenet exit");

    return 0;
}

