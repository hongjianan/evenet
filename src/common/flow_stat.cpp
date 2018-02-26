/*
 * flow_stat.cpp
 *
 *  Created on: 2018-1-16
 *      Author: Administrator
 */


#include "flow_stat.h"

void tstat_output_period(flow_stat_t* self, int bps)
{
    struct timeval newtime, difference;

    evutil_gettimeofday(&newtime, NULL);
    evutil_timersub(&newtime, &(self->begin_time_period), &difference);
    double elapsed = difference.tv_sec + (difference.tv_usec / 1.0e6);

    self->begin_time_period = newtime;

    linfo("elapsed time:%lf, rx_bytes:%llu rx_packets:%llu, tx_bytes:%llu tx_packets:%llu.", elapsed,
            self->rx_bytes_period, self->rx_packets_period, self->tx_bytes_period, self->tx_packets_period);

    double rx_speed = 0, tx_speed = 0;
    if (elapsed > 0.000001)
    {
        rx_speed = self->rx_bytes_period / elapsed / bps;
        tx_speed = self->tx_bytes_period / elapsed / bps;
    }

    linfo("rx_speed:%.3f , tx_speed:%.3f %s.", rx_speed, tx_speed, (bps == KBPS) ? "KBPS" : "MBPS");

    fstat_calc_total(self);
    fstat_clear_period(self);
}

void tstat_output_total(flow_stat_t* self)
{
    struct timeval newtime, difference;

    evutil_gettimeofday(&newtime, NULL);
    evutil_timersub(&newtime, &(self->begin_time_period), &difference);
    double elapsed = difference.tv_sec + (difference.tv_usec / 1.0e6);

    linfo("total elapsed time:%lf, rx_bytes:%llu rx_packets:%llu, tx_bytes:%llu tx_packets:%llu.", elapsed,
            self->rx_bytes_total + self->rx_bytes_period, self->rx_packets_total + self->rx_packets_period,
            self->tx_bytes_total + self->tx_bytes_period, self->tx_packets_total + self->tx_packets_period);
}
