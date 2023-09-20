#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>


extern char **environ;
#define MAX_COMMAND_LENGTH 100

void displayPrompt(void);
void parseCommand(char *command);
void exitShell(void);
void printEnvironment(void);
int handlePath(char *command);
size_t my_getline(char **string, size_t *n, FILE *stream);
void exit_Shell(int status);
void executeCommand(char *command);

#endif /* TEST_H */
