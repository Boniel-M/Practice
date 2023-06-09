#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char** environ;
#define MAX_COMMAND_LENGTH 100

void displayPrompt(void);
void parseCommand(char* command);
void exitShell();
void printEnvironment();

#endif
