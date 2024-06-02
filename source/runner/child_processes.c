#include "child_processes.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_child_processes(const struct args args){
    bool result = showChildProcesses(args);
    return result ? RUN_OK : RUN_FAILED;
}