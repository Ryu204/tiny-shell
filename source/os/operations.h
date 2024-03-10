#pragma once

#include <stdbool.h>

#include "type.h"

void get_cwd(unsigned int buffer_size, os_char *buffer);

bool change_cwd(const os_char *new_dir);