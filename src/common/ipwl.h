/*
 * allow_ip.h
 *
 *  Created on: 2018-1-17
 *      Author: Administrator
 */

#ifndef COMMON_IPWL_H_
#define COMMON_IPWL_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "defined.h"

typedef struct ipwl_info
{
    uint32_t tmp;
} ipwl_info_t;

typedef struct ipwl_mgr
{
    uint32_t tmp;
} ipwl_mgr_t;

extern ipwl_mgr_t g_ipwl_mgr;

static inline ipwl_mgr_t* ipwl_mgr_get_instance()
{
    return &g_ipwl_mgr;
}

int32_t ipwl_load(ipwl_mgr_t* ipwl, const char* filename);

int32_t ipwl_add(ipwl_mgr_t* ipwl, uint32_t left_ip, uint32_t right_ip);

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_IPWL_H_ */
