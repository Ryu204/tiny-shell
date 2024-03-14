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
    const bool valid = args_init_from_str(&arguments, str);

    if (!valid) {
        res->type = CMD_INVALID_SYNTAX;
        return;
    }

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
    } else {
        res->type = CMD_LAUNCH_EXECUTABLE;
        res->val.args.argc = arguments.argc;
        res->val.args.argv = malloc(arguments.argc * sizeof(os_char *));
        for(int i = 0; i < arguments.argc; ++i) {
            size_t len = strlen(arguments.argv[i]);
            res->val.args.argv[i] = malloc((len + 1) * sizeof(os_char));
            memcpy(res->val.args.argv[i], arguments.argv[i], len * sizeof(os_char));
            res->val.args.argv[i][len] = '\0';
        }
    }

    args_destroy(&arguments);
}

void cmd_destroy(struct cmd *obj) {
    switch(obj->type) {
    case CMD_CHANGE_DIR:
        free(obj->val.new_dir);
        break;
    case CMD_LAUNCH_EXECUTABLE:
        args_destroy(&(obj->val.args));
        break;
    default:
        break;
    }
}