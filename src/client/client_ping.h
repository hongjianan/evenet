/*
 * service_ping.h
 *
 *  Created on: 2018-1-25
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

typedef struct cli_ping cli_ping_t;

struct cli_ping {
    connection_t* conn;
};

void cli_ping_init(cli_ping_t* client, connection_t* conn);

void cli_ping_release(cli_ping_t* client);

int32_t cli_ping_request_handler(cli_ping_t* self, uint8_t* buffer, size_t length, uint32_t uri);


//#ifdef __cplusplus
//}
//#endif

#endif /* CLIENT_CLIENT_PING_H_ */
