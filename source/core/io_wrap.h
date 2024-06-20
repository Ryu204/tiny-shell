#pragma once

#include <stdbool.h>

#include "../runner/res.h"
#include "cmd.h"

/*! @brief Set the result of last command (RUN_OK/RUN_FAIL/...) */
void io_set_last_status(enum run_result result);

/*! @brief Set default text color to white */
void io_set_text_white();

/*! @brief Config if prompt should be printed */
void io_set_prompt_visibility(bool visible);

/*! @brief Ask and convert user's input into command */
void scan_input(struct cmd *obj);

/*! @brief Used `format_xxx` instead of `printf` and such for uniform output */
void format_success(char *fmt, ...);

/*! @brief Used `format_xxx` instead of `printf` and such for uniform output */
void format_output(char *fmt, ...);

/*! @brief Used `format_xxx` instead of `printf` and such for uniform output */
void format_usage(char *fmt, ...);

/*! @brief Used `format_xxx` instead of `printf` and such for uniform output */
void format_error(char *fmt, ...);