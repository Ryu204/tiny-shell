#include "cmd.h"
#include "args.h"
#include "io_wrap.h"
#include "utils.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool is_minibat_file(const os_char *file) {
    const os_char *dot = strrchr(file, '.');
    if(dot == NULL) {
        return false;
    }
    return !strcmp(dot + 1, "mb");
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
    } else if(strcmp(name, "list") == 0) {
        res->type = CMD_LIST;
    } else if(strcmp(name, "date") == 0) {
        res->type = CMD_DATE;
    } else if(strcmp(name, "time") == 0) {
        res->type = CMD_TIME;
    } else if(strcmp(name, "stop") == 0) {
        if(arguments.argc != 2 || !is_number(arguments.argv[1])) {
            res->type = CMD_INVALID_SYNTAX;
        } else {
            res->type = CMD_STOP_PROC;
            res->val.proc_id = atoi(arguments.argv[1]);
        }
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
    } else if(strcmp(name, "addpath") == 0) {
        if(arguments.argc != 2) {
            format_error("Unexpected number of arguments\n");
            res->type = CMD_INVALID_SYNTAX;
        } else {
            res->type = CMD_ADD_PATH;
            const size_t len = strlen(arguments.argv[1]);
            res->val.new_path = (os_char *)malloc((len + 1) * sizeof(os_char));
            res->val.new_path[len] = '\0';
            memcpy(res->val.new_path, arguments.argv[1], strlen(arguments.argv[1]) * sizeof(os_char));
        }
    } else if(strcmp(name, "delete") == 0) {
        if(arguments.argc > 2 || arguments.argc < 2) {
            res->type = CMD_INVALID_SYNTAX;
        } else {
            res->type = CMD_DEL_FILE;

            const unsigned int dir_len = strlen(arguments.argv[1]);
            res->val.filename = malloc(dir_len + 1);
            memcpy(res->val.filename, arguments.argv[1], dir_len);
            res->val.filename[dir_len] = '\0';
        }
    } else if(strcmp(name, "lsdir") == 0) {
        if(arguments.argc != 2) {
            format_error("Usage: lsdir <dir>\n");
            res->type = CMD_INVALID_SYNTAX;
        } else {
            res->type = CMD_LSDIR;

            const unsigned int dir_len = strlen(arguments.argv[1]);
            res->val.dir = malloc(dir_len + 1);
            memcpy(res->val.dir, arguments.argv[1], dir_len);
            res->val.dir[dir_len] = '\0';
        }
    } else if(arguments.argc && is_minibat_file(arguments.argv[0])) {
        if(arguments.argc != 1) {
            format_error("Too many arguments\n");
            res->type = CMD_INVALID_SYNTAX;
        } else {
            res->type = CMD_MINIBAT;
            args_deep_copy_init(&res->val.args, &arguments);
        }
    } else {
        res->type = CMD_LAUNCH_EXECUTABLE;
        args_deep_copy_init(&res->val.args, &arguments);
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
    case CMD_ADD_PATH:
        free(obj->val.new_path);
        break;
    case CMD_DEL_FILE:
        free(obj->val.filename);
        break;
    case CMD_LSDIR:
        free(obj->val.dir);
        break;
    case CMD_MINIBAT:
    case CMD_LAUNCH_EXECUTABLE:
        args_destroy(&(obj->val.args));
        break;
    default:
        break;
    }
}