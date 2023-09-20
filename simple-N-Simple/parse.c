#include "shell.h"

/**
 * isExecutable - Checks if a file is an executable command
 * @info: Pointer to info struct
 * @filePath: Path to the file to check
 *
 * Return: 1 if executable, 0 otherwise
 */
int isExecutable(info_t *info, char *filePath)
{
	struct stat fileStat;

	(void)info;
	if (!filePath || stat(filePath, &fileStat))
		return (0);

	if (fileStat.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * duplicateSubstring - Duplicates a substring from a string
 * @source: Source string
 * @start: Start index of the substring
 * @stop: Stop index of the substring
 *
 * Return: Pointer to the new buffer
 */
char *duplicateSubstring(char *source, int start, int stop)
{
	static char buffer[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (source[i] != ':')
			buffer[k++] = source[i];
	buffer[k] = 0;
	return (buffer);
}

/**
 * findExecutablePath - Finds the full path of an executable command in PATH
 * @info: Pointer to info struct
 * @pathString: PATH string
 * @command: Command to find
 *
 * Return: Full path of the command if found, or NULL
 */
char *findExecutablePath(info_t *info, char *pathString, char *command)
{
	int i = 0, currentPosition = 0;
	char *fullPath;

	if (!pathString)
		return (NULL);
	if ((_strlen(command) > 2) && starts_with(command, "./"))
	{
		if (isExecutable(info, command))
			return (command);
	}
	while (1)
	{
		if (!pathString[i] || pathString[i] == ':')
		{
			fullPath = duplicateSubstring(pathString, currentPosition, i);
			if (!*fullPath)
				_strcat(fullPath, command);
			else
			{
				_strcat(fullPath, "/");
				_strcat(fullPath, command);
			}
			if (isExecutable(info, fullPath))
				return (fullPath);
			if (!pathString[i])
				break;
			currentPosition = i;
		}
		i++;
	}
	return (NULL);
}
