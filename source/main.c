#include "core/config.h"
#include "core/io.h"
#include "runner/select.h"

#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if(argc >= 2 && strcmp(argv[1], "-v") == 0) {
        format_output("%s, version %s", APP_NAME, APP_VERSION);
        return 0;
    }

    while(true) {
        struct cmd *cmd = scan_input();
        enum run_result res = select_runner(cmd);
        io_set_last_status(res);
        cmd_del(cmd);

        if(res == RUN_EXIT)
            return 0;
    }
}