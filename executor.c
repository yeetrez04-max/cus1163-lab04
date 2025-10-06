#include <stdio.h>      // for printf, perror
#include <stdlib.h>     // for exit
#include <sys/types.h>  // for pid_t
#include <sys/wait.h>   // for waitpid, WIFEXITED, etc.
#include <unistd.h>     // for fork, execvp

int execute_command(char *command, char **args) {
    pid_t pid;
    int status;

    // TODO 1: Fork the child process
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    // TODO 2: Child process - execute command
    if (pid == 0) {
        execvp(command, args);
        perror("execvp failed");
        exit(1);
    }

    // TODO 3: Parent process - wait for the child
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else {
        return -1;
    }
}

