#include "del.h"
#include "../os/operations.h"

enum run_result run_del(const os_char *dir) {
    bool result = del(dir);
    return result ? RUN_OK : RUN_FAILED;
}