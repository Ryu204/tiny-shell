#include <windows.h>
#include <stdio.h>
#include <tchar.h> 
#include <strsafe.h>
#pragma comment(lib, "User32.lib")


int main() {
    WIN32_FIND_DATA data;
    LARGE_INTEGER fileSize;
    char path[] = "./build/*";

    HANDLE hFind = FindFirstFile(path, &data);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                _tprintf(TEXT("%s\n"), data.cFileName);
            } else {
                _tprintf(TEXT("%s\n"), data.cFileName);
            }
        }
        while (FindNextFile(hFind, &data) != 0);
    }
}