#include "help.h"
#include "../core/io_wrap.h"

enum run_result run_help() {
    format_output("%s",
                  "\"help\":    print this message\n"
                  "\"cd\":      change directory\n"
                  "\"exit\":    exit shell\n"
                  "\"kill\":    kill a process\n"
                  "\"resume\":  resume running a process\n"
                  "\"child\":  Show all child processes\n");
    return RUN_OK;
}