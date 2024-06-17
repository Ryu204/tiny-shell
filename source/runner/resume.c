#include "resume.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_resume(const os_char *proc_id){
    bool result = resume(proc_id);
    return result ? RUN_OK : RUN_FAILED;
}