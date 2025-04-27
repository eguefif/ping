#include "ping.h"

struct addrinfo *get_addr_info(char *host);
void display_result(struct addrinfo *result, char *host);

struct sockaddr_in resolve_dns(Params *params) {
    struct addrinfo *result;
    struct sockaddr_in addr;

    result = get_addr_info(params->host);
    display_result(result, params->host);
    memcpy(&addr, result->ai_addr, sizeof(addr));
    return addr;
}

struct addrinfo *get_addr_info(char *host) {
    struct addrinfo hints;
    struct addrinfo *result;
    int error;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    if ((error = getaddrinfo(host, NULL, &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }

    if (result == NULL) {
        fprintf(stdout, "DNS resolution failed: no result\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

void display_result(struct addrinfo *result, char *host) {
    struct addrinfo *tmp;
    void *addr;
    char *ip_version;
    int i = 0;

    printf("The '%s' domain name resolves to:\n", host);
    for (tmp = result; tmp != NULL; tmp = tmp->ai_next) {
        i++;

        if (tmp->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)tmp->ai_addr;
            addr = &(ipv4->sin_addr);
            ip_version = "ipv4";
        } else {

            struct sockaddr_in *ipv6 = (struct sockaddr_in *)tmp->ai_addr;
            addr = &(ipv6->sin_addr);
            ip_version = "ipv6";
        }

        char addr_ip[100];
        inet_ntop(tmp->ai_family, addr, addr_ip, sizeof addr_ip);
        printf("%d => %s(%s)\n", i, addr_ip, ip_version);
    }
}
