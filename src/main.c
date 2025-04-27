#include "ping.h"

struct sockaddr_in get_sockaddrin_from_str(char *ip);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ping [-v] host/ip");
    }
    Params params;
    parse_params(&params, argv);
    run_ping(params);
}

struct sockaddr_in get_sockaddrin_from_str(char *ip) {
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);

    if (inet_pton(AF_INET, ip, &(addr.sin_addr)) != 1) {
        fprintf(stderr, "Error: ip address wrong format %s", ip);
        exit(EXIT_FAILURE);
    }

    return addr;
}
