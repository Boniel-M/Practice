#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define ARGUMENTS_SIZE 64

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

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork error");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            if (execve(arguments[0], arguments, NULL) == -1) {
                perror(arguments[0]);
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            wait(NULL);
        }
    }

    free(buffer);

    return EXIT_SUCCESS;
}
