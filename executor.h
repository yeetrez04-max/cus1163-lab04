#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// Maximum number of arguments for a command
#define MAX_ARGS 64

// Maximum length of a command
#define MAX_CMD_LENGTH 256

/**
 * Execute a command using fork-exec-wait pattern
 *
 * @param command The command to execute (e.g., "ls")
 * @param args Array of arguments including command name, ending with NULL
 *             Example: {"ls", "-l", "/home", NULL}
 * @return Exit status of the command (0 for success, non-zero for failure)
 */
int execute_command(char *command, char **args);

#endif