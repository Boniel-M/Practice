#include "shell.h"

/**
 * read_input_buffer - Reads and buffers chained commands.
 * @info: Pointer to the parameter struct.
 * @buf: Address of the buffer.
 * @len: Address of the length variable.
 *
 * Return: Bytes read.
 */
ssize_t read_input_buffer(info_t *info, char **buf, size_t *len)
{
	ssize_t bytes_read = 0;
	size_t buffer_length = 0;

	if (!*len) /* If buffer is empty, read input. */
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, interruptHandler);
#if USE_GETLINE
		bytes_read = getline(buf, &buffer_length, stdin);
#else
		bytes_read = custom_getline(info, buf, &buffer_length);
#endif
		if (bytes_read > 0)
		{
			if ((*buf)[bytes_read - 1] == '\n')
			{
				(*buf)[bytes_read - 1] = '\0'; /* Remove trailing newline. */
				bytes_read--;
			}
			info->line_count_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->history_count++);
			{
				*len = bytes_read;
				info->command_buffer = buf;
			}
		}
	}
	return (bytes_read);
}

/**
 * get_input_line - Retrieves a line of input without the newline character.
 * @info: Pointer to the parameter struct.
 *
 * Return: Bytes read.
 */
ssize_t get_input_line(info_t *info)
{
	static char *buffer; /* The ';' command chain buffer */
	static size_t position_i, position_j, buffer_length;
	ssize_t bytes_read = 0;
	char **buffer_ptr = &(info->arg_str), *ptr;

	_putchar(BUF_FLUSH);
	bytes_read = read_input_buffer(info, &buffer, &buffer_length);
	if (bytes_read == -1) /* EOF */
		return (-1);
	if (buffer_length) /* If there are commands left in the chain buffer. */
	{
		position_j = position_i; /* Initialize new iterator to the current buffer position. */
		ptr = buffer + position_i; /* Get a pointer for return. */

		check_chain(info, buffer, &position_j, position_i, buffer_length);
		while (position_j < buffer_length) /* Iterate to semicolon or end. */
		{
			if (is_chain(info, buffer, &position_j))
				break;
			position_j++;
		}

		position_i = position_j + 1; /* Increment past the nullified semicolon. */
		if (position_i >= buffer_length) /* Reached the end of the buffer? */
		{
			position_i = buffer_length = 0; /* Reset position and length. */
			info->command_buffer_type = COMMAND_NORMAL;
		}

		*buffer_ptr = ptr; /* Pass back a pointer to the current command position. */
		return (_strlen(ptr)); /* Return the length of the current command. */
	}

	*buffer_ptr = buffer; /* If not a chain, pass back the buffer from custom_getline(). */
	return (bytes_read); /* Return the length of the buffer from custom_getline(). */
}

/**
 * read_buffer - Reads a buffer.
 * @info: Pointer to the parameter struct.
 * @buffer: Buffer.
 * @position_i: Size.
 *
 * Return: Bytes read.
 */
ssize_t read_buffer(info_t *info, char *buffer, size_t *position_i)
{
	ssize_t bytes_read = 0;

	if (*position_i)
		return (0);
	bytes_read = read(info->read_file_descriptor, buffer, READ_BUFFER_SIZE);
	if (bytes_read >= 0)
		*position_i = bytes_read;
	return (bytes_read);
}

/**
 * custom_getline - Gets the next line of input from STDIN.
 * @info: Pointer to the parameter struct.
 * @pointer: Address of the pointer to the buffer, preallocated or NULL.
 * @length: Size of the preallocated pointer buffer if not NULL.
 *
 * Return: Number of characters read.
 */
int custom_getline(info_t *info, char **pointer, size_t *length)
{
	static char buffer[READ_BUFFER_SIZE];
	static size_t position_i, buffer_length;
	size_t k;
	ssize_t bytes_read = 0, characters_read = 0;
	char *ptr = NULL, *new_ptr = NULL, *c;

	ptr = *pointer;
	if (ptr && length)
		characters_read = *length;
	if (position_i == buffer_length)
		position_i = buffer_length = 0;

	bytes_read = read_buffer(info, buffer, &buffer_length);
	if (bytes_read == -1 || (bytes_read == 0 && buffer_length == 0))
		return (-1);

	c = _strchr(buffer + position_i, '\n');
	k = c ? 1 + (unsigned int)(c - buffer) : buffer_length;
	new_ptr = _realloc(ptr, characters_read, characters_read ? characters_read + k : k + 1);
	if (!new_ptr) /* MALLOC FAILURE! */
		return (ptr ? free(ptr), -1 : -1);

	if (characters_read)
		_strncat(new_ptr, buffer + position_i, k - position_i);
	else
		_strncpy(new_ptr, buffer + position_i, k - position_i + 1);

	characters_read += k - position_i;
	position_i = k;
	ptr = new_ptr;

	if (length)
		*length = characters_read;
	*pointer = ptr;
	return (characters_read);
}

/**
 * interruptHandler - Blocks Ctrl-C.
 * @signal_number: The signal number.
 *
 * Return: void.
 */
void interruptHandler(__attribute__((unused))int signal_number)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
