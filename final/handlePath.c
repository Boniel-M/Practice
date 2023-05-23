#include "test.h"
/**
 * handlePath - function that handles path
 *
 * @command: command to pass on the shell prompt
 *
 * Return: 0 when success
 */
int handlePath(char *command)
{
	char *path;

	if (access(command, X_OK) == 0)
	{
		parseCommand(command);
		return (1);
	}
	path = getenv("PATH");

	if (path == NULL)
	{
		write(STDOUT_FILENO, "PATH variable not found\n", 24);
		return (0);
	}
	char *token = strtok(path, ":");

	while (token != NULL)
	{
		char commandPath[MAX_COMMAND_LENGTH];
		int i, j;

		for (i = 0; token[i] != '\0'; i++)
		{
			commandPath[i] = token[i];
		}
		commandPath[i++] = '/';

		for (j = 0; command[j] != '\0'; j++, i++)
		{
			commandPath[i] = command[j];
		}
		commandPath[i] = '\0';

		if (access(commandPath, X_OK) == 0)
		{
			parseCommand(commandPath);
			return (1);
		}
		token = strtok(NULL, ":");
	}
	return (0);
}
