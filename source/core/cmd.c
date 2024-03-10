#include "cmd.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void to_lower(char *str);
bool is_whitespace(char c);
char *trim_whitespaces(const char *str);

struct cmd *cmd_from_str(const char *str) {
    struct cmd *res = malloc(sizeof(struct cmd));
    res->type = CMD_UNKNOWN;

    char *trimmed = trim_whitespaces(str);
    const int trimmed_len = (int)strlen(trimmed);

    if(trimmed == NULL || trimmed_len == 0) {
        free(trimmed);
        return res;
    }

    char *name = NULL;
    int name_len = 0;
    for(int i = 1; true; ++i) {
        if(is_whitespace(trimmed[i])) {
            name = malloc(i + 1);
            memcpy(name, trimmed, i);
            name[i] = '\0';
            name_len = i;
            break;
        }
    }
    assert(name != NULL && name_len > 0);

    if(strcmp(name, "help") == 0) {
        res->type = CMD_HELP;
    } else if(strcmp(name, "exit") == 0) {
        res->type = CMD_EXIT;
    } else if(strcmp(name, "cd") == 0) {
        res->type = CMD_CHANGE_DIR;
        res->val.new_dir = trim_whitespaces(trimmed + name_len);
        if(res->val.new_dir == NULL || strlen(res->val.new_dir) == 0) {
            res->type = CMD_INVALID_SYNTAX;
            free(res->val.new_dir);
        }
    }

    free(name);
    free(trimmed);
    return res;
}

void to_lower(char *str) {
    for(int i = 0; str[i] != '\0'; ++i) {
        if(isalpha(str[i]))
            str[i] = (char)tolower(str[i]);
    }
}

void cmd_del(struct cmd *obj) {
    switch(obj->type) {
    case CMD_CHANGE_DIR:
        free(obj->val.new_dir);
        break;
    default:
        break;
    }
    free(obj);
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

char *trim_whitespaces(const char *str) {
    const int n = (int)strlen(str);
    int first = -1;
    int last = -1;

    for(int i = 0; i < n; ++i) {
        if(!is_whitespace(str[i])) {
            first = i;
            break;
        }
    }
    for(int i = n - 1; i >= 0; --i) {
        if(!is_whitespace(str[i])) {
            last = i;
            break;
        }
    }
    if(first < 0 || last < 0 || first > last)
        return NULL;
    const unsigned int length = last - first + 1;
    char *res = malloc(length + 1);
    memcpy(res, str + first, length);
    res[length] = '\0';
    return res;
}