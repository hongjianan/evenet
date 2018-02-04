/*
 * server_ping.cpp
 *
 *  Created on: 2018年2月3日
 *      Author: Administrator
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <service/service_ping.h>
#include "common/log.h"
#include "common/header.h"
#include "common/packer.h"
#include "common/uri.h"
#include "proto/ping.pb-c.h"

void svr_ping_init(svr_ping* self, connection* conn)
{
    self->conn = conn;
    self->sno = 0;
}

void svr_ping_release(svr_ping* self)
{
    conn_release(self->conn);
    free(self->conn);
    self->conn = NULL;
}

int32_t svr_ping_request_handler(svr_ping* server, uint8_t* inbuf, size_t length, uint32_t uri)
{
    uint8_t* outbuf = NULL;
    size_t   outlen = 0;
    uint32_t rsp_uri = 0;

    switch (uri)
    {
    case SERVICE_PING_REQ:
    {
        Ping__PingReq *req = ping__ping_req__unpack(NULL, length, inbuf);
        Ping__PingRsp rsp = PING__PING_RSP__INIT;
        /* handler */
        svr_ping_ping_req_handler(server, req, &rsp);
        ping__ping_req__free_unpacked(req, NULL);
        /* pack response */
        message_pack(ping__ping_rsp, rsp, outbuf, outlen, rsp_uri, SERVICE_PING_RSP);
    }
        break;
    default:
        lerror("svr_ping_do_request error message type:%d.", uri);
        return -1;
    }

    if (outbuf) {
        message_pack_write(server->conn->bev, outbuf, outlen, rsp_uri);
    }

    return 0;
}


void svr_ping_ping_req_handler(svr_ping* server, Ping__PingReq* req, Ping__PingRsp* rsp)
{
    ldebug("%s sno:%u.", __FUNCTION__, req->sno);

    if (server->sno != req->sno) {
        lerror("echo sno error, server:%u client:%u", server->sno, req->sno);
        rsp->sno = server->sno;
        return;
    }

    server->sno = req->sno;
    rsp->sno = req->sno;
}

