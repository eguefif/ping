#include "ping.h"

void gather_statistics(Stats *stats, long double elapsed, boolean success) {

    stats->acc += (unsigned long)elapsed;
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

long double subtract_time(struct timeval after, struct timeval before) {
    long double total_after;
    long double total_before;

    total_after = after.tv_sec * 1000 + (double)after.tv_usec / 1000;
    total_before = before.tv_sec * 1000 + (double)before.tv_usec / 1000;

    return total_after - total_before;
}
