/*
 * server_ping.cpp
 *
 *  Created on: 2018-2-3
 *      Author: Administrator
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <event2/event.h>
#include <event2/buffer.h>

#include "common/log.h"
#include "common/header.h"
#include "common/packer.h"
#include "common/uri.h"
#include "proto/ping.pb-c.h"
#include "service/service_ping.h"

void svr_ping_init(svr_ping_t* self, connection_t* conn)
{
    assert(self);
    assert(conn);

    self->conn = conn;
}

void svr_ping_release(svr_ping_t* self)
{
    conn_release(self->conn);
    free(self->conn);
    self->conn = NULL;
}

int32_t svr_ping_request_handler(svr_ping_t* server, uint8_t* inbuf, size_t length, uint32_t uri)
{
    uint8_t* outbuf = NULL;
    size_t   outlen = 0;
    uint32_t rsp_uri = 0;

    switch (uri)
    {
    default:
        lerror("svr_ping_do_request error message type:%d.", uri);
        return -1;
    }

    if (outbuf) {
        conn_pack_write(server->conn, outbuf, outlen, rsp_uri);
    }

    return 0;
}

