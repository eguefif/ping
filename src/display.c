#include "ping.h"

double get_avg(Stats stats);
double get_mdev(Stats stats);

void display_ping_message(int seq, Params *params, double elapsed) {
    printf("%d bytes from %s(%s): icmp_seq=%d, ttl=%d, time=%d ms\n", 64,
           params->host, params->ip, seq, TTL, (int)elapsed);
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
    printf("rtt min/avg/max/mdev = %3.2lf/%3.2lf/%3.2lf/%3.2lf ms\n", stats.min,
           get_avg(stats), stats.max, get_mdev(stats));
}

double get_avg(Stats stats) { return stats.acc / (double)stats.success_count; }

double get_mdev(Stats stats) { return stats.acc / (double)stats.success_count; }
