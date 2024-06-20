#include "stop.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_stop(const os_char *proc_id) {
    bool result = stop_proccess(proc_id);
    return result ? RUN_OK : RUN_FAILED;
}