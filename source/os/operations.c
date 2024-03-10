#include "operations.h"

#ifdef _WIN32
#    include <WinBase.h>

void get_cwd(unsigned int buffer_size, os_char *buffer) {
    GetCurrentDirectory((DWORD)buffer_size, (LPSTR)buffer);
}

#endif