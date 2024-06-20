#include "io_wrap.h"
#include "../os/operations.h"
#include "config.h"
#include "utils.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RED "\033[31m"
#define COLOR_BLUE "\033[34m"
#define COLOR_BLACK "\033[30m"

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
    char buffer[INPUT_BUFFER_SIZE] = {0};
    if(!fgets(buffer, INPUT_BUFFER_SIZE, stdin)) {
        cmd_init_from_str(obj, "exit");
    } else {
        cmd_init_from_str(obj, buffer);
    }
}

void format_usage(char *fmt, ...) {
    if(support_color())
        printf(COLOR_YELLOW);
    printf("USAGE: ");
    va_list argptr; // NOLINT
    va_start(argptr, fmt);
    vfprintf(stdout, fmt, argptr);
    va_end(argptr);
    if(support_color())
        printf(COLOR_BLACK);
}

void format_success(char *fmt, ...) {
    if(support_color())
        printf(COLOR_GREEN);
    va_list argptr; // NOLINT
    va_start(argptr, fmt);
    vfprintf(stdout, fmt, argptr);
    va_end(argptr);
    if(support_color())
        printf(COLOR_BLACK);
}

void format_output(char *fmt, ...) {
    if(support_color())
        printf(COLOR_BLACK);
    va_list argptr; // NOLINT
    va_start(argptr, fmt);
    vfprintf(stdout, fmt, argptr);
    va_end(argptr);
}

void format_error(char *fmt, ...) {
    if(support_color())
        printf(COLOR_RED);
    printf("ERR: ");
    va_list argptr; // NOLINT
    va_start(argptr, fmt);
    vfprintf(stdout, fmt, argptr);
    va_end(argptr);
    if(support_color())
        printf(COLOR_BLACK);
}

void prompt_input() {
    os_char buffer[CWD_BUFFER_SIZE];
    get_cwd(CWD_BUFFER_SIZE, buffer);
    if(!*is_prompt_visible()) {
        return;
    }
    switch(*last_run_status()) {
    case RUN_OK:
        if(support_color())
            printf(COLOR_BLUE);
        printf("%s\n-> ", buffer);
        break;
    case RUN_EXIT:
        break;
    case RUN_FAILED:
        if(support_color())
            printf(COLOR_BLUE "%s\n" COLOR_RED ":( -> ", buffer);
        else
            printf("%s\n:( -> ", buffer);
        break;
    default:
        assert(false && "unimplemented");
    }
    if(support_color()) {
        printf(COLOR_BLACK);
    }
}