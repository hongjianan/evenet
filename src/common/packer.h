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
#include "header.h"

#define message_pack(pack_func, msg, outbuf, outlen, header_uri, uri) \
do { \
    size_t msglen = pack_func##__get_packed_size(&msg); \
    outlen = msglen + HEADER_SIZE; \
    outbuf = (uint8_t*)malloc(outlen); \
    pack_func##__pack(&msg, outbuf + HEADER_SIZE); \
    header_uri = uri; \
} while (0)


//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_PACKER_H_ */
