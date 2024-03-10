#pragma once

#include "../core/cmd.h"
#include "res.h"

/**
 * @brief Invoke corresponding functions based on `cmd`'s type and value
 * @return Result of the operation
 */
enum run_result invoke_runner(const struct cmd *cmd);