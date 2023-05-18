#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>  // Include sys/wait.h for waitpid

extern char** environ;

#define MAX_COMMAND_LENGTH 100

// Function to display the prompt and wait for user input
void displayPrompt() {
    printf(">> ");
    fflush(stdout);
}

// Function to parse the command line and handle command lines with arguments
void parseCommand(char* command) {
    char* arguments[2];  // Maximum 2 arguments: command and NULL terminator
    char* token = strtok(command, " \n");  // Split command line by spaces and newline

    int i = 0;
    while (token != NULL && i < 2) {
        arguments[i] = token;
        i++;
        token = strtok(NULL, " \n");
    }
    arguments[i] = NULL;  // Set the last argument to NULL terminator

    // Execute the command using execve
    execve(arguments[0], arguments, environ);

    // If execve returns, it means the command execution failed
    perror("Error");
    exit(EXIT_FAILURE);
}

// Function to implement the exit built-in
void exitShell() {
    exit(EXIT_SUCCESS);
}

// Function to implement the env built-in
void printEnvironment() {
    extern char** environ;
    char** env = environ;

    while (*env != NULL) {
        printf("%s\n", *env);
        env++;
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        displayPrompt();

        // Read user input
        if (fgets(command, sizeof(command), stdin) == NULL) {
            // Handle end of file condition (Ctrl+D)
            printf("\n");
            break;
        }

        // Remove trailing newline character
        command[strcspn(command, "\n")] = '\0';

        // Check for built-in commands
        if (strcmp(command, "exit") == 0) {
            exitShell();
        } else if (strcmp(command, "env") == 0) {
            printEnvironment();
            continue;
        }

        // Fork and execute the command
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork error");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            parseCommand(command);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

