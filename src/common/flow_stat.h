
#ifndef COMMON_FLOW_STAT_H
#define COMMON_FLOW_STAT_H

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <event2/util.h>
#include "log.h"

#define KBPS (1024)
#define MBPS (1024 * 1024)

typedef struct flow_stat_t
{
    int64_t rx_bytes_total;
    int64_t rx_packets_total;
    int64_t rx_bytes_period;
    int64_t rx_packets_period;

    int64_t tx_bytes_total;
    int64_t tx_packets_total;
    int64_t tx_bytes_period;
    int64_t tx_packets_period;

    struct timeval begin_time_total;
    struct timeval begin_time_period;
} traffic_stat;

static inline void fstat_init(traffic_stat* self, struct timeval* begin)
{
    self->rx_bytes_total    = 0;
    self->rx_packets_total  = 0;
    self->rx_bytes_period   = 0;
    self->rx_packets_period = 0;

    self->tx_bytes_total    = 0;
    self->tx_packets_total  = 0;
    self->tx_bytes_period   = 0;
    self->tx_packets_period = 0;

    self->begin_time_total  = *begin;
    self->begin_time_period = *begin;
}

static inline void fstat_calc_total(traffic_stat* self)
{
    self->rx_bytes_total    += self->rx_bytes_period;
    self->rx_packets_total  += self->rx_packets_period;
    self->tx_bytes_total    += self->tx_bytes_period;
    self->tx_packets_total  += self->tx_packets_period;
}

static inline void fstat_clear_period(traffic_stat* self)
{
    self->rx_bytes_period = 0;
    self->rx_packets_period = 0;
    self->tx_bytes_period = 0;
    self->tx_packets_period = 0;
}

void tstat_output_period(traffic_stat* self, int bps);

void tstat_output_total(traffic_stat* self);

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_FLOW_STAT_H */
