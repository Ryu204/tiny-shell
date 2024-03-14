#pragma once

#include "../os/type.h"
#include "res.h"

/*! @brief Set current process's environment variable */
enum run_result run_set_env(const os_char *name, const os_char *val);

/*! @brief Unset current process's environment variable */
enum run_result run_unset_env(const os_char *name);

/*! @brief Set current process's environment variable */
enum run_result run_get_all_env();

/*! @brief Set current process's environment variable */
enum run_result run_get_env(const os_char *name);
