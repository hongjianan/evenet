/*
 * header.h
 *
 *  
 *
 *  Created on: 2018年1月8日
 *      Author: Hong
 */

#ifndef COMMON_HEADER_H_
#define COMMON_HEADER_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stdint.h>

typedef struct message_header
{
#define HEADER_MAGIC    (0x159C)
#define HEADER_SIZE     (sizeof(message_header_t))

    uint32_t length;
    uint32_t uri;
    uint16_t magic;
    int16_t  ret_code;
} message_header_t;


static inline int message_header_check(message_header_t* self)
{
    if (HEADER_MAGIC != self->magic) {
        return -1;
    }
    return 0;
}

static inline void message_header_set(message_header_t* self, uint32_t len,
                                      uint32_t uri, int16_t ret_code)
{
    self->length      = len;
    self->magic       = HEADER_MAGIC;
    self->ret_code    = ret_code;
    self->uri         = uri;
}

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_HEADER_H_ */
/* end of file */
