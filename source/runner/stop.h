#pragma once

#include "../core/args.h"
#include "res.h"

/*! @brief Resume running a process */
enum run_result run_stop(const os_char *proc_id);