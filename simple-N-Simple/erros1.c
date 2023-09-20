#include "shell.h"

/**
 * string_to_int - Converts a string to an integer.
 * @s: The string to be converted.
 * Return: 0 if no numbers in string, converted number otherwise,
 *         -1 on error.
 */
int string_to_int(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * print_error_message - Prints an error message.
 * @info: Pointer to the parameter and return info struct.
 * @error_string: String containing specified error type.
 */
void print_error_message(info_t *info, char *error_string)
{
	print_string(info->filename);
	print_string(": ");
	print_integer(info->line_count, STDERR_FILENO);
	print_string(": ");
	print_string(info->argv[0]);
	print_string(": ");
	print_string(error_string);
}

/**
 * print_integer - Prints an integer number.
 * @input: The integer to print.
 * @fd: The file descriptor to write to.
 *
 * Return: Number of characters printed.
 */
int print_integer(int input, int fd)
{
	int (*put_char_function)(char) = put_character;
	int i, count = 0;
	unsigned int absolute_value, current;

	if (fd == STDERR_FILENO)
		put_char_function = put_error_character;
	if (input < 0)
	{
		absolute_value = -input;
		put_char_function('-');
		count++;
	}
	else
		absolute_value = input;
	current = absolute_value;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absolute_value / i)
		{
			put_char_function('0' + current / i);
			count++;
		}
		current %= i;
	}
	put_char_function('0' + current);
	count++;

	return (count);
}

/**
 * convert_to_string - Converts a number to a string.
 * @num: Number to convert.
 * @base: Base for conversion.
 * @flags: Argument flags.
 *
 * Return: The converted string.
 */
char *convert_to_string(long int num, int base, int flags)
{
	static char *conversion_array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}
	conversion_array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do {
		*--ptr = conversion_array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * remove_first_comment - Replaces the first instance of '#' with '\0'.
 * @buf: Address of the string to modify.
 */
void remove_first_comment(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}

