#pragma once

#include "../os/type.h"

/**
 * @brief Possible command types the shell can understand
 */
enum cmd_type {
    CMD_HELP,
    CMD_UNKNOWN,
    CMD_INVALID_SYNTAX,
    CMD_CHANGE_DIR,
    CMD_EXIT,
};

/**
 * @brief Resulting struct after parsing the input
 */
struct cmd {
    enum cmd_type type;
    union {
        os_char *new_dir;
    } val;
};

/*! @brief Build command from raw input */
struct cmd *cmd_from_str(const char *str);
void cmd_del(struct cmd *obj);