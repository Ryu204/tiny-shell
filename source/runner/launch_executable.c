#include "launch_executable.h"
#include "../os/operations.h"

enum run_result run_launch_executable(const struct args args) {
    bool result = launch_executable(args);
    return result ? RUN_OK : RUN_FAILED;
}