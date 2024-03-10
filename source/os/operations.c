#include "operations.h"
#include "../core/io_wrap.h"

#ifdef _WIN32
#    include <WinBase.h>
#include <conio.h>

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
    clrscr();
}

#elif defined(__linux__)
#    include <errno.h>
#    include <unistd.h>
#include <stdio.h>

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