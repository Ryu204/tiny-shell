#pragma once

#include "../os/type.h"

/**
 * @brief Possible command types the shell can understand
 */
enum cmd_type {
    CMD_UNKNOWN,
    CMD_INVALID_SYNTAX,
    CMD_NOOP,

    CMD_CHANGE_DIR,
    CMD_HELP,
    CMD_EXIT,
    CMD_CLEAR,

    CMD_LAUNCH_FOREGROUND,
};

/**
 * @brief Resulting struct after parsing the input
 */
struct cmd {
    enum cmd_type type;
    union {
        os_char *new_dir;
        struct {
            int argc;
            os_char **argv;
        } args;
    } val;
};

/*! @brief Build command from raw input */
struct cmd *cmd_from_str(const char *str);
void cmd_del(struct cmd *obj);