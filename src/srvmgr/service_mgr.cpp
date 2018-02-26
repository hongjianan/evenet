/*
 * service_mgr.cpp
 *
 *  Created on: 2018-1-29
 *      Author: Administrator
 */

#include "service_mgr.h"


int service_mgr_init()
{
    /* echo service */
    service_ping_init(&g_service_ping);

    return 0;
}

