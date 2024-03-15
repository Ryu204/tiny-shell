#ifdef __linux__

#    include "../core/io_wrap.h"
#    include "operations.h"

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