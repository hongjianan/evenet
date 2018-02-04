/*
 * clihnd_ping.h
 *
 *  
 *
 *  Created on: 2018年2月4日
 *      Author: Hong
 */

#ifndef CLIENT_CLIHND_PING_H_
#define CLIENT_CLIHND_PING_H_

//#include <vector>
#include "common/connection.h"

typedef struct clihnd_ping_t {
    connector conntor;
} clihnd_ping;

void clihnd_ping_init(clihnd_ping* self);

extern clihnd_ping g_clihnd_ping;

#endif /* CLIENT_CLIHND_PING_H_ */
/* end of file */
