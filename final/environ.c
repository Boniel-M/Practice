#include "test.h"

void printEnvironment() {
    char** env = environ;

    while (*env != NULL) {
        write(STDOUT_FILENO, *env, strlen(*env));
        write(STDOUT_FILENO, "\n", 1);
        env++;
    }
}

