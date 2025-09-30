#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "executor.h"

#define MAX_INPUT 1024

void parse_command(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " \t\n");

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

void print_welcome() {
    printf("========================================\n");
    printf("   Simple Command Executor\n");
    printf("========================================\n");
    printf("Type 'exit' to quit\n\n");
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    int status;

    print_welcome();

    while (1) {
        printf("cmd> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) {
            continue;
        }

        parse_command(input, args);

        if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        if (args[0] != NULL) {
            status = execute_command(args[0], args);
            printf("\nCommand '%s' completed with exit status: %d\n\n",
                   args[0], status);
        }
    }

    return 0;
}