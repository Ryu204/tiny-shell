#include "add_path.h"
#include "../os/operations.h"
#include "../core/io_wrap.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifdef _WIN32
#define SEPARATOR ";"

os_char *formatted_path(const os_char *path) {
    os_char *f_path = (os_char *)malloc((strlen(path) + 2) * sizeof(os_char));
    strcpy(f_path, path);
    strncat(f_path, SEPARATOR, 2);
    return f_path;
}

#elif defined(__linux__)
#define SEPARATOR ":"

os_char *formatted_path(const os_char *path) {
    os_char *f_path = (os_char *)malloc((strlen(path) + 2) * sizeof(os_char));
    strcpy(f_path, SEPARATOR);
    strncat(f_path, path, strlen(path));
    return f_path;
}

#endif

bool in_path_env(const os_char *path_env, const os_char *new_path) {
    size_t len = strlen(path_env);
    os_char tmp_path_env[len + 1];
    strcpy(tmp_path_env, path_env);
    tmp_path_env[len] = '\0';

    char *rest = NULL;
    char *token = strtok_r(tmp_path_env, SEPARATOR, &rest);
    while (token != NULL) {
        if (!strcmp(token, new_path)) {
            return true;
        }
        token = strtok_r(NULL, SEPARATOR, &rest);
    }
    return false;
}

bool add_path(const os_char *new_path) {
    char buffer[SHRT_MAX];
    unsigned int bufferSize = sizeof(buffer) / sizeof(char);
    
    if (!get_shell_env("PATH", bufferSize, buffer)) {
        format_error("Fail to get PATH environment variable!\n");
        return false;
    }

    if (in_path_env(buffer, new_path)) {
        format_output("The path %s is already in the PATH environment variable.\n", new_path);
        return true;
    }

    os_char *f_path = formatted_path(new_path);
    strncat(buffer, f_path, strlen(f_path));

    if (!set_shell_env("PATH", buffer)) {
        format_error("Failed to set the PATH environment variable!\n");
        free(f_path);
        return false;
    }
    format_output("PATH environment variable is updated successfully.\n");
    free(f_path);
    return true;
}

enum run_result run_add_path(const os_char *new_path) {
    return add_path(new_path) ? RUN_OK : RUN_FAILED;
}