#include "operations.h"
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
    default:
        format_error("System error code: %d\n", error);
    }
}

#endif