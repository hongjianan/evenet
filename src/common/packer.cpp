/*
 * packer.cpp
 *
 *  Created on: 2018年1月11日
 *      Author: Administrator
 */

#include <stdlib.h>
#include <event2/bufferevent.h>
#include "packer.h"

void message_pack_ref_cleanup(const void *data, size_t datalen, void *extra)
{
    free((void*)data);
}


int message_pack_write(struct bufferevent* bev, uint8_t* buffer, int len, uint32_t uri)
{
    message_header* header = (message_header*)buffer;
    message_header_set(header, len, uri, 0);
    return evbuffer_add_reference(bufferevent_get_output(bev), buffer, len,
                           message_pack_ref_cleanup, NULL);
}

static inline int send_message(uint8_t* msg, uint32_t length, uint32_t uri)
{
    return 0;
}
