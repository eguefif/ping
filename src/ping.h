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

typedef enum {
    HOST,
    IP,
} target_type;

typedef struct {
    boolean verbose;
    boolean target_type;
    char *host;
    char *ip;
    struct addrinfo *dns_result;
} Params;

void parse_params(Params *params, int argc, char **argv);
struct sockaddr_in resolve_dns(Params *params);
void send_ping(struct sockaddr_in addr);

#endif
