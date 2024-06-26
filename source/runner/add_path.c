#include "add_path.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#    define SEPARATOR ";"

os_char *formatted_path(const os_char *path) {
    os_char *f_path = (os_char *)malloc((strlen(path) + 2) * sizeof(os_char));
    size_t len = strlen(path);
    strncpy(f_path, path, strlen(path));
    f_path[len] = '\0';
    strncat(f_path, SEPARATOR, 2);
    return f_path;
}

#elif defined(__linux__)
#    define SEPARATOR ":"

os_char *formatted_path(const os_char *path) {
    os_char *f_path = (os_char *)malloc((strlen(path) + 2) * sizeof(os_char));
    strncpy(f_path, SEPARATOR, 1);
    f_path[1] = '\0';
    strncat(f_path, path, strlen(path));
    return f_path;
}

#endif

char *strtok_r_custom(char *str, const char *delim, char **saveptr) {
    char *token = NULL;
    if(str == NULL) {
        str = *saveptr;
    }
    // Skip leading delimiters
    str += strspn(str, delim);
    if(*str == '\0') {
        return NULL;
    }
    // Find the end of the token
    token = str;
    str = strpbrk(token, delim);
    if(str == NULL) {
        // This token finishes the string
        *saveptr = strchr(token, '\0');
    } else {
        // Terminate the token and update the save pointer
        *str = '\0';
        *saveptr = str + 1;
    }
    return token;
}

bool in_path_env(const os_char *path_env, const os_char *new_path) {
    size_t len = strlen(path_env);
    os_char tmp_path_env[SHRT_MAX];
    strncpy(tmp_path_env, path_env, len);
    tmp_path_env[len] = '\0';

    char *rest = NULL;
    char *token = strtok_r_custom(tmp_path_env, SEPARATOR, &rest);
    while(token != NULL) {
        if(!strcmp(token, new_path)) {
            return true;
        }
        token = strtok_r_custom(NULL, SEPARATOR, &rest);
    }
    return false;
}

bool add_path(const os_char *new_path) {
    char buffer[SHRT_MAX];
    unsigned int bufferSize = sizeof(buffer) / sizeof(char);

    if(!get_shell_env("PATH", bufferSize, buffer)) {
        format_error("Fail to get PATH environment variable!\n");
        return false;
    }

    if(in_path_env(buffer, new_path)) {
        format_error("The path %s is already in the PATH environment variable.\n", new_path);
        return false;
    }

    os_char *f_path = formatted_path(new_path);
    strncat(buffer, f_path, strlen(f_path));

    if(!set_shell_env("PATH", buffer)) {
        format_error("Failed to set the PATH environment variable!\n");
        free(f_path);
        return false;
    }
    format_success("PATH environment variable is updated successfully.\n");
    free(f_path);
    return true;
}

enum run_result run_add_path(const os_char *new_path) {
    return add_path(new_path) ? RUN_OK : RUN_FAILED;
}