#pragma once

#include "../runner/res.h"
#include "cmd.h"

void io_set_last_status(enum run_result result);
struct cmd *scan_input();
void format_output(char *fmt, ...);
void format_error(char *fmt, ...);