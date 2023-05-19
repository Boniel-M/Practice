#include "test.h"

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

    return (0);
}
