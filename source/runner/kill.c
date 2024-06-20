#include "kill.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_kill(int proc_id) {
    bool result = kill(proc_id);
    return result ? RUN_OK : RUN_FAILED;
}