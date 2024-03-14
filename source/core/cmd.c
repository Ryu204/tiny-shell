#include "cmd.h"
#include "args.h"
#include "io_wrap.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void cmd_init_from_str(struct cmd *res, const char *str) {
    assert(res && "NULL input");
    res->type = CMD_UNKNOWN;

    struct args arguments;
    args_init_from_str(&arguments, str);

    if(arguments.argc == 0) {
        args_destroy(&arguments);
        res->type = CMD_NOOP;
        return;
    }

    const char *name = arguments.argv[0];

    if(strcmp(name, "help") == 0) {
        res->type = CMD_HELP;
    } else if(strcmp(name, "exit") == 0) {
        res->type = CMD_EXIT;
    } else if(strcmp(name, "cd") == 0) {
        res->type = CMD_CHANGE_DIR;
        if(arguments.argc != 2) {
            format_error("Expected directory name, got %d argument(s)\n", (int)(arguments.argc - 1));
            res->type = CMD_INVALID_SYNTAX;
        } else {
            const unsigned int len = strlen(arguments.argv[1]);
            res->val.new_dir = malloc(len + 1);
            memcpy(res->val.new_dir, arguments.argv[1], len);
            res->val.new_dir[len] = '\0';
        }
    } else if(strcmp(name, "clear") == 0) {
        res->type = CMD_CLEAR;
    } else if(strcmp(name, "fore") == 0) {
        res->type = CMD_LAUNCH_FOREGROUND;

        const char *tmp_str = strstr(str, "fore") + 4;

        // format_output("DEBUG: tmp_str=%s:%d", tmp_str, strlen(tmp_str));

        while(*tmp_str != '\0' && is_whitespace(*tmp_str)) {
            ++tmp_str;
        }

        size_t len = strlen(tmp_str);
        while(len > 0 && is_whitespace(*(tmp_str + len - 1))) {
            --len;
        }

        res->val.command_line = malloc((len + 1) * sizeof(os_char));
        for(int i = 0; i < len; ++i) {
            res->val.command_line[i] = tmp_str[i];
        }
        res->val.command_line[len] = '\0';
    }

    args_destroy(&arguments);
}

void cmd_destroy(struct cmd *obj) {
    switch(obj->type) {
    case CMD_CHANGE_DIR:
        free(obj->val.new_dir);
        break;
    case CMD_LAUNCH_FOREGROUND:
        free(obj->val.command_line);
    default:
        break;
    }
}