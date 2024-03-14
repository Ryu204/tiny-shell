#include "env.h"
#include "../core/config.h"
#include "../core/io_wrap.h"
#include "../os/operations.h"

enum run_result run_set_env(const os_char *name, const os_char *val) {
    return set_shell_env(name, val) ? RUN_OK : RUN_FAILED;
}

enum run_result run_unset_env(const os_char *name) {
    return unset_shell_env(name) ? RUN_OK : RUN_FAILED;
}

enum run_result run_get_all_env() {
    os_char *env = get_all_shell_env_display();
    if(!env) {
        return RUN_FAILED;
    } else {
        format_output("%s\n", env);
        return RUN_OK;
    }
}

enum run_result run_get_env(const os_char *name) {
    os_char buffer[ENV_VAR_BUFFER_SIZE];
    if(!get_shell_env(name, ENV_VAR_BUFFER_SIZE, buffer)) {
        return RUN_FAILED;
    }
    format_output("%s\n", buffer);
    return RUN_OK;
}