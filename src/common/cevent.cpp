/*
 * event.cpp
 *
 *  Created on: 2018年1月16日
 *      Author: Administrator
 */

/**
 * @brief: cdp_worker服务事件处理模块实现
 * @note:
 * @author: lifan
 */

#include <signal.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "log.h"
#include "cevent.h"

//libevent 事件上下文
struct event_base* g_event_base = NULL;


/**
 * @brief 事件初始化
 * @return 成功返回 0
 * @return 失败返回 <0
 */
int32_t cevent_base_init(void)
{
    /*新建基础事件句柄*/
    g_event_base = event_base_new();
    if (!g_event_base) {
        lerror("Call event_base_new() failed!");
        return -1;
    }
    linfo("%s() successful.", __FUNCTION__);
    return 0;
}


/**
 * @brief 事件释放
 * @return void
 */
void cevent_base_cleanup(void)
{
    if (g_event_base != NULL) {
        event_base_free(g_event_base);
        g_event_base = NULL;
    }
}


/**
 * @brief 获取event_base对象的句柄
 * @return event_base对象的句柄
 */
struct event_base* cevent_get_event_base(void)
{
    return g_event_base;
}


/**
 * @brief 进入事件循环
 * @return void
 */
void cevent_base_loop(void)
{
    assert(g_event_base);
    linfo("cevent_base_loop begin.");
    if (event_base_dispatch(g_event_base) < 0) {
        lerror("cevent_base_loop failed.");
    }
}

/**
 * @brief 退出事件循环
 * @return void
 */
void cevent_base_loopexit(void)
{
    if (g_event_base) {
        event_base_loopbreak(g_event_base);
        linfo("cevent_base_loop exit.");
    }
}

/**
 * @brief 添加信号处理，信号处理被添加后运行过程中不会被删除，因此不用处理evsignal对象的回收问题
 * @param [in] sig: 信号类型
 * @param [in] func: 信号处理回调函数
 * @param [in] arg: 信号处理回调函数参数
 * @return 0: 成功
 * @return <0: 失败
 */
int32_t cevent_evsignal_add(int32_t sig, evsignal_cb func, void *arg)
{
    assert(g_event_base);
    assert(sig > 0);

    struct event* evsignal = NULL;

    /*新建信号事件*/
    evsignal = evsignal_new(g_event_base, sig, func, arg);
    if (!evsignal) {
        lerror("evsignal_new failed, error:%s", strerror(errno));
        goto _ERR;
    }

    /*添加信号处理*/
    if (evsignal_add(evsignal, NULL) < 0) {
        lerror("evsignal_add failed, error:%s", strerror(errno));
        goto _ERR;
    }
    return 0;

_ERR:
    if(evsignal) {
        event_free(evsignal);
        evsignal = NULL;
    }
    return -1;
}


/**
 * @brief 添加定时器
 * @param param [in] msec 定时器时间间隔:ms
 * @return 成功返回 event指针
 * @return 失败返回 NULL
 */
struct event* cevent_timer_add(int64_t msec, evtimer_cb func, void *arg)
{
    assert(g_event_base);

    struct timeval tv;
    struct event * ptimer = NULL;

    tv.tv_sec = msec / 1000;
    tv.tv_usec = (msec % 1000) * 1000;

    /*新建定时器事件*/
    ptimer = event_new(g_event_base, -1, EV_PERSIST, func, arg);
    if (!ptimer) {
        lerror("event_new failed, error:%s", strerror(errno));
        goto _ERR;
    }

    /*添加定时器事件*/
    if (evtimer_add(ptimer, &tv) < 0) {
        lerror("evtimer_add failed, error:%s", strerror(errno));
        goto _ERR;
    }
    return ptimer;

_ERR:
    if (ptimer) {
        event_free(ptimer);
        ptimer = NULL;
    }
    return NULL;
}


/**
 * @brief 释放event事件资源
 * @return void
 */
void cevent_free(struct event* pevent)
{
    if (pevent != NULL) {
        event_free(pevent);
    }
}

