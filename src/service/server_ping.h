/*
 * server_ping.h
 *
 *  Created on: 2018-2-3
 *      Author: Administrator
 */

#ifndef SERVICE_SERVER_PING_H_
#define SERVICE_SERVER_PING_H_

#include "common/connection.h"
#include "proto/ping.pb-c.h"

typedef struct service_ping service_ping_t;
typedef struct svr_ping svr_ping_t;

struct svr_ping {
    connection_t*   conn;
    service_ping_t* service;

    uint32_t      sno;
};

void svr_ping_init(svr_ping_t* self, connection_t* conn);

void svr_ping_release(svr_ping_t* self);

int32_t svr_ping_request_handler(svr_ping_t* self, uint8_t* buffer, size_t length, uint32_t uri);

#endif /* SERVICE_SERVER_PING_H_ */
