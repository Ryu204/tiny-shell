#pragma once

#include <stdbool.h>

#include "../runner/res.h"
#include "cmd.h"

/*! @brief Set the result of last command (RUN_OK/RUN_FAIL/...) */
void io_set_last_status(enum run_result result);

/*! @brief Config if prompt should be printed */
void io_set_prompt_visibility(bool visible);

/*! @brief Ask and convert user's input into command */
struct cmd *scan_input();

/*! @brief Used `format_xxx` instead of `printf` and such for uniform output */
void format_output(char *fmt, ...);

/*! @brief Used `format_xxx` instead of `printf` and such for uniform output */
void format_error(char *fmt, ...);