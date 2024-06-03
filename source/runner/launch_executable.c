#include "launch_executable.h"
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>

void handler(int sig) {
    return;
}

enum run_result run_launch_executable(const struct args args) {
    if (!args.background) {
        signal(SIGINT, handler);
    }
    bool result = launch_executable(args);
    return result ? RUN_OK : RUN_FAILED;
}