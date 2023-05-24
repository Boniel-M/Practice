#include "test.h"

/**
 * executeCommand - executes commands passed on the command line.
 *@command: commands passed to the command line.
 *Return: void.
 */

void executeCommand(char *command)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("Fork error");
		_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (!handlePath(command))
		{
			char *token = strtok(command, " \n");
			char *arguments[MAX_COMMAND_LENGTH];
			int i = 0;

			while (token != NULL && i < MAX_COMMAND_LENGTH - 1)
			{
				arguments[i] = token;
				i++;
				token = strtok(NULL, " \n");
			}
			arguments[i] = NULL;
			execve(arguments[0], arguments, environ);
			perror("Error");
			_exit(EXIT_FAILURE);
		}
		_exit(EXIT_SUCCESS);
	}
	else
	{
		int status;

		waitpid(pid, &status, 0);
	}
}
