#include "stop.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_stop(const struct args args){
    bool result = stop_proccess(args);
    return result ? RUN_OK : RUN_FAILED;
}