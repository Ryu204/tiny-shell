#include "resume.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_resume(const struct args args){
    bool result = resume(args);
    return result ? RUN_OK : RUN_FAILED;
}