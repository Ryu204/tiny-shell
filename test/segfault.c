#include <stdlib.h>

int main() {
    int a = 0;
    int b = *((int *)NULL); // NOLINT

    return 0;
}