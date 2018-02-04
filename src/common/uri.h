/*
 * uri.h
 *
 *  
 *
 *  Created on: 2018年1月28日
 *      Author: Hong
 */

#ifndef COMMON_URI_H_
#define COMMON_URI_H_

#include "service_type.h"

enum uri_ping {
    SERVICE_PING_REQ = (SERVICE_PING << 16) + 1,
    SERVICE_PING_RSP = (SERVICE_PING << 16) + 2,
};

enum uri_filecmp {
    SERVICE_FILECMP_CONSULT_REQ     = (SERVICE_FILECMP << 16) + 1,
    SERVICE_FILECMP_CONSULT_RSP     = (SERVICE_FILECMP << 16) + 2,

    SERVICE_BLOCKS_MD5_REQ          = (SERVICE_FILECMP << 16) + 5,
    SERVICE_BLOCKS_MD5_RSP          = (SERVICE_FILECMP << 16) + 6,

    SERVICE_BLOCK_CONTENT_REQ       = (SERVICE_FILECMP << 16) + 9,
    SERVICE_BLOCK_CONTENT_RSP       = (SERVICE_FILECMP << 16) + 10,

    SERVICE_WORKER_COMPLETE_REQ     = (SERVICE_FILECMP << 16) + 13,
    SERVICE_WORKER_COMPLETE_RSP     = (SERVICE_FILECMP << 16) + 14,

    SERVICE_CLIENT_COMPLETE_REQ     = (SERVICE_FILECMP << 16) + 17,
    SERVICE_CLIENT_COMPLETE_RSP     = (SERVICE_FILECMP << 16) + 18,
};

#endif /* COMMON_URI_H_ */
/* end of file */
