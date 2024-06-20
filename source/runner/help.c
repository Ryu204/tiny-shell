#include "help.h"
#include "../core/io_wrap.h"

enum run_result run_help() {
    format_output("addpath  append new path to PATH\n"
                  "cd       change directory\n"
                  "child    show all child processes\n"
                  "clear    clear the console\n"
                  "date     get current date\n"
                  "delete   delete a file permanently\n"
                  "env      get environment variables\n"
                  "exit     exit shell\n"
                  "help     print this message\n"
                  "kill     kill a process\n"
                  "list     list all running processes\n"
                  "lsdir    list of files and subdirs in a directory\n"
                  "resume   resume a suspended process\n"
                  "set      set environment variable\n"
                  "stop     suspend a process\n"
                  "time     get current time\n"
                  "unset    unset environment variable\n");
    return RUN_OK;
}