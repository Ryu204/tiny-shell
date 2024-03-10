#include "cd.h"
#include "../os/operations.h"

enum run_result run_cd(os_char *new_dir) {
    return change_cwd(new_dir) ? RUN_OK : RUN_FAILED;
}