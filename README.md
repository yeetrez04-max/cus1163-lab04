# Lab 4: Simple Command Executor

#### Learning Objectives

- Understand `exec()` family of system calls and how processes transform into new programs
- Master the fork-exec-wait pattern that forms the foundation of process management in Unix
- Practice command parsing and argument handling
- Learn error handling for process operations

#### Prerequisites

- Completed Lab 3 (Process Management and IPC)
- Understanding of `fork()` and `wait()` system calls
- Basic C programming knowledge

#### Introduction

Every time you type a command in a terminal, the shell uses the fork-exec-wait pattern to run it. The shell forks a new
process, that process transforms itself into your command using `exec()`, and the shell waits for it to complete.

In this lab, you'll build a simple command executor that works like a mini-shell. Your program will accept commands from
the user, fork a child process, use `exec()` to run the command, and wait for it to complete. The commands you'll
execute are real Linux programs. When a user types "ls -l", your executor will actually run the ls program with the -l
argument.

#### What You'll Implement

You need to write just 1 function with 3 TODO sections:

- `execute_command()` - Forks a child, execs the command, and waits for completion

Everything else is provided for you.

#### Lab File Structure

**main.c (PROVIDED - Complete)**

This file contains the main entry point and user interface. It handles user input, parses commands into arguments, and
calls your `execute_command()` function. This file requires no changes.

**executor.h (PROVIDED)**

This header file contains function prototypes and necessary system headers. No changes are necessary for this file.

**executor.c (TEMPLATE - TO COMPLETE)**

This file contains the core logic for command execution. You will implement the fork-exec-wait pattern by completing 3
TODO comments.

#### Project Setup

1. Clone the repo: `git clone <repo_url> && cd <repo_name>`
2. Complete the 3 TODOs in executor.c
3. Compile: `gcc -o lab4 main.c executor.c`
4. Run: `./lab4`

#### The Function You'll Implement

The function `execute_command()` demonstrates the most important pattern in Unix process management. Every shell uses
this exact pattern.

Here's what happens step by step:

1. Parent process forks a child process
2. Child process calls `execvp()` to transform into the target command
3. If `exec()` succeeds, the child is now running the new program
4. If `exec()` fails, the child prints an error and exits
5. Parent process waits for the child to complete
6. Parent returns the child's exit status

#### Expected Output

**Starting the Program:**

```
========================================
   Simple Command Executor
========================================
Type 'exit' to quit

cmd> 
```

**Example Session:**

```
cmd> ls
main.c
executor.c
executor.h
lab4

Command 'ls' completed with exit status: 0

cmd> pwd
/home/student/os-labs/lab4

Command 'pwd' completed with exit status: 0

cmd> echo Hello World
Hello World

Command 'echo Hello World' completed with exit status: 0

cmd> date
Tue Sep 30 10:25:43 PDT 2025

Command 'date' completed with exit status: 0

cmd> invalidcommand
execvp: No such file or directory

Command 'invalidcommand' completed with exit status: 1

cmd> exit
Goodbye!
```

#### Key System Calls You'll Use

**fork() - Process Creation (Review from Lab 3)**

Creates a new process by duplicating the current process. Returns:

- 0 in the child process
- Child's PID in the parent process
- -1 on error

```c
pid_t pid = fork();
if (pid == 0) {
    // Child process
} else if (pid > 0) {
    // Parent process
} else {
    perror("fork failed");
}
```

**execvp() - Program Execution**

Transforms the current process into a different program.

```c
char *args[] = {"ls", "-l", NULL};
execvp("ls", args);

// If we reach here, exec failed!
perror("execvp");
exit(1);
```

**Important:** If `exec()` succeeds, it NEVER returns. Your process has been transformed. If `exec()` returns, something
went wrong.

**waitpid() - Process Synchronization (Review from Lab 3)**

Makes the parent process pause until the child exits.

```c
int status;
waitpid(child_pid, &status, 0);

if (WIFEXITED(status)) {
    int exit_status = WEXITSTATUS(status);
    printf("Child exited with status: %d\n", exit_status);
}
```

