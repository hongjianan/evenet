/*
 * console.cpp
 *
 *  Created on: 2018-2-8
 *      Author: Administrator
 */

#include <string.h>
#include "common/log.h"
#include "console.h"

static void cmd_set_name(void* arg);
static void cmd_get_name(void* arg);

cmd_ops_t g_cmd_ops[] = {
    {"set_name", cmd_set_name},
    {"get_name", cmd_get_name},
};

void cmd_handler(const char* cmd, void* arg)
{
    for (int idx = 0; idx < sizeof(g_cmd_ops)/sizeof(g_cmd_ops[0]); ++idx) {
        if (0 == strcmp(cmd, g_cmd_ops[idx].cmd)) {
            return g_cmd_ops[idx].op(arg);
        }
    }
    lwarn("unknown command:%s", cmd);
}

void cmd_set_name(void* arg)
{
    linfo("run set name");
}

void cmd_get_name(void* arg)
{
    linfo("run get name");
}

