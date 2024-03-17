#include "cmd.h"
#include "args.h"
#include "io_wrap.h"
#include "../os/operations.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool is_minibat_file(const os_char *file) {
    const os_char *extension = get_file_extension(file);
    if(!extension || strlen(extension) != 2) {
        return false;
    }
    return extension[0] == 'm' && extension[1] == 'b';
}

void cmd_init_from_str(struct cmd *res, const char *str) {
    assert(res && "NULL input");
    res->type = CMD_UNKNOWN;

    struct args arguments;
    const bool valid = args_init_from_str(&arguments, str);

    if(!valid) {
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
    } else if(strcmp(name, "set") == 0) {
        if(arguments.argc < 2 || arguments.argc > 3) {
            format_error("Provide variable name and optionally a value\n");
            res->type = CMD_INVALID_SYNTAX;
        } else if(arguments.argc == 2) {
            res->type = CMD_SET_ENV;
            const unsigned int name_len = strlen(arguments.argv[1]);
            res->val.env.name = malloc(name_len + 1);
            memcpy(res->val.env.name, arguments.argv[1], name_len);
            res->val.env.name[name_len] = '\0';
            res->val.env.val = NULL;
        } else if(arguments.argc == 3) {
            res->type = CMD_SET_ENV;
            const unsigned int name_len = strlen(arguments.argv[1]);
            res->val.env.name = malloc(name_len + 1);
            memcpy(res->val.env.name, arguments.argv[1], name_len);
            res->val.env.name[name_len] = '\0';

            const unsigned int val_len = strlen(arguments.argv[2]);
            res->val.env.val = malloc(val_len + 1);
            memcpy(res->val.env.val, arguments.argv[2], val_len);
            res->val.env.val[val_len] = '\0';
        }
    } else if(strcmp(name, "unset") == 0) {
        if(arguments.argc != 2) {
            format_error("Expected variable name\n");
            res->type = CMD_INVALID_SYNTAX;
        } else {
            res->type = CMD_UNSET_ENV;
            const unsigned int len = strlen(arguments.argv[1]);
            res->val.env.name = malloc(len + 1);
            memcpy(res->val.env.name, arguments.argv[1], len);
            res->val.env.name[len] = '\0';
            res->val.env.val = NULL;
        }
    } else if(strcmp(name, "env") == 0) {
        if(arguments.argc == 1) {
            res->type = CMD_GET_ENV_ALL;
        } else if(arguments.argc == 2) {
            res->type = CMD_GET_ENV;
            const unsigned int len = strlen(arguments.argv[1]);
            res->val.env.name = malloc(len + 1);
            memcpy(res->val.env.name, arguments.argv[1], len);
            res->val.env.name[len] = '\0';
            res->val.env.val = NULL;
        } else {
            format_error("Too many arguments\n");
            res->type = CMD_INVALID_SYNTAX;
        }
    } else if(arguments.argc && is_minibat_file(arguments.argv[0])) {
        if(arguments.argc != 1) {
            format_error("Too many arguments\n");
            res->type = CMD_INVALID_SYNTAX;
        } else {
            res->type = CMD_MINIBAT;
            struct args *args = args_deep_copy(&arguments);
            res->val.args = *args;
            free(args);
        }
    } else {
        res->type = CMD_LAUNCH_EXECUTABLE;
        struct args *args = args_deep_copy(&arguments);
        res->val.args = *args;
        free(args);
    }

    args_destroy(&arguments);
}

void cmd_destroy(struct cmd *obj) {
    switch(obj->type) {
    case CMD_CHANGE_DIR:
        free(obj->val.new_dir);
        break;
    case CMD_SET_ENV:
    case CMD_UNSET_ENV:
    case CMD_GET_ENV:
        free(obj->val.env.name);
        free(obj->val.env.val);
        break;
    case CMD_MINIBAT: // NOLINT
        args_destroy(&(obj->val.args)); // NOLINT
        break; // NOLINT
    case CMD_LAUNCH_EXECUTABLE:
        args_destroy(&(obj->val.args));
        break;
    default:
        break;
    }
}