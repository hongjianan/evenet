/*
 * listener.h
 *
 *  Created on: 2018年1月16日
 *      Author: Administrator
 */

#ifndef COMMON_LISTENER_H_
#define COMMON_LISTENER_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <event2/listener.h>
#include "connection.h"
#include "service_type.h"


typedef struct service_listener_t {
    char name[MAX_SERVICE_NAME_LEN];
    service_type type;

    uint32_t ip;
    uint16_t port;

    struct event_base*     evbase;
    struct evconnlistener* evlistener;
    evconnlistener_cb   listencb;
    conn_handler        handler;

} service_listener;

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_LISTENER_H_ */
