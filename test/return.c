#include <stdio.h>	
#include <stdlib.h>	

int main(int argc, char** argv) {	
    if (argc != 2) {	
        printf("Usage: return <exit_code>");	
    }	
    char* end = NULL;	
    const int base = 10;	
    unsigned int n = strtol(argv[1], &end, base);	
    return (int)n;	
}