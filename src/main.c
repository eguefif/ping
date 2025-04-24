#include "ping.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ping [-v] host/ip");
    }
    Params params;
    parse_params(&params, argc, argv);
    if (params.target_type == HOST) {
        resolve_dns(&params);
    }
}
