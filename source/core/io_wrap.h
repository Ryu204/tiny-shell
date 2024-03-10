#pragma once

#include "../runner/res.h"
#include "cmd.h"

/*! @brief Set the result of last command (RUN_OK/RUN_FAIL/...) */
void io_set_last_status(enum run_result result);

/*! @brief Ask and convert user's input into command */
struct cmd *scan_input();

/*! @brief Used these instead of `printf` and such for uniform output */
void format_output(char *fmt, ...);
void format_error(char *fmt, ...);