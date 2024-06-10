#include "help.h"
#include "../core/io_wrap.h"

enum run_result run_help() {
    format_output("%s",
                  "\"help\":    print this message\n"
                  "\"cd\":      change directory\n"
                  "\"exit\":    exit shell\n"
                  "\"time\":    get current time\n"
                  "\"date\":    get current date\n"
                  "\"list\":    get list of running processes\n"
                  "\"stop\":    stop a process\n"
                  
                  );
    return RUN_OK;
}