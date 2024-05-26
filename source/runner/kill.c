#include "kill.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_kill(const struct args args){
    bool result = kill(args);
    return result ? RUN_OK : RUN_FAILED;
}