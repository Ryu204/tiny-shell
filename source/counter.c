#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: counter <count>\n");
        return 1;
    }

    int count = atoi(argv[1]);
    if (count <= 0) {
        printf("Please provide a positive integer as the argument.\n");
        return 1;
    }

    for (int i = 0; i <= count; ++i) {
        printf("%d\n", i);
        Sleep(1000); // Sleep for 1 second
    }

    return 0;
}
