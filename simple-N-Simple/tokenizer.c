#include "shell.h"

/**
 * splitString - splits a string into words. Repeated delimiters are ignored.
 * @input: the input string
 * @delimiter: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **splitString(char *input, char *delimiter)
{
	int i, j, k, m, numWords = 0;
	char **words;

	if (input == NULL || input[0] == 0)
		return (NULL);
	if (!delimiter)
		delimiter = " ";
	for (i = 0; input[i] != '\0'; i++)
		if (!isDelimiter(input[i], delimiter) && (isDelimiter(input[i + 1], delimiter) || !input[i + 1]))
			numWords++;

	if (numWords == 0)
		return (NULL);
	words = malloc((1 + numWords) * sizeof(char *));
	if (!words)
		return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (isDelimiter(input[i], delimiter))
			i++;
		k = 0;
		while (!isDelimiter(input[i + k], delimiter) && input[i + k])
			k++;
		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			words[j][m] = input[i++];
		words[j][m] = 0;
	}
	words[j] = NULL;
	return (words);
}

/**
 * splitString2 - splits a string into words based on a single delimiter
 * @input: the input string
 * @delimiter: the delimiter character
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **splitString2(char *input, char delimiter)
{
	int i, j, k, m, numWords = 0;
	char **words;

	if (input == NULL || input[0] == 0)
		return (NULL);
	for (i = 0; input[i] != '\0'; i++)
		if ((input[i] != delimiter && input[i + 1] == delimiter) ||
			(input[i] != delimiter && !input[i + 1]) || input[i + 1] == delimiter)
			numWords++;

	if (numWords == 0)
		return (NULL);
	words = malloc((1 + numWords) * sizeof(char *));
	if (!words)
		return (NULL);
	for (i = 0, j = 0; j < numWords; j++)
	{
		while (input[i] == delimiter && input[i] != delimiter)
			i++;
		k = 0;
		while (input[i + k] != delimiter && input[i + k] && input[i + k] != delimiter)
			k++;
		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			words[j][m] = input[i++];
		words[j][m] = 0;
	}
	words[j] = NULL;
	return (words);
}
