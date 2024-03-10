#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#define SHELL_BUFSIZE 1024
#define SHELL_BUFEXTRA 512
#define SHELL_DELIM " "

int shell_help(int argc, char **argv);
int shell_exit(int argc, char **argv);
int shell_date(int argc, char **argv);

char *builtin_str[] = {
    "help",
    "exit",
    "date"
};

char *builtin_help[] = {
    "print all commands.",
    "exit shell.",
    "print current date"
};

int (*builtin_func[]) (int argc, char **argv) = {
    &shell_help,
    &shell_exit,
    &shell_date
};

int shell_cnt_builtin_func() {
    return sizeof(builtin_func) / sizeof(void *);
}

int shell_help(int argc, char **argv) {
    if (argc > 1) {
        printf("help: unkown option");
        fflush(stdout);
        return 0;
    }
    printf("List of all commands.\n");
    for (int i = 0; i < shell_cnt_builtin_func(); ++i) {
        printf("%s: %s\n", builtin_str[i], builtin_help[i]);
    }
    fflush(stdout);
    return 0;
}

int shell_exit(int argc, char **argv) {
    if (argc > 1) {
        printf("help: unkown option");
        fflush(stdout);
        return 0;
    }
    printf("shell: Good bye!");
    fflush(stdout);
    exit(0);
}

char *day_of_week[] = {
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat",
};

char *month[] = {
    "",
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec",
};

int shell_date(int argc, char **argv) {
    if (argc > 1) {
        printf("date: unkown option");
        fflush(stdout);
        return 0;
    }
    SYSTEMTIME lt;

    GetLocalTime(&lt);

    printf("%s, %s %02d, %04d %02d:%02d:%02d\n", 
        day_of_week[lt.wDayOfWeek], 
        month[lt.wMonth],
        lt.wDay,
        lt.wYear,
        lt.wHour,
        lt.wMinute,
        lt.wSecond
    );
    fflush(stdout);
    return 0;
}

int shell_execute(int argc, char **argv) {
    if (!argc) {
        return 0;
    }
    else{
        for (int i = 0; i < shell_cnt_builtin_func(); ++i) {
            if (!strcmp(argv[0], builtin_str[i])) {
                return (*builtin_func[i])(argc, argv);
            }
        }
    }
    return 0;
}

char **shell_split_line(char *line, int *argc) {
    int bufsize = SHELL_BUFSIZE;
    int pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        printf("shell: allocation error\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SHELL_DELIM);
    while (token != NULL) {
        tokens[pos] = token;
        ++pos;

        if (pos >= bufsize) {
            bufsize += SHELL_BUFEXTRA;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                printf("shell: allocation error\n");
                fflush(stdout);
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SHELL_DELIM);
    }
    
    tokens[pos] = NULL;
    *argc = pos;
    return tokens;
}

char *shell_read_line() {
    int bufsize = SHELL_BUFSIZE;
    int pos = 0;
    char *buffer = malloc(bufsize * sizeof(char));
    int c;

    if (!buffer) {
        printf("shell: allocation error\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    while (true) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[pos] = '\0';
            return buffer;
        }
        else {
            buffer[pos] = c;
        }
        ++pos;

        if (pos >= bufsize) {
            bufsize += SHELL_BUFEXTRA;
            buffer = realloc(buffer, bufsize * sizeof(char));
            if (!buffer) {
                printf("shell: allocation error\n");
                fflush(stdout);
                exit(EXIT_FAILURE);
            }
        }
    }
}

void shell_loop() {
    char *line;
    int argc;
    char **argv;
    int status;

    do {
        printf("bcdt_shell ~/\n$ ");
        fflush(stdout);

        line = shell_read_line();
        argv = shell_split_line(line, &argc);
        status = shell_execute(argc, argv);

        free(line);
        free(argv);
    } while (!status);
}

int main() {
    system("clear");
    
    shell_loop();
    
    return 0;
}