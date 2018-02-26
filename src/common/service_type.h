/*
 * service_type.h
 *
 *  Created on: 2018-1-26
 *      Author: Administrator
 */

#ifndef COMMON_SERVICE_TYPE_H_
#define COMMON_SERVICE_TYPE_H_

//#ifdef __cplusplus
//extern "C" {
//#endif

#define MAX_SERVICE_NAME_LEN    (128)

#define SERVICE_PING_NAME   ("service_ping")
#define SERVICE_FILECMP_NAME    ("service_filecmp")


typedef enum service_type {
    SERVICE_COMMON      = 0,
    SERVICE_PING        = 1,
    SERVICE_FILECMP     = 2,
    SERVICE_UNKNOWN
} service_type_t;


//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_SERVICE_TYPE_H_ */
