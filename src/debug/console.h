/*
 * console.h
 *
 *  Created on: 2018-2-8
 *      Author: Administrator
 */

#ifndef DEBUG_CONSOLE_H_
#define DEBUG_CONSOLE_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

typedef void (*cmd_fun)(void*);

typedef struct cmd_ops {
    char*   cmd;
    cmd_fun op;
} cmd_ops_t;

void cmd_handler(const char* cmd, void* arg);

//#ifdef __cplusplus
//}
//#endif

#endif /* DEBUG_CONSOLE_H_ */
