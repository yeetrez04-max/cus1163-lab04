#include "executor.h"

/**
 * Execute a command using the fork-exec-wait pattern
 *
 * This function demonstrates the fundamental process management pattern
 * used by all Unix shells:
 * 1. Fork a child process
 * 2. Child calls exec to transform into the target command
 * 3. Parent waits for child to complete
 *
 * @param command The command to execute (e.g., "ls", "pwd", "echo")
 * @param args Array of arguments: [command, arg1, arg2, ..., NULL]
 *             Example: {"ls", "-l", NULL} or {"echo", "Hello", NULL}
 * @return Exit status of the command (0=success, non-zero=failure, -1=error)
 */
int execute_command(char *command, char **args) {
    pid_t pid;
    int status;

    // TODO 1: Fork a child process
    // Use fork() to create a new process
    // Store the return value in 'pid'
    // Check if fork failed (pid < 0) and return -1 if so

    // TODO 2: Child process - Execute the command
    // Check if we're in the child process (pid == 0)
    // Call execvp(command, args) to transform into the target program
    // If execvp returns, it failed - print error and exit(1)
    // CRITICAL: Child must call exit(1), NOT return!


    // TODO 3: Parent process - Wait for child to complete
    // Use waitpid(pid, &status, 0) to wait for the specific child
    // Check if child exited normally with WIFEXITED(status)
    // If yes, return the exit code with WEXITSTATUS(status)
    // Otherwise return -1

    return -1;  // This line should be replaced by your TODO 3 code
}