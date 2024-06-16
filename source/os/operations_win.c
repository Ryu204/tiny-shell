#ifdef _WIN32

#    include "../core/config.h"
#    include "../core/io_wrap.h"
#    include "operations.h"

#    include <WinBase.h>
#    include <signal.h>
#    include <stdio.h>
#    include <tlhelp32.h>

void report_error_code(DWORD err);

void get_cwd(unsigned int buffer_size, os_char *buffer) {
    DWORD status = GetCurrentDirectory((DWORD)buffer_size, (LPSTR)buffer);
    if(status == 0 || status > buffer_size) {
        report_error_code(GetLastError());
    }
}

bool change_cwd(const os_char *new_dir) {
    if(!SetCurrentDirectory(new_dir)) {
        report_error_code(GetLastError());
        return false;
    }
    return true;
}

void report_error_code(DWORD error) {
    switch(error) {
    case ERROR_FILE_NOT_FOUND:
        format_error("File or directory not found\n");
        break;
    case ERROR_INVALID_NAME:
        format_error("Filename, directory name, or volume label syntax is incorrect\n");
        break;
    case ERROR_DIRECTORY:
        format_error("Directory name is invalid\n");
        break;
    case ERROR_PATH_NOT_FOUND:
        format_error("Cannot find the path specified\n");
        break;
    case ERROR_ENVVAR_NOT_FOUND:
        format_error("Environment variable not found\n");
        break;
    default:
        format_error("System error code: %d\n", error);
    }
}

void clear_screen() {
    // From: https://learn.microsoft.com/en-us/windows/console/clearing-the-screen#example-2
    // NOLINTBEGIN
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    // Get the number of character cells in the current buffer.
    if(!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
    // NOLINTEND
}

bool is_empty_str(os_char *str) {
    return !strcmp(str, "");
}

void extract_from_args(const struct args args, os_char **p_command_line) {
    size_t len = 0;
    if(!is_empty_str(args.argv[0])) {
        len += strlen(args.argv[0]) + 2;
    }
    for(int i = 1; i < args.argc; ++i) {
        if(is_empty_str(args.argv[i])) {
            continue;
        } else {
            len += strlen(args.argv[i]) + 3;
        }
    }

    os_char *command_line = malloc((len + 1) * sizeof(os_char));
    len = 0;
    if(!is_empty_str(args.argv[0])) {
        sprintf(command_line, "\"%s\"", args.argv[0]);
        len += strlen(args.argv[0]) + 2;
    }
    for(int i = 1; i < args.argc; ++i) {
        if(is_empty_str(args.argv[i])) {
            continue;
        } else {
            sprintf(command_line + len, " \"%s\"", args.argv[i]);
            len += strlen(args.argv[i]) + 3;
        }
    }

    command_line[len] = '\0';

    *p_command_line = command_line;
}

bool launch_executable(const struct args args) {
    if(!args.background) {
        signal(SIGINT, SIG_IGN);
    }

    os_char *command_line = NULL;

    extract_from_args(args, &command_line);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    const unsigned int len = strlen(command_line);
    os_char *tmp_command_line = malloc((len + 1) * sizeof(os_char));
    memcpy(tmp_command_line, command_line, len * sizeof(os_char));
    tmp_command_line[len] = '\0';

    free(command_line);

    if(!CreateProcess(
           NULL,             // No module name (use command line)
           tmp_command_line, // Command line
           NULL,             // Process handle not inheritable
           NULL,             // Thread handle not inheritable
           FALSE,            // Set handle inheritance to FALSE
           0,                // No creation flags
           NULL,             // Use parent's environment block
           NULL,             // Use parent's starting directory
           &si,              // Pointer to STARTUPINFO structure
           &pi               // Pointer to PROCESS_INFORMATION structure
           )) {
        free(tmp_command_line);
        report_error_code(GetLastError());
        return false;
    }

    free(tmp_command_line);

    // Wait until child process exits.
    if(!args.background) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exit_code = 0;
#    define exit_cleanup(res) \
        { \
            CloseHandle(pi.hProcess); \
            CloseHandle(pi.hThread); \
            return res; \
        }
        if(GetExitCodeProcess(pi.hProcess, &exit_code) == 0) {
            report_error_code(GetLastError());
            exit_cleanup(false);
        } else {
            if(exit_code == 0) {
                exit_cleanup(true);
            } else {
                format_error("Exit code: %ld\n", exit_code);
                exit_cleanup(false);
            }
        }
    }

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    exit_cleanup(true);
#    undef exit_cleanup
}

