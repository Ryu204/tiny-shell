#pragma once

#include <stdbool.h>

#include "../os/type.h"

/**
 * @brief Result after parsing an arbitrary string
 */
struct args {
    os_char **argv;
    unsigned int argc;
};

/**
 * @brief Build `args` from an input string
 * @note Caller is responsible to free the result
 * @return `NULL` if `input` is empty, real data otherwise
 */
void args_init_from_str(struct args *obj, const os_char *input);
void args_destroy(struct args *obj);

/**! @brief Helper function */
bool is_whitespace(os_char c);