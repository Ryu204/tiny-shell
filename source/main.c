#include "core/config.h"
#include "core/io_wrap.h"
#include "runner/invoke.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Version report
    if(argc >= 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)) {
        format_output("%s, version %s\n", APP_NAME, APP_VERSION);
        return EXIT_SUCCESS;
    }
    // No prompt
    if(argc >= 2 && (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--silent") == 0)) {
        io_set_prompt_visibility(false);
    }
    // Exec a file
    if(argc >= 3 && (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0)) {
        if(!freopen(argv[2], "r", stdin)) {
            format_error("Cannot open file \"%s\"\n", argv[2]);
            return EXIT_FAILURE;
        }
        io_set_prompt_visibility(false);
    }

    while(true) {
        // Scan the input line and convert it to `cmd`
        struct cmd cmd;
        scan_input(&cmd);
        // Invoke the corresponding function
        enum run_result res = invoke_runner(&cmd);
        // Set status indicator
        io_set_last_status(res);
        // Deallocate the command
        cmd_destroy(&cmd);
        format_output("\n");
        if(res == RUN_EXIT){
            return EXIT_SUCCESS;
        }
    }
}