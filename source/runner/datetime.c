#include "datetime.h"

enum run_result run_get_time() {
    return get_time() ? RUN_OK : RUN_FAILED;
}

enum run_result run_get_date() {
    return get_date() ? RUN_OK : RUN_FAILED;
}