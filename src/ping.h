#ifndef PING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

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
    char *error[150];
    boolean error_flag;

} Params;

void parse_params(Params *params, int argc, char **argv);
#endif
