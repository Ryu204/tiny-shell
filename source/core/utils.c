#include "utils.h"
#include "../os/operations.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

bool is_number(const char *c) {
    if(c == NULL || c[0] == '\0')
        return false;
    const unsigned int length = strlen(c);
    static const unsigned int MAX_INT_DIGITS = 10;
    if(length > MAX_INT_DIGITS)
        return false;
    for(int i = 0; i < length; ++i) {
        if(c[i] < '0' || c[i] > '9')
            return false;
    }
    return true;
}

#ifdef _WIN32
#    include <VersionHelpers.h>
bool priv_support_color() {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if(handle == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD mode = 0;
    if(!GetConsoleMode(handle, &mode)) {
        return false;
    }
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    const bool succeed = SetConsoleMode(handle, mode);
    return succeed;
}
#elif defined(__linux__)
#    include <stdlib.h>
bool priv_support_color() {
#    define BUFFER_LENGTH 100
    char buffer[BUFFER_LENGTH];
    const char *term = getenv("TERM"); // NOLINT
    const char *color_terms[] = {
        "xterm", "xterm-color", "xterm-256color", "screen", "screen-256color",
        "tmux", "tmux-256color", "linux", "cygwin", "rxvt-unicode-256color",
        "xterm-kitty"};
    const unsigned int len = sizeof(color_terms) / sizeof(color_terms[0]);
    for(size_t i = 0; i < len; i++) {
        if(strcmp(term, color_terms[i]) == 0) {
            return true;
        }
    }
    return false;
}
#endif

bool support_color() {
    static bool res = false;
    static bool checked = false;

    if(!checked) {
        res = priv_support_color();
        checked = true;
    }

    return res;
}