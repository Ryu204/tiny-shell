#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

int run_executable(char *commandLine, HANDLE *hProcess) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if(!CreateProcess(
        NULL,
        commandLine,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )){
        printf("CreatProcess failed (%d).\n", GetLastError);
        return 1;
    }

    *hProcess = pi.hProcess;

    //CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}

int main() {
    HANDLE hProcess;
    run_executable("counter.exe 5", &hProcess);
    while (true) {
        DWORD exitCode;
        if (GetExitCodeProcess(hProcess, &exitCode)) {
            if (exitCode == STILL_ACTIVE) {
            } else {
                printf("Process has completed with exit code %lu.\n", exitCode);
                break;
            }
        } else {
            printf("Error getting process exit code.\n");
        }
    }
    return 0;
}