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
	char *token;
	char command[MAX_COMMAND_LENGTH];
	int i, j;

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
	
	token = strtok(path, ":");

	while (token != NULL)
	{

		commandPath[x++] = '/';

		for (y = 0; command[y] != '\0'; y++, x++)
			commandPath[x] = command[y];
		commandPath[x] = '\0';

		if (access(commandPath, X_OK) == 0)
		{
			parseCommand(commandPath);
			return (1);
		}
		token = strtok(NULL, ":");
	}
	return (0);
}
