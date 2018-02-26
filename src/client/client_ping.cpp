/*
 * echo_server.cpp
 *
 *  Created on: 2018-1-25
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


void cli_ping_init(cli_ping_t* client, connection_t* conn)
{
    client->conn = conn;
    conn_create_rxbuf(conn, DEFAULT_CONN_PROBUF_LEN);
}

void cli_ping_release(cli_ping_t* client)
{
    if (client->conn) {
        conn_release(client->conn);
        free(client->conn);
        client->conn = NULL;
    }
}

int32_t cli_ping_request_handler(cli_ping_t* self, uint8_t* inbuf, size_t length, uint32_t uri)
{
    uint8_t* outbuf = NULL;
    size_t   outlen = 0;
    uint32_t rsp_uri = 0;

    switch (uri)
    {
    default:
        lerror("%s error message type:%u %u:%u.", __FUNCTION__, uri, uri/(1<<16), uri%(1<<16));
        return -1;
    }

    if (outbuf) {
        message_pack_write(self->conn->bev, outbuf, outlen, rsp_uri);
    }

    return 0;
}

