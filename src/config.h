/*
 * config.h
 *
 *  Created on: 2018年1月16日
 *      Author: Administrator
 */

#ifndef CONFIG_H_
#define CONFIG_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stdint.h>
#include "common/defined.h"

typedef struct proc_conf_t
{
    char      proc_name[MAX_NAME_LEN];
    uint16_t    server_port;
    uint32_t    server_ip;
} proc_conf;


int config_load(proc_conf* self, const char* filename);

int config_reload(proc_conf* self);

char* config_get_name(proc_conf* self);

extern proc_conf g_proc_conf;


//#ifdef __cplusplus
//}
//#endif

#endif /* CONFIG_H_ */
