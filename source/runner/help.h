#pragma once

#include "../core/io.h"
#include "res.h"

enum run_result inline run_help() {
    format_output("%s",
            "\"help\": print this message\n"
            "\"exit\": exit shell\n");
    return RUN_OK;
}