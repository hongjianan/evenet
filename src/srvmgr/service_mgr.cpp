/*
 * service_mgr.cpp
 *
 *  Created on: 2018年1月29日
 *      Author: Administrator
 */

#include <srvmgr/service_mgr.h>
#include "service/service_ping.h"

int service_mgr_init()
{
    /* echo service */
    service_ping_init(&g_service_ping);

    return 0;
}

