/*
 * client_mgr.h
 *
 *  Created on: 2018年1月29日
 *      Author: Administrator
 */

#ifndef SRC_CLIENT_MGR_H_
#define SRC_CLIENT_MGR_H_

#include <event2/event.h>
#include "client/client_ping.h"

int client_mgr_init();

cli_ping* client_mgr_create_cli_ping(struct event_base *base);

#endif /* SRC_CLIENT_MGR_H_ */
