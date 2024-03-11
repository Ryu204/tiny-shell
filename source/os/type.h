#pragma once

#ifdef _WIN32
#    include <windows.h>
#    define os_char TCHAR
#elif defined(__linux__)
#    define os_char char
#endif