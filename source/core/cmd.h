#pragma once

#include "../os/type.h"

enum cmd_type {
    CMD_HELP,
    CMD_UNKNOWN,
    CMD_INVALID_SYNTAX,
    CMD_CHANGE_DIR,
    CMD_EXIT,
};

struct cmd {
    enum cmd_type type;
    union {
        os_char *new_dir;
    } val;
};

struct cmd *cmd_from_str(const char *str);
void cmd_del(struct cmd *obj);