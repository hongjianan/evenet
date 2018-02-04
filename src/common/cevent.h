/*
 * event.h
 *
 *  Created on: 2018年1月16日
 *      Author: Administrator
 */

#ifndef COMMON_EVENT_H_
#define COMMON_EVENT_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stdint.h>
#include <event2/event.h>

typedef event_callback_fn   evsignal_cb;
typedef event_callback_fn   evtimer_cb;


/**
 * @brief 事件初始化
 * @return 成功返回 0
 *       失败返回 <0
 */
int32_t cevent_base_init(void);

/**
 * @brief libevent对象销毁
 * @return void
 */
void cevent_base_cleanup(void);

/**
 * @brief 进入事件循环
 * @return void
 */
void cevent_base_loop(void);

/**
 * @brief 退出事件循环
 * @return void
 */
void cevent_base_loopexit(void);


/**
 * @brief 获取event_base
 * @return void
 */
struct event_base* get_event_base_handle(void);

/**
 * @brief 添加信号处理
 * @param [in] sig 信号类型
 * @param [in] func 信号处理回调函数
 * @param [in] arg 信号处理回调函数参数
 * @return 成功返回 0
 *       失败返回 <0
 */
int32_t cevent_evsignal_add(int32_t sig, evsignal_cb func, void *arg);


/**
 * @brief 添加定时器
 * @param param [in] msec 定时器时间间隔:ms
 * @return 成功返回 event指针
 * @return 失败返回 NULL
 */
struct event* cevent_timer_add(int64_t msec, evtimer_cb func, void *arg);


/**
 * @brief 释放event事件资源
 * @return 无
 */
void cevent_free(struct event * pevent);


extern struct event_base* g_event_base;

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_EVENT_H_ */