#### Implementation Guide

**TODO 1: Fork the Child Process**

Pseudocode:

```
CREATE a new process using fork()
STORE the result in pid variable

IF fork failed (pid is negative) THEN
    PRINT error message
    RETURN -1 to indicate failure
END IF
```

**TODO 2: Child Process - Execute the Command**

Pseudocode:

```
IF we are in the child process (pid equals 0) THEN
    TRANSFORM this process into the target command using execvp()
    
    // If execution reaches here, execvp failed
    PRINT error message about exec failure
    EXIT the child process with status 1
END IF
```

**Critical:** The child must call `exit()`, not `return`. If you return, you'll have two processes executing parent
code.

**TODO 3: Parent Process - Wait for Child**

Pseudocode:

```
// At this point, we are in the parent process
DECLARE status variable

WAIT for the specific child process to complete using waitpid()

IF child exited normally THEN
    EXTRACT and RETURN the exit status code
ELSE
    RETURN -1 to indicate abnormal termination
END IF
```

**Complete Implementation Structure (Pseudocode)**

```
FUNCTION execute_command(command, args):
    // TODO 1: Fork the child process
    pid = CREATE new process
    IF pid < 0 THEN
        PRINT "fork failed"
        RETURN -1
    END IF
    
    // TODO 2: Child process - execute the command
    IF pid == 0 THEN
        TRANSFORM into command using execvp(command, args)
        PRINT "execvp failed"
        EXIT with status 1
    END IF
    
    // TODO 3: Parent process - wait for child
    DECLARE status
    WAIT for child with pid using waitpid(pid, &status, 0)
    
    IF child exited normally THEN
        RETURN child's exit status
    END IF
    
    RETURN -1
END FUNCTION
```

#### Why These Concepts Matter

The fork-exec-wait pattern is how every program gets started. When you type a command in bash, bash forks a child, the
child execs your command, and bash waits for completion.

This same pattern is used by:

- All Unix shells (bash, zsh, sh)
- Service managers (systemd, init)
- Process launchers and GUI application starters
- Web servers handling requests
- Container runtimes

#### Common Mistakes to Avoid

**Not calling `exit()` in the child after failed `exec()`**

```c
if (pid == 0) {
    execvp(command, args);
    perror("execvp");
    return -1;  // WRONG! Use exit(1) instead
}
```

**Forgetting NULL terminator in args array**

```c
char *args[] = {"ls", "-l"};  // WRONG! Missing NULL
execvp("ls", args);
```

Correct version:

```c
char *args[] = {"ls", "-l", NULL};  // Correct!
execvp("ls", args);
```

**Using `exec()` in parent process**

```c
if (pid > 0) {
    execvp(command, args);  // WRONG! Parent becomes the command
}
```

**Not checking `WIFEXITED()` before `WEXITSTATUS()`**

```c
int status;
waitpid(pid, &status, 0);
return WEXITSTATUS(status);  // WRONG! Check WIFEXITED first
```

#### Testing Your Implementation

**Basic commands:**

- `ls`
- `pwd`
- `date`
- `whoami`

**Commands with arguments:**

- `ls -l`
- `echo Hello World`
- `cat file.txt`

**Commands that fail:**

- `invalidcommand`
- `ls /nonexistent`

#### Analysis Questions

Answer all four questions in your submission:

1. What happens to the child's memory when `exec()` is called?
2. Why do we need both `fork()` and `exec()`? Why not one system call?
3. What happens if you call `exec()` in the parent process instead of the child?
4. Why must the child call `exit()` instead of return after a failed `exec()`?

#### Compilation and Execution

```bash
gcc -o lab4 main.c executor.c
./lab4
```

#### Submission Requirements

```bash
git add .
git commit -m "completed lab 4 - simple command executor"
git push origin main
```

Include:

- Completed executor.c with all 3 TODOs implemented
- Screenshots showing various commands being executed
- Test output showing both successful and failed commands

#### Grading Rubric

**Total Points: 20**

Working Code (15 points)

