#include "core/config.h"
#include "core/io_wrap.h"
#include "runner/invoke.h"

#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Version report
    if(argc >= 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)) {
        format_output("%s, version %s\n", APP_NAME, APP_VERSION);
        return 0;
    }

    while(true) {
        // Scan the input line and convert it to `cmd`
        struct cmd *cmd = scan_input();
        // Invoke the corresponding function
        enum run_result res = invoke_runner(cmd);
        // Set status indicator
        io_set_last_status(res);
        // Deallocate the command
        cmd_del(cmd);

        format_output("\n");
        if(res == RUN_EXIT)
            return 0;
    }
}