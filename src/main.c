#include "ping.h"

struct sockaddr_in get_sockaddrin(char *ip);

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ping [-v] host/ip");
    }
    Params params;
    struct sockaddr_in addr;
    parse_params(&params, argc, argv);
    if (params.target_type == HOST) {
        addr = resolve_dns(&params);
    } else {
        addr = get_sockaddrin(params.ip);
    }
    send_ping(addr);
}

struct sockaddr_in get_sockaddrin(char *ip) {
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);

    if (inet_pton(AF_INET, ip, &(addr.sin_addr)) != 1) {
        fprintf(stderr, "Error: ip addres wrong format %s", ip);
        exit(EXIT_FAILURE);
    }

    return addr;
}
