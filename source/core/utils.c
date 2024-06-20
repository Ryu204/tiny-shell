#include "utils.h"
#include <stddef.h>
#include <string.h>

bool is_number(const char *c) {
    if(c == NULL || c[0] == '\0')
        return false;
    const unsigned int length = strlen(c);
    static const unsigned int MAX_INT_DIGITS = 10;
    if(length > MAX_INT_DIGITS)
        return false;
    for(int i = 0; i < length; ++i) {
        if(c[i] < '0' || c[i] > '9')
            return false;
    }
    return true;
}
