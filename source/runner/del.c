#include "del.h"
#include "../os/operations.h"

enum run_result run_del(const struct args args) {
    bool result = del(args);
    return result ? RUN_OK : RUN_FAILED;
}