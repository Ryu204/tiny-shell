#include "args.h"
#include "../core/io_wrap.h"

#include <stdlib.h>
#include <string.h>

enum {
    MAGIC_TOKEN = '~'
};

/**! @brief Transform whitespaces inside quotes into `MAGIC_TOKEN` */
os_char *transform_quotes(const os_char *str) {
    unsigned int length = 0;
    os_char *res = malloc(strlen(str) + 1);
    bool quote_opened = false;
    for(int i = 0; str[i] != '\0'; ++i) {
        if(str[i] == '\'') {
            quote_opened = !quote_opened;
        } else if(quote_opened) {
            if(is_whitespace(str[i]))
                res[length] = MAGIC_TOKEN;
            else
                res[length] = str[i];
            length++;
        } else {
            res[length] = str[i];
            length++;
        }
    }

    if(quote_opened) {
        format_error("Number of \' character is odd\n");
        free(res);
        return NULL;
    }

    res[length] = '\0';
    os_char *tmp = realloc(res, length + 1);
    if(!tmp) {
        format_error("Cannot allocate memory for input args\n");
        free(res);
        return NULL;
    } else {
        return tmp;
    }
}

struct args *split_by_whitespaces(const os_char *str) {
    static const unsigned int MAX_ARGC = 128;

    os_char **argv = malloc(MAX_ARGC * sizeof(os_char *));
    unsigned int argc = 0;

    bool is_space = true;
    bool was_space = true;
    int start = 0;

    for(int i = 0; true; ++i) {
        is_space = is_whitespace(str[i]) || str[i] == '\0';
        if(is_space && !was_space) {
            if(argc >= MAX_ARGC) {
                format_error("Too many arguments, max number is %u\n", MAX_ARGC);
            }
            const unsigned int len = (i - 1) - start + 1;
            argv[argc] = malloc(len + 1);
            memcpy(argv[argc], str + start, len);
            argv[argc][len] = '\0';
            argc++;
        }
        if(!is_space && was_space) {
            start = i;
        }
        was_space = is_space;
        if(str[i] == '\0') {
            break;
        }
    }

    struct args *res = malloc(sizeof(struct args));
    res->argc = argc;
    res->argv = argv;
    return res;
}

/**! @brief Transform `MAGIC_TOKEN` inside `arg` into space */
void re_transform_arg(os_char *arg) {
    const unsigned int len = strlen(arg);
    for(int i = 0; i < len; ++i) {
        if(arg[i] == MAGIC_TOKEN) {
            arg[i] = ' ';
        }
    }
}

struct args *args_from_str(const os_char *input) {
    if(!input) {
        return NULL;
    }
    os_char *quote_transformed = transform_quotes(input);
    if(!quote_transformed) {
        return NULL;
    }
    struct args *splitted = split_by_whitespaces(quote_transformed);
    free(quote_transformed);
    for(int i = 0; i < splitted->argc; ++i) {
        re_transform_arg(splitted->argv[i]);
    }
    return splitted;
}

void args_del(struct args *obj) {
    if(!obj) {
        return;
    }
    for(int i = 0; i < obj->argc; ++i) {
        free(obj->argv[i]);
    }
    free(obj->argv);
    free(obj);
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}