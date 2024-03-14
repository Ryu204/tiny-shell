#include "launch_executable.h"
#include "../os/operations.h"
#include <stdio.h>

bool is_empty_str(os_char *str){
    return !strcmp(str, "");
}

enum run_result run_launch_executable(const struct args args) {
    bool wait = true;
    bool flag = false;
    size_t len = 0;
    if(!is_empty_str(args.argv[0])) {
        len += strlen(args.argv[0]) + 2;
    }
    for(int i = 1; i < args.argc; ++i) {
        if(is_empty_str(args.argv[i])) {
            continue;
        }
        if(strcmp(args.argv[i], "&") != 0 || flag) {
            len += strlen(args.argv[i]) + 3;
        }
        else
        if(!flag) {
            wait = false;
            flag = true;
        }
    }

    os_char *command_line = malloc((len + 1) * sizeof(os_char));
    flag = false;
    len = 0;
    if(!is_empty_str(args.argv[0])) {
        sprintf(command_line, "\"%s\"", args.argv[0]);
        len += strlen(args.argv[0]) + 2;
    }
    for(int i = 1; i < args.argc; ++i) {
        if(is_empty_str(args.argv[i])) {
            continue;
        }
        if(strcmp(args.argv[i], "&") != 0 || flag) {
            sprintf(command_line + len, " \"%s\"", args.argv[i]);
            len += strlen(args.argv[i]) + 3;
        }
        else
        if(!flag) {
            flag = true;
        }
    }

    command_line[len] = '\0';

    bool result = launch_executable(command_line, wait);

    free(command_line);

    return result ? RUN_OK : RUN_FAILED;
}