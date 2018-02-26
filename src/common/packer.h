/*
 * packer.h
 *
 *  Created on: 2018-1-11
 *      Author: Administrator
 */

#ifndef COMMON_PACKER_H_
#define COMMON_PACKER_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stdlib.h>
#include <event2/event.h>
#include "header.h"

//#define message_pack(pack_size_func, pack_func, msg, outbuf, outlen, header_uri, uri) \
//do { \
//    int32_t msglen = pack_size_func(&msg); \
//    outlen = msglen + HEADER_SIZE; \
//    outbuf = (uint8_t*)malloc(outlen); \
//    pack_func(&msg, outbuf + HEADER_SIZE); \
//    header_uri = uri; \
//} while (0)


//#define proto_unpack(unpack_func)

#define message_pack(pack_func, msg, outbuf, outlen, header_uri, uri) \
do { \
    size_t msglen = pack_func##__get_packed_size(&msg); \
    outlen = msglen + HEADER_SIZE; \
    outbuf = (uint8_t*)malloc(outlen); \
    pack_func##__pack(&msg, outbuf + HEADER_SIZE); \
    header_uri = uri; \
} while (0)

void message_pack_ref_cleanup(const void *data, size_t datalen, void *extra);

int message_pack_write(struct bufferevent* bev, uint8_t* buffer, int len, uint32_t uri);


//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_PACKER_H_ */
