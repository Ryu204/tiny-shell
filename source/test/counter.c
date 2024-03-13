// #include <stdio.h>

// #include "../core/io_wrap.h"
// #include "../os/type.h"

// const int SPAN = 1000;
// const int BASE = 10;

// int main(int argc, char *argv[]) {
//     if(argc != 2) {
//         printf("Usage: counter <number>\n");
//         return 1;
//     }

//     int n = 0;
//     char *endptr = NULL;

//     n = strtol(argv[1], &endptr, BASE);

//     if(*endptr != '\0') {
//         printf("Invalid argument: %s\n", argv[1]);
//         return 1;
//     }

//     for(int i = 0; i <= n; ++i) {
//         printf("Second %d.\n", i);
//         Sleep(SPAN);
//     }
//     return 0;
// }