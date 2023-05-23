#include "test.h"

int handlePath(char* command) {
    if (access(command, X_OK) == 0) {
        parseCommand(command);
        return 1;
    }

    char* path = getenv("PATH");
    if (path == NULL) {
        write(STDOUT_FILENO, "PATH variable not found\n", 24);
        return 0;
    }

    char* token = strtok(path, ":");
    while (token != NULL) {
        char commandPath[MAX_COMMAND_LENGTH];
        int i, j;
        for (i = 0; token[i] != '\0'; i++) {
            commandPath[i] = token[i];
        }
        commandPath[i++] = '/';
        for (j = 0; command[j] != '\0'; j++, i++) {
            commandPath[i] = command[j];
        }
        commandPath[i] = '\0';

        if (access(commandPath, X_OK) == 0) {
            parseCommand(commandPath);
            return 1;
        }
        token = strtok(NULL, ":");
    }

    return 0;
}