| Component                                       | Criteria                                                  | Points       |
|-------------------------------------------------|-----------------------------------------------------------|--------------|
| **TODO 1: Fork the Child Process**              |                                                           | **5 points** |
|                                                 | Correctly uses fork() system call                         | 2            |
|                                                 | Properly checks for fork() failure (pid < 0)              | 2            |
|                                                 | Returns -1 on fork failure with appropriate error message | 1            |
| **TODO 2: Child Process - Execute the Command** |                                                           | **5 points** |
|                                                 | Correctly identifies child process (pid == 0)             | 1            |
|                                                 | Uses execvp() with correct parameters (command, args)     | 2            |
|                                                 | Handles exec failure with perror()                        | 1            |
|                                                 | Child calls exit(1) instead of return after failed exec   | 1            |
| **TODO 3: Parent Process - Wait for Child**     |                                                           | **5 points** |
|                                                 | Correctly uses waitpid() to wait for specific child       | 2            |
|                                                 | Properly checks if child exited normally with WIFEXITED() | 1            |
|                                                 | Returns child's exit status using WEXITSTATUS()           | 1            |
|                                                 | Returns -1 for abnormal termination                       | 1            |

Analysis Questions (3 points)

Answer ALL of the following questions (0.75 points each):

1. What happens to the child's memory when exec() is called?
2. Why do we need both fork() and exec()? Why not one system call?
3. What happens if you call exec() in the parent process instead of the child?
4. Why must the child call exit() instead of return after a failed exec()?

| Grading Criteria                                     | Points per Question |
|------------------------------------------------------|---------------------|
| Demonstrates understanding of fork-exec-wait pattern | 0.375               |
| Provides clear and accurate explanation              | 0.375               |
| **Total per question**                               | **0.75**            |
| **Total for all 4 questions**                        | **3.0**             |

Screenshots (2 points)

| Component          | Criteria                                                                                                              | Points |
|--------------------|-----------------------------------------------------------------------------------------------------------------------|--------|
| **Basic Commands** | Screenshot showing successful execution of at least 3 basic commands (ls, pwd, date, whoami, echo) with exit status 0 | 1      |
| **Error Handling** | Screenshot showing invalid command with error message and non-zero exit status                                        | 1      |

**Total: 20 points**

#### Resources

**Linux Manual Pages**

Access in your terminal:

```bash
man 2 fork      # Process creation
man 3 execvp    # Execute program with PATH search
man 2 waitpid   # Wait for specific process
```

Online:

- fork(2): https://man7.org/linux/man-pages/man2/fork.2.html
- execvp(3): https://man7.org/linux/man-pages/man3/exec.3.html
- waitpid(2): https://man7.org/linux/man-pages/man2/waitpid.2.html

**fork() - Process Creation**

```c
#include <unistd.h>
pid_t fork(void);
```

- Returns 0 in child
- Returns child's PID in parent
- Returns -1 on error

**execvp() - Execute Program**

```c
#include <unistd.h>
int execvp(const char *file, char *const argv[]);
```

- file: Program name (searches PATH)
- argv: Arguments array ending with NULL
- Only returns on error

**waitpid() - Wait for Process**

```c
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *status, int options);
```

- WIFEXITED(status): True if exited normally
- WEXITSTATUS(status): Get exit code (0-255)

**Common Errors**

- fork: Resource temporarily unavailable - Too many processes, wait for processes to finish
- execvp: No such file or directory - Command not found, check command spelling
- execvp: Permission denied - Not executable, check file permissions

**Debugging Commands**

```bash
# Compile with debug symbols
gcc -g -o lab4 main.c executor.c

# Trace system calls
strace ./lab4

# Check for zombie processes
ps aux | grep Z
```

**Related Resources**

- Beej's Guide to Unix IPC - Fork: https://beej.us/guide/bgipc/html/split/fork.html
- GNU C Library - Process Creation: https://www.gnu.org/software/libc/manual/html_node/Process-Creation-Concepts.html

**Tip:** Use `man 2 <syscall>` for system calls and `man 3 <function>` for library functions