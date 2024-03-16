#include "minibat.h"

enum run_result run_minibat(const struct args args) {
    bool result = minibat(args);
    return result ? RUN_OK : RUN_FAILED;
}