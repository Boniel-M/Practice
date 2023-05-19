#include "test.h"

// Function to implement the env built-in
void printEnvironment()
{
    char** env = environ;

    while (*env != NULL) {
        printf("%s\n", *env);
        env++;
    }
}
