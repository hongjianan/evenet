/*
 * listener.h
 *
 *  Created on: 2018-1-16
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

typedef struct service_listener service_listener_t;

struct service_listener {
    char name[MAX_SERVICE_NAME_LEN];
    service_type_t type;

    uint32_t ip;
    uint16_t port;

    struct event_base*      evbase;
    struct evconnlistener*  evlistener;
    evconnlistener_cb       listencb;
    conn_handler_t          handler;

};

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_LISTENER_H_ */
