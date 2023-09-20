#include "shell.h"

/**
 * isCommandChain - checks if the current character in the buffer is a command chaining delimiter
 * @info: the parameter struct
 * @buffer: the character buffer
 * @position: address of the current position in the buffer
 *
 * Return: 1 if it's a command chaining delimiter, 0 otherwise
 */
int isCommandChain(info_t *info, char *buffer, size_t *position)
{
	size_t j = *position;

	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		info->commandBufferType = CMD_OR;
	}
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		info->commandBufferType = CMD_AND;
	}
	else if (buffer[j] == ';') /* Found the end of this command */
	{
		buffer[j] = 0; /* Replace semicolon with null terminator */
		info->commandBufferType = CMD_CHAIN;
	}
	else
		return 0;
	*position = j;
	return 1;
}

/**
 * checkCommandChain - checks if we should continue chaining based on the last command's status
 * @info: the parameter struct
 * @buffer: the character buffer
 * @position: address of the current position in the buffer
 * @startIndex: starting position in the buffer
 * @length: length of the buffer
 *
 * Return: void
 */
void checkCommandChain(info_t *info, char *buffer, size_t *position, size_t startIndex, size_t length)
{
	size_t j = *position;

	if (info->commandBufferType == CMD_AND)
	{
		if (info->status)
		{
			buffer[startIndex] = 0;
			j = length;
		}
	}
	if (info->commandBufferType == CMD_OR)
	{
		if (!info->status)
		{
			buffer[startIndex] = 0;
			j = length;
		}
	}

	*position = j;
}

/**
 * replaceAliases - replaces aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceAliases(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = findNodeWithPrefix(info->aliases, info->arguments[0], '=');
		if (!node)
			return 0;
		free(info->arguments[0]);
		p = strchr(node->string, '=');
		if (!p)
			return 0;
		p = strdup(p + 1);
		if (!p)
			return 0;
		info->arguments[0] = p;
	}
	return 1;
}

/**
 * replaceVariables - replaces variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceVariables(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->arguments[i]; i++)
	{
		if (info->arguments[i][0] != '$' || !info->arguments[i][1])
			continue;

		if (!strcmp(info->arguments[i], "$?"))
		{
			replaceString(&(info->arguments[i]),
				strdup(convertNumber(info->status, 10, 0)));
			continue;
		}
		if (!strcmp(info->arguments[i], "$$"))
		{
			replaceString(&(info->arguments[i]),
				strdup(convertNumber(getpid(), 10, 0)));
			continue;
		}
		node = findNodeWithPrefix(info->environment, &info->arguments[i][1], '=');
		if (node)
		{
			replaceString(&(info->arguments[i]),
				strdup(strchr(node->string, '=') + 1));
			continue;
		}
		replaceString(&info->arguments[i], strdup(""));
	}
	return 0;
}

/**
 * replaceString - replaces a string
 * @oldString: address of the old string
 * @newString: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replaceString(char **oldString, char *newString)
{
	free(*oldString);
	*oldString = newString;
	return 1;
}
