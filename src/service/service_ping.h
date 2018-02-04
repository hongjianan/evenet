/*
 * service_ping.h
 *
 *  Created on: 2018年1月25日
 *      Author: Administrator
 */

#ifndef SERVICE_SERVICE_PING_H_
#define SERVICE_SERVICE_PING_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <string.h>
#include <stdlib.h>

#include "common/listener.h"
#include "server_ping.h"

#define SERVICE_PING_PORT   (12345)

typedef struct service_ping_t {
    service_listener    listener;
} service_ping;


void service_ping_init(service_ping* self);

void service_ping_release(service_ping* self);

extern service_ping g_service_ping;


//#ifdef __cplusplus
//}
//#endif

#endif /* SERVICE_SERVICE_PING_H_ */
