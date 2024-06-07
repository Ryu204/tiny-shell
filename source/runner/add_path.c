#include "add_path.h"
#include "../os/operations.h"
#include <stdlib.h>

enum run_result run_add_path(const os_char *new_path) {
    return add_path(new_path) ? RUN_OK : RUN_FAILED;
}