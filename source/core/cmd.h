#pragma once

enum cmd_type {
    CMD_HELP,
    CMD_UNKNOWN,
    CMD_EXIT,
};

struct cmd {
    enum cmd_type type;
    union {
    } val;
};

struct cmd *cmd_from_str(const char *str);
void cmd_del(struct cmd *obj);