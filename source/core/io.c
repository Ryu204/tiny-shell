#include "io.h"
#include "config.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

enum run_result *last_run_status() {
    static enum run_result res = RUN_OK;
    return &res;
}

void io_set_last_status(enum run_result result) {
    *last_run_status() = result;
}

void prompt_input();
struct cmd *scan_input() {
    prompt_input();
    static char buffer[INPUT_BUFFER_SIZE] = {0};
    fgets(buffer, INPUT_BUFFER_SIZE, stdin);
    return cmd_from_str(buffer);
}

void format_output(char *fmt, ...) {
    va_list argptr = NULL;
    va_start(argptr, fmt);
    vfprintf(stdout, fmt, argptr);
    va_end(argptr);
}

void prompt_input() {
    switch(*last_run_status()) {
    case RUN_OK:
        printf(":) --> ");
        break;
    case RUN_EXIT:
        printf(":(( --> ");
        break;
    case RUN_FAILED:
        printf(":( --> ");
        break;
    default:
        assert(false && "unimplemented");
    }
}