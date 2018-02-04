/*
 * log.h
 *
 *  
 *
 *  Created on: 2018年1月7日
 *      Author: Hong
 */

#ifndef COMMON_LOG_H_
#define COMMON_LOG_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stdio.h>

#define ldebug(fmt, arg...)    printf("[DBG] " fmt "\n", ##arg)
#define linfo(fmt, arg...)     printf("[INF] " fmt "\n", ##arg)
#define lwarn(fmt, arg...)     printf("[WAR] " fmt "\n", ##arg)
#define lerror(fmt, arg...)    printf("[ERR] " fmt "\n", ##arg)

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_LOG_H_ */
/* end of file */
