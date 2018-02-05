/*
 * allow_ip.h
 *
 *  Created on: 2018年1月17日
 *      Author: Administrator
 */

#ifndef COMMON_IPWL_H_
#define COMMON_IPWL_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "defined.h"

typedef struct ipwl_info_t
{
    uint32_t tmp;
} ipwl_info;

typedef struct ipwl_mgr_t
{
    uint32_t tmp;
} ipwl_mgr;

extern ipwl_mgr g_ipwl_mgr;

static inline ipwl_mgr* ipwl_mgr_get_instance()
{
    return &g_ipwl_mgr;
}

int32_t ipwl_load(ipwl_mgr* ipwl, const char* filename);

int32_t ipwl_add(ipwl_mgr* ipwl, uint32_t left_ip, uint32_t right_ip);

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_IPWL_H_ */
