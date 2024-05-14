#include "list.h"

enum run_result run_get_list_process() {
    return enum_proc() ? RUN_OK : RUN_FAILED;
}
