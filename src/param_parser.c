#include "ping.h"

void parse_target(char *target, Params *params);
boolean is_ip_addr(char *target);

void parse_params(Params *params, char **argv) {
    parse_target(argv[1], params);
}

void parse_target(char *target, Params *params) {
    char *check = malloc(sizeof(char) * strlen(target));
    strncpy(check, target, strlen(target));
    if (is_ip_addr(check)) {
        params->ip = target;
    } else {
        params->host = target;
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
