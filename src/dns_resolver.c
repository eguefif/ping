#include "ping.h"

void resolve_dns(Params *params) {
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *tmp;
    int error;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    if ((error = getaddrinfo(params->host, NULL, &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }

    if (result == NULL) {
        fprintf(stdout, "DNS resolution failed: no result\n");
        return;
    }

    printf("The '%s' domain name resolves to:\n", params->host);
    for (tmp = result; tmp != NULL; tmp = result->ai_next) {
        void *addr;

        struct sockaddr_in *ipv4 = (struct sockaddr_in *)tmp->ai_addr;
        addr = &(ipv4->sin_addr);

        char addr_ip[100];
        inet_ntop(tmp->ai_family, addr, addr_ip, sizeof addr_ip);
        printf("=> %s\n", addr_ip);
    }
    params->dns_results = result;
}
