/*
 * server_ping.h
 *
 *  Created on: 2018年2月3日
 *      Author: Administrator
 */

#ifndef SERVICE_SERVER_PING_H_
#define SERVICE_SERVER_PING_H_

#include "common/connection.h"
#include "proto/ping.pb-c.h"

typedef struct service_ping_t service_ping;

typedef struct svr_ping_t {
    connection*   conn;
    service_ping* service;

    uint32_t      sno;
} svr_ping;

void svr_ping_init(svr_ping* self, connection* conn);

void svr_ping_release(svr_ping* self);

int32_t svr_ping_request_handler(svr_ping* self, uint8_t* buffer, size_t length, uint32_t uri);

void svr_ping_ping_req_handler(svr_ping* self, Ping__PingReq* mgs, Ping__PingRsp* rsp);

#endif /* SERVICE_SERVER_PING_H_ */
