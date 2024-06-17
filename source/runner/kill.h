#pragma once

#include "../core/args.h"
#include "res.h"

/*! @brief Kill a process */
enum run_result run_kill(const os_char *proc_id);