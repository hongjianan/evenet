/*
 * connection.h
 *
 *  Created on: 2018年1月16日
 *      Author: Administrator
 */

#ifndef COMMON_CONNECTION_H_
#define COMMON_CONNECTION_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <event2/event.h>
#include <event2/bufferevent.h>

#include "log.h"
#include "proto/ping.pb-c.h"


#define DEFAULT_CONN_PING_TIME      (3 * 1000)
#define DEFAULT_CONN_TIMEOUT        (10) //bufferevent 读写超时时间
#define DEFAULT_CONN_PROBUF_LEN     (8 * 1024)

typedef enum conn_type_t {
    CONN_NONE = 0,
    CONN_CLIENT,
    CONN_SERVER,
} conn_type;

typedef enum conn_status_t {
    CONN_CONNECTING,
    CONN_BROKEN,
    CONN_OK,
    CONN_FAIL,
    CONN_CLOSE
} conn_status;

typedef struct conn_handler_t
{
    bufferevent_data_cb     readcb;
    bufferevent_data_cb     writecb;
    bufferevent_event_cb    eventcb;
} conn_handler;

typedef struct connection_t
{
    struct event_base*  evbase;
    struct bufferevent* bev;
    struct event*       ping_timer;
    uint32_t            req_sno;
    uint32_t            rsp_sno;

    conn_handler        handler;
    conn_status         status;
    conn_type           type;

    uint32_t    rip;
    uint16_t    rport;

    uint8_t*    rxpb;
    uint32_t    rxpblen;

    uint64_t    sendbytes;
    uint64_t    recvbytes;
    uint32_t    last_recv_time;
} connection;

void conn_init(connection* self, struct event_base* evbase, struct bufferevent* bev,
               uint32_t ip, uint16_t port, conn_handler* handler, conn_type type, conn_status status);

void conn_release(connection* self);

int conn_create_rxbuf(connection* self, uint32_t len);

static inline int conn_write(connection* self, uint8_t* buffer, uint32_t len)
{
    assert(self);

    if (CONN_OK != self->status) {
        lerror("connection:%p is not CONN_OK, status:%d", self, self->status);
        return -2;
    }
    return bufferevent_write(self->bev, buffer, len);
}

struct event* conn_create_ping_timer(connection* self, int64_t msec);

int32_t conn_ping_request_handler(connection* self, uint8_t* inbuf, size_t length, uint32_t uri);


//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_CONNECTION_H_ */
