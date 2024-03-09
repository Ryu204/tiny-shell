#include "core/io.h"
#include "runner/select.h"

#include <stdbool.h>

int main() {
    while(true) {
        struct cmd *cmd = scan_input();
        enum run_result res = select_runner(cmd);
        io_set_last_status(res);
        cmd_del(cmd);

        if(res == RUN_EXIT)
            return 0;
    }
}