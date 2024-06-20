#include "child_processes.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_child_processes(const os_char *proc_id){
    bool result = show_child_processes(proc_id);
    return result ? RUN_OK : RUN_FAILED;
}