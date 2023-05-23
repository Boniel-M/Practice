#include "test.h"

int main() {
    char* command = NULL;
    size_t command_size = 0;

    while (1) {
        displayPrompt();

        ssize_t input_length = getline(&command, &command_size, stdin);

        if (input_length == -1) {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0) {
            exitShell();
	    exit_Shell(0);
        } else if (strcmp(command, "env") == 0) {
            printEnvironment();
            continue;
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork error");
            _exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (!handlePath(command)) {
                char* token = strtok(command, " \n");
                char* arguments[MAX_COMMAND_LENGTH];
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
            _exit(EXIT_SUCCESS);
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    free(command);
    return 0;
}

