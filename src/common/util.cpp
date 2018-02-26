/*
 * util.c
 *
 *  Created on: 2018-1-11
 *      Author: Administrator
 */

#include "log.h"
#include "util.h"

struct event* create_timer(struct event_base* base, event_callback_fn cb, void* arg, struct timeval tv)
{
    struct event* timer = event_new(base, -1, EV_PERSIST, cb, arg);
    if (NULL == timer) {
        lerror("create_timer event_new error.\n");
        return NULL;
    }
    if (0 == event_add(timer, &tv)) {
        return timer;
    }

    lerror("create_timer event_add error.\n");
    event_free(timer);

    return NULL;
}


