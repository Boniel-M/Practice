#include "test.h"

void parseCommand(char* command) {
    char* arguments[MAX_COMMAND_LENGTH];
    char* token = strtok(command, " \n");

    int i = 0;
    while (token != NULL && i < MAX_COMMAND_LENGTH - 1) {
        arguments[i] = token;
        i++;
        token = strtok(NULL, " \n");
    }
    arguments[i] = NULL;

    execve(arguments[0], arguments, environ);

    perror("Error");
    _exit(EXIT_FAILURE);
}

