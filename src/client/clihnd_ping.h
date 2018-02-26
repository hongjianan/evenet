/*
 * clihnd_ping_t.h
 *
 *  
 *
 *  Created on: 2018-2-4
 *      Author: Hong
 */

#ifndef CLIENT_CLIHND_PING_H_
#define CLIENT_CLIHND_PING_H_

#include "common/connector.h"
#include "client_ping.h"

typedef struct clihnd_ping clihnd_ping_t;

typedef struct clihnd_ping {
    connector_t conntor;
} clihnd_ping_t;

void clihnd_ping_init(clihnd_ping_t* self);

extern clihnd_ping_t g_clihnd_ping;

#endif /* CLIENT_CLIHND_PING_H_ */
/* end of file */
