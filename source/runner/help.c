#include "help.h"
#include "../core/io_wrap.h"

enum run_result run_help() {
    format_output("\"help\":    print this message\n"
                  "\"cd\":      change directory\n"
                  "\"exit\":    exit shell\n"
                  "\"kill\":    kill a process\n"
                  "\"resume\":  resume running a process\n"
                  "\"child\":   show all child processes\n"
                  "\"delete\":  delete directory\n"
                  "\"list\":    list all running processes\n"
                  "\"lsdir\":   list of files or folders in specific directory\n"
                  "\"time\":    get current time\n"
                  "\"date\":    get current date\n"
                  "\"stop\":    stop a process\n");
    return RUN_OK;
}