#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

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

        buffer[read_bytes - 1] = '\0';
