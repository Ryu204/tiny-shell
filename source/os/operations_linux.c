#ifdef __linux__
#    include "../core/config.h"
#    include "../core/io_wrap.h"
#    include "operations.h"
#    include "type.h"

#    include <assert.h>
#    include <dirent.h>
#    include <errno.h>
#    include <libgen.h>
#    include <stdio.h>
#    include <stdlib.h>
#    include <string.h>
#    include <sys/stat.h>
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
    struct args copied_args;
    args_deep_copy_init(&copied_args, &args);
    int pid = fork();
    int stat_loc = 0;
    if(pid == -1) {
        report_error_code(errno);
        args_destroy(&copied_args);
        return false;
    }
    if(pid == 0) {
        if(execvp(copied_args.argv[0], copied_args.argv) == -1) {
            report_error_code(errno);
            _exit(EXIT_FAILURE);
        }
        _exit(EXIT_SUCCESS);
    } else {
        if(!args.background) {
            waitpid(pid, &stat_loc, 0);
        } else {
            signal(SIGCHLD, handle_background_child_exit);
            return true;
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
        args_destroy(&copied_args);
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

bool is_number(const char *c) {
    if(c == NULL || c[0] == '\0')
        return false;
    const unsigned int length = strlen(c);
    for(int i = 0; i < length; ++i) {
        if(c[i] < '0' || c[i] > '9')
            return false;
    }
    return true;
}

void enumProc() {
    DIR *procdir = NULL;
    struct dirent *entry = NULL;
    char path[PATH_MAX];

    // Open the /proc directory
    procdir = opendir("/proc");
    if(procdir == NULL) {
        format_error("Cannot open /proc");
        report_error_code(errno);
        return;
    }
    // Loop through entries in /proc
    // NOLINTNEXTLINE
    while((entry = readdir(procdir)) != NULL) {
        // Check if entry is a numeric directory (potential process ID)
        if(!is_number(entry->d_name))
            continue;
        // Construct path to process status file
        snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);
        // Open the status file
        FILE *statusfile = fopen(path, "r");
        if(statusfile == NULL) {
            continue;
        }
// Read the first line for process name
#    define NAME_LENGTH 512
        char name[NAME_LENGTH];
        if(fscanf(statusfile, "Name: %s512\n", name) != 1) {
            fclose(statusfile);
            continue;
        }
        // Get PID using entry->d_name (assuming it's numeric)
        pid_t pid = atoi(entry->d_name);
        // Get parent process ID
        char line[NAME_LENGTH];
        pid_t ppid = -1;
        // NOLINTBEGIN
        while(fgets(line, sizeof(line), statusfile) != NULL) {
            // Find the line starting with "PPid:"
            if(strncmp(line, "PPid:", 5) == 0) {
                sscanf(line + 5, "%d", &ppid); // Extract PPID from the line
                break;
            }
        }
        // NOLINTEND
        // Get thread count
        DIR *t_procdir = NULL;
        char thread_count[3];
        snprintf(path, sizeof path, "/proc/%d/task", pid);
        t_procdir = opendir(path);

        if(t_procdir == NULL) {
            thread_count[0] = '?';
            thread_count[1] = '\0';
        } else {
            unsigned int t_count = 0;
            struct dirent *entry = NULL;
            // NOLINTNEXTLINE
            while((entry = readdir(t_procdir)) != NULL) {
                if(!is_number(entry->d_name))
                    continue;
                t_count++;
            }
            snprintf(thread_count, sizeof thread_count, "%d", t_count);
            closedir(t_procdir);
        }
        // Print process information
        printf("PID: %6u PPID: %6u T %3s Name: %s \n", pid, ppid, thread_count, name);
        fclose(statusfile);
    }
    closedir(procdir);
}

bool minibat(const struct args args) {
    enum { buffer_size = 300 };
    os_char *buffer = malloc(buffer_size * sizeof(os_char));
    if(readlink("/proc/self/exe", buffer, buffer_size) == -1) {
        report_error_code(errno);
        free(buffer);
        return false;
    }
    // Append `"${buffer} -f "` and launch the executable
    enum { count = 2 };
    struct args *mod = malloc(sizeof(struct args));
    mod->argc = count + args.argc;
    mod->background = args.background;
    mod->argv = malloc((mod->argc + 1) * sizeof(os_char *));

    mod->argv[0] = buffer;

    mod->argv[1] = malloc(3 * sizeof(os_char));
    memcpy(mod->argv[1], "-f", 2);
    mod->argv[1][2] = '\0';

    for(int i = count; i < mod->argc; ++i) {
        const unsigned int len = strlen(args.argv[i - count]);
        mod->argv[i] = malloc((len + 1) * sizeof(os_char));
        memcpy(mod->argv[i], args.argv[i - count], len);
        mod->argv[i][len] = '\0';
    }
    mod->argv[mod->argc] = NULL;
    const bool res = launch_executable(*mod);
    args_destroy(mod);
    free(mod);
    return res;
};

#endif