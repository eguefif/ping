#ifndef PING_H
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define true 1
#define false 0
#define EXIT_FAILURE 1
#define TTL 64

typedef int boolean;

typedef struct {
    char *target;
    boolean verbose;
    char *host;
    char *ip;
    struct sockaddr_in addr;
} Params;

typedef struct {
    int count;
    int success_count;
    long double acc;
    double min;
    double max;
} Stats;

void parse_params(Params *params, char **argv, int argc);
void resolve_dns(Params *params);
void run_ping(Params params);
char *reverse_resolve_dns(struct sockaddr *addr, char *ip);

void display_ping_message(int eq, Params *params, double elapsed);
void display_stat(Params *params, Stats stats);
void display_unreachable(int seq, Params *params);

long double subtract_time(struct timeval after, struct timeval before);
void gather_statistics(Stats *stats, long double elapsed, boolean success);
#endif
