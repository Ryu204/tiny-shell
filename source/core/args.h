#pragma once

#include <stdbool.h>

#include "../os/type.h"

/**
 * @brief Result after parsing an arbitrary string
 */
struct args {
    os_char **argv;
    unsigned int argc;
    bool background;
};

/**
 * @brief Build `args` from an input string
 * @return `true` if initialization succeeded, `false` otherwise
 */
bool args_init_from_str(struct args *obj, const os_char *input);
void args_destroy(struct args *obj);

/*! @brief Deep copy the object, including arguments string */
struct args *args_deep_copy(const struct args *obj);

/*! @brief Initialize the object by deep copying `source` */
void args_deep_copy_init(struct args *obj, const struct args *source);

/**! @brief Helper function */
bool is_whitespace(os_char c);