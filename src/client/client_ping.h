/*
 * service_ping.h
 *
 *  Created on: 2018年1月25日
 *      Author: Administrator
 */

#ifndef CLIENT_CLIENT_PING_H_
#define CLIENT_CLIENT_PING_H_

//#ifdef __cplusplus
//extern "C" {
//#endif


#include <event2/buffer.h>

#include "common/connection.h"
#include "proto/ping.pb-c.h"


typedef struct cli_ping_t {
    connection* conn;
} cli_ping;

void cli_ping_init(cli_ping* client, connection* conn);

void cli_ping_release(cli_ping* client);

int32_t cli_ping_request_handler(cli_ping* self, uint8_t* buffer, size_t length, uint32_t uri);

void cli_ping_do_ping_req(cli_ping* self);

void cli_ping_ping_rsp_handler(cli_ping* self, Ping__PingRsp* msg);



//#ifdef __cplusplus
//}
//#endif

#endif /* CLIENT_CLIENT_PING_H_ */
