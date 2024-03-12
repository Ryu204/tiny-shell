#include "io_wrap.h"
#include "../os/operations.h"
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

bool *is_prompt_visible() {
    static bool visible = true;
    return &visible;
}

void io_set_prompt_visibility(bool visible) {
    *is_prompt_visible() = visible;
}

void prompt_input();
void scan_input(struct cmd *obj) {
    prompt_input();
    static char buffer[INPUT_BUFFER_SIZE] = {0};
    if(!fgets(buffer, INPUT_BUFFER_SIZE, stdin)) {
        return cmd_init_from_str(obj, "exit");
    }
    return cmd_init_from_str(obj, buffer);
}

void format_output(char *fmt, ...) {
    va_list argptr; // NOLINT
    va_start(argptr, fmt);
    vfprintf(stdout, fmt, argptr);
    va_end(argptr);
}

void format_error(char *fmt, ...) {
    printf("ERROR: ");
    va_list argptr; // NOLINT
    va_start(argptr, fmt);
    vfprintf(stdout, fmt, argptr);
    va_end(argptr);
}

void prompt_input() {
    static os_char buffer[CWD_BUFFER_SIZE];
    get_cwd(CWD_BUFFER_SIZE, buffer);
    if(!*is_prompt_visible()) {
        return;
    }

    switch(*last_run_status()) {
    case RUN_OK:
        printf("%s\n-> ", buffer);
        break;
    case RUN_EXIT:
        break;
    case RUN_FAILED:
        printf("%s\n:( -> ", buffer);
        break;
    default:
        assert(false && "unimplemented");
    }
}