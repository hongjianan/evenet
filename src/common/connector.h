/*
 * connector.h
 *
 *  
 *
 *  Created on: 2018年2月4日
 *      Author: Hong
 */

#ifndef COMMON_CONNECTOR_H_
#define COMMON_CONNECTOR_H_

#include "service_type.h"
#include "connection.h"

typedef struct connector_t {
    char name[MAX_SERVICE_NAME_LEN];
    service_type type;

    uint32_t server_ip;
    uint16_t server_port;
    struct sockaddr_in  sin;

    conn_handler        handler;
} connector;


#endif /* COMMON_CONNECTOR_H_ */
/* end of file */
