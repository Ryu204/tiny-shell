#pragma once

#include "../core/args.h"
#include "../os/type.h"
#include "res.h"

/*! @brief Run the executable foreground */
enum run_result run_fore(const os_char *command_line);