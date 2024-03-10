#include "help.h"
#include "../core/io_wrap.h"

enum run_result run_help() {
    format_output("%s",
                  "\"help\":    print this message\n"
                  "\"cd\":      change directory\n"
                  "\"exit\":    exit shell\n");
    return RUN_OK;
}