#pragma once

#ifdef _WIN32
#    include <Windows.h>
#    define os_char TCHAR
#elif defined(__linux__)
#    define os_char char
#endif