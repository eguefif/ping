#include "ping.h"

void parse_target(char *target, Params *params);
boolean is_ip_addr(char *target);

void parse_params(Params *params, int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-v", 2) == 0) {
            params->verbose = true;
        } else {
            parse_target(argv[i], params);
        }
    }
}

void parse_target(char *target, Params *params) {
    char *check = malloc(sizeof(char) * strlen(target));
    strncpy(check, target, strlen(target));
    if (is_ip_addr(check)) {
        params->ip = target;
        params->target_type = IP;
    } else {
        params->host = target;
        params->target_type = HOST;
    }
    free(check);
}

boolean is_ip_addr(char *target) {
    char *next = strtok(target, ".");
    int counter = 0;
    while (next != NULL) {
        int chunk = atoi(next);
        if (chunk < 0 || chunk > 255) {
            return false;
        }
        next = strtok(NULL, ".");
        counter++;
    }
    if (counter != 4) {
        return false;
    }
    return true;
}
