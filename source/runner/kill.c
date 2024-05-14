#include "kill.h"
#include "../core/io_wrap.h"

enum run_result run_kill(const struct args args){
    bool result = kill(args);
    return result ? RUN_OK : RUN_FAILED;
}