/*
 * connector.h
 *
 *  
 *
 *  Created on: 2018-2-4
 *      Author: Hong
 */

#ifndef COMMON_CONNECTOR_H_
#define COMMON_CONNECTOR_H_

#include "service_type.h"
#include "connection.h"

typedef struct connector {
    char name[MAX_SERVICE_NAME_LEN];
    service_type_t type;

    uint32_t server_ip;
    uint16_t server_port;
    struct sockaddr_in  sin;

    conn_handler_t        handler;
} connector_t;


#endif /* COMMON_CONNECTOR_H_ */
/* end of file */
