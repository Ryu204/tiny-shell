#include "lsdir.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_lsdir(const struct args args){
    bool result = lsdir(args);
    return result ? RUN_OK : RUN_FAILED;
}