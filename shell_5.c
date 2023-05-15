#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define ARGUMENTS_SIZE 64
#define PATH_SIZE 1024

extern char **environ;  // External variable holding the environment

int main(void) {
    char *buffer;
    ssize_t read_bytes;
    size_t bufsize = BUFFER_SIZE;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if (buffer == NULL) {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("($) ");
        read_bytes = getline(&buffer, &bufsize, stdin);
        if (read_bytes == -1) {
            if (feof(stdin)) {
                printf("\n");
                break;
            } else {
                perror("getline error");
                exit(EXIT_FAILURE);
            }
        }

        buffer[read_bytes - 1] = '\0';  // Remove the newline character

        char *arguments[ARGUMENTS_SIZE];
        char *token;
        int arg_index = 0;

        // Tokenize the input command line
        token = strtok(buffer, " ");
        while (token != NULL) {
            arguments[arg_index] = token;
            arg_index++;

            token = strtok(NULL, " ");
        }
        arguments[arg_index] = NULL;  // Set the last element to NULL for execve()

        // Check if the command is the "exit" built-in
        if (strcmp(arguments[0], "exit") == 0) {
            break;
        }

        // Check if the command is the "env" built-in
        if (strcmp(arguments[0], "env") == 0) {
            char **env_ptr = environ;
            while (*env_ptr != NULL) {
                printf("%s\n", *env_ptr);
                env_ptr++;
            }
            continue;
        }

        // Check if the command exists in PATH
        int command_exists = 0;
        char *path = getenv("PATH");
        char *path_token;
        char command_path[PATH_SIZE];

        if (path != NULL) {
            path_token = strtok(path, ":");
            while (path_token != NULL) {
                snprintf(command_path, sizeof(command_path), "%s/%s", path_token, arguments[0]);

                if (access(command_path, F_OK) == 0) {
                    command_exists = 1;
                    break;
                }

                path_token = strtok(NULL, ":");
            }
        }

        if (command_exists) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork error");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Child process
                if (execve(command_path, arguments, NULL) == -1) {
                    perror(command_path);
                    exit(EXIT_FAILURE);
                }
            } else {
                // Parent process
                wait(NULL);
            }
        } else {
            printf("%s: command not found\n", arguments[0]);
        }
    }

    free(buffer);

    return EXIT_SUCCESS;
}

