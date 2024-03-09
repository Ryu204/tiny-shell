#include "select.h"
#include "help.h"

#include <assert.h>
#include <stdbool.h>

enum run_result select_runner(const struct cmd *cmd) {
    switch(cmd->type) {
    case CMD_HELP:
        return run_help();
    case CMD_EXIT:
        format_output("%s", "Goodbye");
        return RUN_EXIT;
    case CMD_UNKNOWN:
        format_output("%s", "Unknown command\n");
        return RUN_FAILED;
        break;
    default:
        assert(false && "unimplemented command");
    }
}