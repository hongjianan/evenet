/*
 * client_mgr.h
 *
 *  Created on: 2018-1-29
 *      Author: Administrator
 */

#ifndef SRC_CLIENT_MGR_H_
#define SRC_CLIENT_MGR_H_

#include <event2/event.h>
#include "client/clihnd_ping.h"


int climgr_init();

cli_ping_t* climgr_create_ping(struct event_base *base);

#endif /* SRC_CLIENT_MGR_H_ */
