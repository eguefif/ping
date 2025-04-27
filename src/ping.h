#ifndef PING_H
#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
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
    unsigned long acc;
    unsigned long acc2;
    unsigned long min;
    unsigned long max;
} Stats;

void parse_params(Params *params, char **argv, int argc);
void resolve_dns(Params *params);
void run_ping(Params params);
char *reverse_resolve_dns(struct sockaddr *addr, char *ip);

void display_ping_message(int eq, Params *params, unsigned long elapsed);
void display_stat(Params *params, Stats stats);
void display_unreachable(int seq, Params *params);

unsigned long subtract_time(struct timeval after, struct timeval before);
void gather_statistics(Stats *stats, unsigned long elapsed, boolean success);
double to_ms(unsigned long elapsed);
#endif
