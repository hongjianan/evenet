/*
 * packer.cpp
 *
 *  Created on: 2018-1-11
 *      Author: Administrator
 */

#include <stdlib.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include "packer.h"

void message_pack_ref_cleanup(const void *data, size_t datalen, void *extra)
{
    free((void*)data);
}


int message_pack_write(struct bufferevent* bev, uint8_t* buffer, int len, uint32_t uri)
{
    message_header_t* header = (message_header_t*)buffer;
    message_header_set(header, len, uri, 0);
    return evbuffer_add_reference(bufferevent_get_output(bev), buffer, len,
                           message_pack_ref_cleanup, NULL);
}

