/*
 * echo_server.cpp
 *
 *  Created on: 2018年1月25日
 *      Author: Administrator
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#include <event2/event.h>
#include <event2/buffer.h>

#include "common/log.h"
#include "common/header.h"
#include "common/uri.h"
#include "common/cevent.h"
#include "common/packer.h"
#include "client_ping.h"


void cli_ping_init(cli_ping* client, connection* conn)
{
    client->conn = conn;
    client->sno = 0;

    conn_create_rxbuf(conn, DEFAULT_CONN_PROBUF_LEN);
}

void cli_ping_release(cli_ping* client)
{
    conn_release(client->conn);
    free(client->conn);
    client->conn = NULL;
}

int32_t cli_ping_request_handler(cli_ping* self, uint8_t* inbuf, size_t length, uint32_t uri)
{
    uint8_t* outbuf = NULL;
    size_t   outlen = 0;
    uint32_t rsp_uri = 0;

    switch (uri)
    {
    case SERVICE_PING_RSP:
    {
        Ping__PingRsp *msg = ping__ping_rsp__unpack(NULL, length, inbuf);

        /* handler */
        cli_ping_ping_rsp_handler(msg);
        ping__ping_rsp__free_unpacked(msg, NULL);
    }
        break;
    default:
        lerror("%s error message type:%u %u:%u.", __FUNCTION__, uri, uri/(1<<16), uri%(1<<16));
        return -1;
    }

    if (outbuf) {
        message_pack_write(self->conn->bev, outbuf, outlen, rsp_uri);
    }

    return 0;
}


void cli_ping_ping_rsp_handler(cli_ping* self, Ping__PingRsp* rsp)
{
    ldebug("%s echo_rsp sno:%u.", __FUNCTION__, rsp->sno);
}

void cli_ping_do_ping_req(cli_ping* self)
{
    ldebug("%s.", __FUNCTION__);
    Ping__PingReq req = PING__PING_REQ__INIT;

    req.sno = self->sno++;

    /* handler */
    uint8_t* outbuf;
    size_t outlen;
    uint32_t uri;

    message_pack(ping__ping_req, req, outbuf, outlen, uri, SERVICE_PING_REQ);
    message_pack_write(self->conn->bev, outbuf, outlen, uri);
}

