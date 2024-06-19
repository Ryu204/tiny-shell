#include "lsdir.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_lsdir(const os_char *dir) {
    bool result = lsdir(dir);
    return result ? RUN_OK : RUN_FAILED;
}