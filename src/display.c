#include "ping.h"

double get_avg(Stats stats);
double get_mdev(Stats stats);

void display_ping_message(int seq, Params *params, unsigned long elapsed) {
    printf("%d bytes from %s(%s): icmp_seq=%d, ttl=%d, time=%2.lf ms\n", 64,
           params->host, params->ip, seq, TTL, to_ms(elapsed));
}

void display_unreachable(int seq, Params *params) {
    printf("%d bytes from %s(%s): icmp_seq=%d Destination Host Unreachable\n",
           64, params->host, params->ip, seq);
}

void display_stat(Params *params, Stats stats) {
    printf("--- %s ping statistics---\n", params->target);
    printf("%d packets transmitted, %d received, %2.2lf%% packet loss, time "
           "%dms\n",
           stats.count, stats.success_count,
           100 - (double)(((double)stats.success_count / (double)stats.count) *
                          100),
           0);
    printf("rtt min/avg/max/mdev = %3.2lf/%3.2lf/%3.2lf/%3.2lf ms\n",
           to_ms(stats.min), get_avg(stats), to_ms(stats.max), get_mdev(stats));
}

double get_avg(Stats stats) {
    return to_ms(stats.acc) / (double)stats.success_count;
}

double get_mdev(Stats stats) {
    double t1 = to_ms(stats.acc) / (double)stats.count;
    double t2 = to_ms(stats.acc2) / (double)stats.count;

    return sqrt(t2 - t1 * t1);
}