bool set_shell_env(const os_char *name, const os_char *val) {
    if(SetEnvironmentVariable(name, val == NULL ? "" : val)) {
        return true;
    }
    report_error_code(GetLastError());
    return false;
}

bool unset_shell_env(const os_char *name) {
    if(SetEnvironmentVariableA(name, NULL)) {
        return true;
    }
    report_error_code(GetLastError());
    return false;
}

bool get_shell_env(const os_char *var, unsigned int buffer_size, os_char *buffer) {
    DWORD res = GetEnvironmentVariable(var, buffer, buffer_size);
    DWORD last_err = GetLastError();
    if(res == 0 && last_err != ERROR_SUCCESS) {
        report_error_code(last_err);
        return false;
    } else if(res >= buffer_size) {
        format_error("Insufficient buffer size\n");
        return false;
    }
    return true;
}

os_char *get_all_shell_env_display() {
    os_char *res = malloc(ENVS_RESERVE_SIZE);
    unsigned int res_len = 0;
    unsigned int reserve_len = ENVS_RESERVE_SIZE;
    os_char *ptr = GetEnvironmentStrings();
    for(os_char *i = ptr; *i != '\0';) {
        const unsigned int len = strlen(i);
        const unsigned int res_new_length = res_len + len + 1;
        // Allocate more memory if needed
        while(res_new_length >= reserve_len) {
            reserve_len += ENVS_RESERVE_SIZE;
            os_char *tmp = realloc(res, reserve_len);
            if(!tmp) {
                format_error("Cannot allocate enough memory\n");
                free(res);
                FreeEnvironmentStrings(ptr);
                return NULL;
            }
            res = tmp;
        }
        memcpy(res + res_len, i, len);
        res_len = res_new_length;
        *(res + res_len - 1) = '\n';
        i += len + 1;
    }
    FreeEnvironmentStrings(ptr);

    *(res + res_len) = '\0';
    return res;
}

bool minibat(const struct args args) {
    const os_char *file = args.argv[0];

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE hFIle = CreateFile(
        file,
        GENERIC_READ,
        FILE_SHARE_READ,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if(hFIle == INVALID_HANDLE_VALUE) {
        format_error("Cannot open file %s, exit code: %d!\n", file, GetLastError());
        return false;
    }

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    BOOL ret = 0;

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));

    si.cb = sizeof(STARTUPINFO);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = hFIle;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    ret = CreateProcess(
        NULL,
        "tiny-shell.exe -s",
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi);

    if(!ret) {
        format_error("Cannot create process %s, exit code: %d!\n", file, GetLastError());
        CloseHandle(hFIle);
        return false;
    }

    if(!args.background) {
        WaitForSingleObject(pi.hProcess, INFINITE);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    CloseHandle(hFIle);

    return true;
}

bool enum_proc() {
    // NOLINTBEGIN
    HANDLE hSnapshot = INVALID_HANDLE_VALUE;
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnapshot, &pe);
    do {
        printf("PID: %6u PPID: %6u T: %3u Name: %s \n", pe.th32ProcessID, pe.th32ParentProcessID, pe.cntThreads, pe.szExeFile);
    } while(Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    // NOLINTEND
    return true;
}

#endif