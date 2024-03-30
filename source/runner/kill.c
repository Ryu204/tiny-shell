#include "kill.h"
#include "../core/io_wrap.h"

enum run_result run_kill(PROCESS_INFORMATION pi){
    kill(pi);
}