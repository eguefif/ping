#ifndef PING_H
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
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

typedef int boolean;

typedef struct {
    char *host;
    char *ip;
    struct sockaddr_in addr;
} Params;

void parse_params(Params *params, char **argv);
struct sockaddr_in resolve_dns(Params *params);
void run_ping(Params params);
char *reverse_resolve_dns(struct sockaddr *addr, char *ip);

#endif
