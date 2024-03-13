#include "fore.h"
#include "../os/operations.h"

enum run_result run_fore(const os_char *command_line) {
    return fore(command_line) ? RUN_OK : RUN_FAILED;
}