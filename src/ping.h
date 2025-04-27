#ifndef PING_H
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define true 1
#define false 0
#define EXIT_FAILURE 1
#define TTL 64

typedef int boolean;

typedef struct {
    boolean verbose;
    char *host;
    char *ip;
    struct sockaddr_in addr;
} Params;

void parse_params(Params *params, char **argv, int argc);
void resolve_dns(Params *params);
void run_ping(Params params);
char *reverse_resolve_dns(struct sockaddr *addr, char *ip);

void display_ping_message(int eq, Params *params);
void display_stat(Params *params);

#endif
