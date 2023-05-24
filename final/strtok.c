#include "test.h"
#include <stdbool.h>

/**
 * my_strtok - function that prints strings of a path
 *
 * @str: string to be printed
 * @delimiters: delimeters to pass
 *
 * Return: a token
 */

char *my_strtok(char *str, const char *delimiters)
{
	static char *next_token;
	/* Stores the next token position */

	if (str != NULL)
	{
		next_token = str;  /* Initialize with the given string */
	}
	if (next_token == NULL || *next_token == '\0')
	{
		return (NULL);  /* No more tokens to extract */
	}
	/* Find the start of the token */
	while (*next_token != '\0' && strchr(delimiters, *next_token) != NULL)
	{
		next_token++;
	}

	if (*next_token == '\0')
	{
		return (NULL);  /* Reached the end of the string */
	}
	/* Mark the start of the token and find the end of the token */
	char *token_start = next_token;

	while (*next_token != '\0' && strchr(delimiters, *next_token) == NULL)
	{
		next_token++;
	}
	if (*next_token != '\0')
	{
		*next_token = '\0';  /* Replace delimiter with null character */
		next_token++;  /* Move to the next character for the next call */
	}
	return (token_start);
}
