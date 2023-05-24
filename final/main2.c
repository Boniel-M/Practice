#include "test.h"

/**
 * main - entry point.
 *
 * Return: Always 0 (success).
 */

int main(void)
{
	char *command = NULL;
	size_t command_size = 0;
	ssize_t input_length;

	while (1)
	{
		displayPrompt();

		input_length = getline(&command, &command_size, stdin);
		if (input_length == -1)
		{
			write(STDOUT_FILENO, "\n", 1);
				break;
		}
		command[strcspn(command, "\n")] = '\0';

		if (strcmp(command, "exit") == 0)
		{
			exitShell();
		}
		else if (strcmp(command, "env") == 0)
		{
			printEnvironment();
			continue;
		}
		executeCommand(command);
	}

	free(command);
	return (0);
}
