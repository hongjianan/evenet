/*
 * connection_t.h
 *
 *  Created on: 2018-1-16
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

typedef void(*conn_request_handler_t)(void*,uint8_t*,int,uint32_t);


#define DEFAULT_CONN_PING_TIME      (3 * 1000)
#define DEFAULT_CONN_TIMEOUT        (10) //bufferevent ¶ÁÐ´³¬Ê±Ê±¼ä
#define DEFAULT_CONN_PROBUF_LEN     (8 * 1024)


typedef enum conn_type {
    CONN_NONE = 0,
    CONN_CLIENT,
    CONN_SERVER,
} conn_type_t;

typedef enum conn_status {
    CONN_CONNECTING,
    CONN_BROKEN,
    CONN_OK,
    CONN_FAIL,
    CONN_CLOSE
} conn_status_t;

typedef struct conn_handler
{
    bufferevent_data_cb     readcb;
    bufferevent_data_cb     writecb;
    bufferevent_event_cb    eventcb;
} conn_handler_t;

typedef struct conn_ping {
    struct event*       ping_timer;
    uint32_t            req_uri;
    uint32_t            req_sno;
    uint32_t            rsp_sno;
} conn_ping_t;

typedef struct connection_t
{
    struct event_base*  evbase;
    struct bufferevent* bev;

    conn_handler_t        handler;
    conn_status_t         status;
    conn_type_t           type;
    conn_ping_t           ping;

    uint32_t    rip;
    uint16_t    rport;

    uint8_t*    rxpb;
    uint32_t    rxpblen;

    uint64_t    sendbytes;
    uint64_t    recvbytes;
    uint32_t    last_recv_time;
} connection_t;

void conn_init(connection_t* self, struct event_base* evbase, struct bufferevent* bev,
               uint32_t ip, uint16_t port, conn_handler_t* handler, conn_type_t type, conn_status_t status);

void conn_release(connection_t* self);

int conn_create_rxbuf(connection_t* self, uint32_t len);

static inline int conn_write(connection_t* self, uint8_t* buffer, uint32_t len)
{
    assert(self);

    if (CONN_OK != self->status) {
        lerror("connection_t:%p is not CONN_OK, status:%d", self, self->status);
        return -2;
    }
    return bufferevent_write(self->bev, buffer, len);
}

struct event* conn_create_ping_timer(connection_t* self, int64_t msec);

void conn_ping_init(conn_ping_t* self, uint32_t req_uri);

int32_t conn_ping_request_handler(connection_t* self, uint8_t* inbuf, size_t length, uint32_t uri);

int conn_readcb (connection_t* self, conn_request_handler_t request_handler, void* arg);
int conn_writecb(connection_t* self, void *arg);
int conn_eventcb(connection_t* self, short events, void *arg);

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_CONNECTION_H_ */
