#include "ping.h"

void gather_statistics(Stats *stats, unsigned long elapsed, boolean success) {

    stats->acc += elapsed;
    stats->acc2 += (long long)elapsed * (long long)elapsed;

    if (stats->min > elapsed || stats->min == 0) {
        stats->min = elapsed;
    }
    if (stats->max < elapsed) {
        stats->max = elapsed;
    }
    stats->count++;
    if (success) {
        stats->success_count++;
    }
}

unsigned long subtract_time(struct timeval after, struct timeval before) {
    unsigned long total_after;
    unsigned long total_before;

    total_after = after.tv_sec * 1000000 + (unsigned long)after.tv_usec;
    total_before = before.tv_sec * 1000000 + (unsigned long)before.tv_usec;
    return total_after - total_before;
}

double to_ms(unsigned long elapsed) { return (double)elapsed / 1000; }
