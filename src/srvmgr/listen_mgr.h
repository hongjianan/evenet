/*
 * listener.h
 *
 *  Created on: 2018年1月16日
 *      Author: Administrator
 */

#ifndef SRC_LISTEN_MGR_H_
#define SRC_LISTEN_MGR_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <vector>
#include "common/listener.h"

typedef std::vector<service_listener*>   listeners;

int32_t listen_mgr_init(struct event_base* evbase);

int32_t listen_mgr_finish();

void listen_mgr_add(service_listener* server);

void listen_mgr_listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
                            struct sockaddr *sa, int socklen, void *user_data);

extern std::vector<service_listener*>   g_listeners;

//#ifdef __cplusplus
//}
//#endif

#endif /* SRC_LISTEN_MGR_H_ */
