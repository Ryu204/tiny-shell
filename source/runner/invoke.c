#include "invoke.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"
#include "cd.h"
#include "help.h"

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
    case CMD_CLEAR:
        clear_screen();
        return RUN_OK;
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
    }
}