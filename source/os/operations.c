#include "operations.h"
#include "../core/config.h"
#include "../core/io_wrap.h"

#ifdef _WIN32
#    include <WinBase.h>

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
    } else if(res > buffer_size) {
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

#elif defined(__linux__)
#    include <errno.h>
#    include <stdio.h>
#    include <unistd.h>

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
    switch(errno) {
    case 0:
        return true;
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
        format_error("System error code: %d\n", errno);
    }
    return false;
}

void clear_screen() {
    printf("\e[1;1H\e[2J");
}

#endif