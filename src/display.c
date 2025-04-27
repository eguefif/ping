#include "ping.h"

void display_ping_message(int seq, Params *params) {
    printf("%d bytes from %s(%s): icmp_seq=%d, ttl=%d, time=%d ms\n", 64,
           params->host, params->ip, seq, TTL, 0);
}

void display_stat(Params *params) {
    printf("--- %s ping statistics---\n", params->host);
}
