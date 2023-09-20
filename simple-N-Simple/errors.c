#include "shell.h"

/**
 * print_error_string - Prints an input string to stderr.
 * @str: The string to be printed.
 *
 * Return: Nothing.
 */
void print_error_string(char *str)
{
	int index = 0;

	if (!str)
		return;
	while (str[index] != '\0')
	{
		print_error_char(str[index]);
		index++;
	}
}

/**
 * print_error_char - Writes the character c to stderr.
 * @c: The character to print.
 *
 * Return: On success, returns 1.
 * On error, returns -1, and errno is set appropriately.
 */
int print_error_char(char c)
{
	static int i;
	static char buffer[WRITE_BUFFER_SIZE];

	if (c == BUFFER_FLUSH || i >= WRITE_BUFFER_SIZE)
	{
		write(2, buffer, i);
		i = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[i++] = c;
	return (1);
}

/**
 * write_to_fd - Writes the character c to the given file descriptor.
 * @c: The character to print.
 * @fd: The file descriptor to write to.
 *
 * Return: On success, returns 1.
 * On error, returns -1, and errno is set appropriately.
 */
int write_to_fd(char c, int fd)
{
	static int i;
	static char buffer[WRITE_BUFFER_SIZE];

	if (c == BUFFER_FLUSH || i >= WRITE_BUFFER_SIZE)
	{
		write(fd, buffer, i);
		i = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[i++] = c;
	return (1);
}

/**
 * print_string_to_fd - Prints an input string to the given file descriptor.
 * @str: The string to be printed.
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters written.
 */
int print_string_to_fd(char *str, int fd)
{
	int count = 0;

	if (!str)
		return (0);
	while (*str)
	{
		count += write_to_fd(*str++, fd);
	}
	return (count);
}
