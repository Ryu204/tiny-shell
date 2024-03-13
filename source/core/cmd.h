#pragma once

#include "../os/type.h"
#include "args.h"

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
    CMD_LAUNCH_BACKGROUND,
};

/**
 * @brief Resulting struct after parsing the input
 */
struct cmd {
    enum cmd_type type;
    union {
        os_char *command_line;
        os_char *new_dir;
        struct args args;
    } val;
};

/*! @brief Build command from raw input */
void cmd_init_from_str(struct cmd *obj, const char *str);
void cmd_destroy(struct cmd *obj);