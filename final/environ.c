#include "test.h"
/**
 *  printEnvironment - function that prints environment of the shell
 *
 *  Return: void
 */
void printEnvironment(void)
{
	char **env = environ;

	while (*env != NULL)
	{
		write(STDOUT_FILENO, *env, strlen(*env));
		write(STDOUT_FILENO, "\n", 1);
		env++;
	}
}
