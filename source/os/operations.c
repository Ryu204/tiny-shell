#include "operations.h"
#include "../core/io_wrap.h"

#include <stdio.h>

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

bool is_empty_str(os_char *str){
    return !strcmp(str, "");
}

void extract_from_args(const struct args args, os_char **p_command_line, bool *p_wait) {
    bool wait = true;
    bool flag = false;
    size_t len = 0;
    if(!is_empty_str(args.argv[0])) {
        len += strlen(args.argv[0]) + 2;
    }
    for(int i = 1; i < args.argc; ++i) {
        if(is_empty_str(args.argv[i])) {
            continue;
        }
        if(strcmp(args.argv[i], "&") != 0 || flag) {
            len += strlen(args.argv[i]) + 3;
        }
        else
        if(!flag) {
            wait = false;
            flag = true;
        }
    }

    os_char *command_line = malloc((len + 1) * sizeof(os_char));
    flag = false;
    len = 0;
    if(!is_empty_str(args.argv[0])) {
        sprintf(command_line, "\"%s\"", args.argv[0]);
        len += strlen(args.argv[0]) + 2;
    }
    for(int i = 1; i < args.argc; ++i) {
        if(is_empty_str(args.argv[i])) {
            continue;
        }
        if(strcmp(args.argv[i], "&") != 0 || flag) {
            sprintf(command_line + len, " \"%s\"", args.argv[i]);
            len += strlen(args.argv[i]) + 3;
        }
        else
        if(!flag) {
            flag = true;
        }
    }

    command_line[len] = '\0';

    *p_command_line = command_line;
    *p_wait = wait;
}

bool launch_executable(const struct args args) {
    os_char *command_line = NULL;
    bool wait = true;

    extract_from_args(args, &command_line, &wait);

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
    if(wait) {
        WaitForSingleObject(pi.hProcess, INFINITE);
    }

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
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