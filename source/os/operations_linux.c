#ifdef __linux__
#    include "../core/config.h"
#    include "../core/io_wrap.h"
#    include "operations.h"
#    include "type.h"

#    include <assert.h>
#    include <errno.h>
#    include <stdio.h>
#    include <stdlib.h>
#    include <string.h>
#    include <sys/wait.h>
#    include <unistd.h>

extern char **environ; // NOLINT

void report_error_code(int code) {
    switch(code) {
    case 0:
        break;
    case EACCES:
        format_error("Access denied!\n");
        break;
    case EBADF:
        format_error("Not a valid filename\n");
        break;
    case ENOTDIR:
        format_error("Not a directory\n");
        break;
    case ENOENT:
        format_error("No such file or directory\n");
        break;
    default:
        format_error("System error code: %d\n", code);
    }
}

void get_cwd(unsigned int buffer_size, os_char *buffer) {
    if(!getcwd(buffer, buffer_size)) {
        format_error("Cannot get current working directory\n");
    }
}

bool change_cwd(const os_char *new_dir) {
    int error_code = chdir(new_dir);
    if(error_code == 0) {
        return true;
    }
    report_error_code(errno);
    return false;
}

void clear_screen() {
    printf("\e[1;1H\e[2J");
}

// NOLINTBEGIN(misc-unused-parameters)
void handle_background_child_exit(int sig) {
    int stat = 0;
    waitpid(-1, &stat, WNOHANG);
}
// NOLINTEND(misc-unused-parameters)

bool launch_executable(const struct args args) {
    assert(args.argc > 0 && "Invalid args");
    int pid = fork();
    int stat_loc = 0;
    if(pid == -1) {
        report_error_code(errno);
        return false;
    }
    if(pid == 0) {
        struct args *copied_args = args_deep_copy(&args);
        assert(copied_args != NULL);
        static char *cmds[] = {"e", "a", NULL};
        if(execvp(copied_args->argv[0], copied_args->argv) == -1) {
            report_error_code(errno);
            _exit(EXIT_FAILURE);
        }
        _exit(EXIT_SUCCESS);
    } else {
        if(!args.background) {
            waitpid(pid, &stat_loc, 0);
        } else {
            signal(SIGCHLD, handle_background_child_exit);
        }
        if(WIFEXITED(stat_loc)) {
            const int exit_code = WEXITSTATUS(stat_loc);
            if(exit_code != 0) {
                format_output("Exit code: %d\n", exit_code);
                return false;
            }
            return true;
        } else if(WIFSTOPPED(stat_loc)) {
            format_error("Stopped\n");
            return false;
        } else if(WCOREDUMP(stat_loc)) {
            format_error("Core dumped\n");
            return false;
        } else if(WIFSIGNALED(stat_loc)) {
            if(WTERMSIG(stat_loc) == SIGSEGV) {
                format_error("Segmentation fault\n");
                return false;
            } else {
                format_error("Terminated by signal\n");
                return false;
            }
        } else {
            format_error("Unknown error\n");
            return false;
        }
    }
}

bool set_shell_env(const os_char *name, const os_char *val) {
    const int res = setenv(name, val == NULL ? "" : val, true); // NOLINT
    if(res == 0) {
        return true;
    }
    report_error_code(errno);
    return false;
}

bool unset_shell_env(const os_char *name) {
    const int res = unsetenv(name); // NOLINT
    if(res == 0) {
        return true;
    }
    report_error_code(errno);
    return false;
}

bool get_shell_env(const os_char *var, unsigned int buffer_size, os_char *buffer) {
    const os_char *res = getenv(var); // NOLINT
    if(!res) {
        format_error("Environment variable does not exist\n");
        return false;
    }
    const unsigned int len = strlen(res);
    if(len >= buffer_size) {
        format_error("Insufficient buffer size\n");
        return false;
    }
    memcpy(buffer, res, len);
    buffer[len] = '\0';
    return true;
}

os_char *get_all_shell_env_display() {
    os_char *res = malloc(ENVS_RESERVE_SIZE);
    unsigned int len = 0;
    unsigned int reserve_len = ENVS_RESERVE_SIZE;

    for(int i = 0; *(environ + i) != NULL; ++i) {
        const os_char *var = *(environ + i);
        const unsigned int var_len = strlen(var);
        const unsigned int new_len = len + 1 + var_len; // 1 for '\n'
        while(new_len >= reserve_len) {
            reserve_len += ENVS_RESERVE_SIZE;
            os_char *new_res = realloc(res, reserve_len);
            if(!new_res) {
                free(res);
                format_error("Cannot allocate enough memory to copy environment variables\n");
                return NULL;
            }
            res = new_res;
        }
        memcpy(res + len, var, var_len);
        len = new_len;
        res[new_len - 1] = '\n';
    }
    res[len] = '\0';
    return res;
}

#endif