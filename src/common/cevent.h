/*
 * event.h
 *
 *  Created on: 2018-1-16
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
 * @brief 浜嬩欢鍒濆鍖�
 * @return 鎴愬姛杩斿洖 0
 *       澶辫触杩斿洖 <0
 */
int32_t cevent_base_init(void);

/**
 * @brief libevent瀵硅薄閿�姣�
 * @return void
 */
void cevent_base_cleanup(void);

/**
 * @brief 杩涘叆浜嬩欢寰幆
 * @return void
 */
void cevent_base_loop(void);

/**
 * @brief 閫�鍑轰簨浠跺惊鐜�
 * @return void
 */
void cevent_base_loopexit(void);


/**
 * @brief 鑾峰彇event_base
 * @return void
 */
struct event_base* get_event_base_handle(void);

/**
 * @brief 娣诲姞淇″彿澶勭悊
 * @param [in] sig 淇″彿绫诲瀷
 * @param [in] func 淇″彿澶勭悊鍥炶皟鍑芥暟
 * @param [in] arg 淇″彿澶勭悊鍥炶皟鍑芥暟鍙傛暟
 * @return 鎴愬姛杩斿洖 0
 *       澶辫触杩斿洖 <0
 */
int32_t cevent_evsignal_add(int32_t sig, evsignal_cb func, void *arg);


/**
 * @brief 娣诲姞瀹氭椂鍣�
 * @param param [in] msec 瀹氭椂鍣ㄦ椂闂撮棿闅�:ms
 * @return 鎴愬姛杩斿洖 event鎸囬拡
 * @return 澶辫触杩斿洖 NULL
 */
struct event* cevent_timer_add(int64_t msec, evtimer_cb func, void *arg);


/**
 * @brief 閲婃斁event浜嬩欢璧勬簮
 * @return 鏃�
 */
void cevent_free(struct event * pevent);


extern struct event_base* g_event_base;

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_EVENT_H_ */
