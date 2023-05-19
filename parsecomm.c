#include "test.h"

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
