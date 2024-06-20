#include "invoke.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"
#include "add_path.h"
#include "cd.h"
#include "delete_file.h"
#include "env.h"
#include "help.h"
#include "launch_executable.h"
#include "list.h"
#include "lsdir.h"
#include "minibat.h"
#include "datetime.h"
#include "stop.h"

#include <assert.h>
#include <stdbool.h>

enum run_result invoke_runner(const struct cmd *cmd) {
    switch(cmd->type) {
    case CMD_HELP:
        return run_help();
    case CMD_EXIT:
        format_output("%s", "Goodbye\n");
        return RUN_EXIT;
    case CMD_CHANGE_DIR:
        return run_cd(cmd->val.new_dir);
    case CMD_DEL_FILE:
        return run_del(cmd->val.filename);
    case CMD_CLEAR:
        clear_screen();
        return RUN_OK;
    case CMD_LSDIR:
        return run_lsdir(cmd->val.dir);
    case CMD_LIST:
        return run_get_list_process();
    case CMD_TIME:
        return run_get_time();
    case CMD_DATE:
        return run_get_date();
    case CMD_STOP_PROC:
        return run_stop(cmd->val.args);
    case CMD_LAUNCH_EXECUTABLE:
        return run_launch_executable(cmd->val.args);
    case CMD_ADD_PATH:
        return run_add_path(cmd->val.new_path);
    case CMD_MINIBAT:
        return run_minibat(cmd->val.args);
    case CMD_SET_ENV:
        return run_set_env(cmd->val.env.name, cmd->val.env.val);
    case CMD_UNSET_ENV:
        return run_unset_env(cmd->val.env.name);
    case CMD_GET_ENV_ALL:
        return run_get_all_env();
    case CMD_GET_ENV:
        return run_get_env(cmd->val.env.name);
    case CMD_UNKNOWN:
        format_output("%s", "Unknown command. Use \"help\" for more information\n");
        return RUN_FAILED;
    case CMD_NOOP:
        return RUN_OK;
    case CMD_INVALID_SYNTAX:
        format_output("%s", "Invalid syntax. Use \"help\" for more information\n");
        return RUN_FAILED;
    default:
        assert(false && "unimplemented command");
        return RUN_FAILED;
    }
}