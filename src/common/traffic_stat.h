
#ifndef COMMON_TRAFFIC_STAT_H
#define COMMON_TRAFFIC_STAT_H

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <stdio.h>
#include <event2/util.h>
#include "log.h"

#define KBPS (1024)
#define MBPS (1024 * 1024)

struct traffic_stat
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
};

inline void tstat_init(struct traffic_stat* self, struct timeval* begin)
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

inline void tstat_calc_total(struct traffic_stat* self)
{
    self->rx_bytes_total    += self->rx_bytes_period;
    self->rx_packets_total  += self->rx_packets_period;
    self->tx_bytes_total    += self->tx_bytes_period;
    self->tx_packets_total  += self->tx_packets_period;
}

inline void tstat_clear_period(struct traffic_stat* self)
{
    self->rx_bytes_period = 0;
    self->rx_packets_period = 0;
    self->tx_bytes_period = 0;
    self->tx_packets_period = 0;
}

static inline void tstat_output_period(struct traffic_stat* self, int bps)
{
    struct timeval newtime, difference;

    evutil_gettimeofday(&newtime, NULL);
    evutil_timersub(&newtime, &(self->begin_time_period), &difference);
    double elapsed = difference.tv_sec + (difference.tv_usec / 1.0e6);

    self->begin_time_period = newtime;

    linfo("elapsed time:%lf, rx_bytes:%llu rx_packets:%llu, tx_bytes:%llu tx_packets:%llu\n", elapsed,
            self->rx_bytes_period, self->rx_packets_period, self->tx_bytes_period, self->tx_packets_period);

    double rx_speed = 0, tx_speed = 0;
    if (elapsed > 0.000001)
    {
        rx_speed = self->rx_bytes_period / elapsed / bps;
        tx_speed = self->tx_bytes_period / elapsed / bps;
    }

    linfo("rx_speed:%.3f , tx_speed:%.3f %s\n", rx_speed, tx_speed, (bps == KBPS) ? "KBPS" : "MBPS");

    tstat_calc_total(self);
    tstat_clear_period(self);
}

static inline void tstat_output_total(struct traffic_stat* self)
{
    struct timeval newtime, difference;

    evutil_gettimeofday(&newtime, NULL);
    evutil_timersub(&newtime, &(self->begin_time_period), &difference);
    double elapsed = difference.tv_sec + (difference.tv_usec / 1.0e6);

    linfo("total elapsed time:%lf, rx_bytes:%llu rx_packets:%llu, tx_bytes:%llu tx_packets:%llu\n", elapsed,
            self->rx_bytes_total + self->rx_bytes_period, self->rx_packets_total + self->rx_packets_period,
            self->tx_bytes_total + self->tx_bytes_period, self->tx_packets_total + self->tx_packets_period);
}

//#ifdef __cplusplus
//}
//#endif

#endif /* COMMON_TRAFFIC_STAT_H */
