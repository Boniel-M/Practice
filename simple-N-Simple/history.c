#include "shell.h"

/**
 * get_history_file - Get the history file path.
 * @info: Parameter struct.
 *
 * Return: Allocated string containing the history file path.
 */
char *get_history_file(info_t *info)
{
	char *directory, *path;

	directory = _getenv(info, "HOME=");
	if (!directory)
		return (NULL);

	path = malloc(sizeof(char) * (_strlen(directory) + _strlen(HISTORY_FILE) + 2));
	if (!path)
		return (NULL);

	path[0] = 0;
	_strcpy(path, directory);
	_strcat(path, "/");
	_strcat(path, HISTORY_FILE);

	return (path);
}

/**
 * write_history - Create or append to the history file.
 * @info: Parameter struct.
 *
 * Return: 1 on success, -1 on failure.
 */
int write_history(info_t *info)
{
	ssize_t file_descriptor;
	char *file_name = get_history_file(info);
	list_t *node = NULL;

	if (!file_name)
		return (-1);

	file_descriptor = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(file_name);

	if (file_descriptor == -1)
		return (-1);

	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, file_descriptor);
		_putfd('\n', file_descriptor);
	}

	_putfd(BUFFER_FLUSH, file_descriptor);
	close(file_descriptor);
	return (1);
}

/**
 * read_history - Read history from a file.
 * @info: Parameter struct.
 *
 * Return: Number of history entries read, or 0 on failure.
 */
int read_history(info_t *info)
{
	int i, last = 0, line_count = 0;
	ssize_t file_descriptor, read_length, file_size = 0;
	struct stat stats;
	char *buffer = NULL, *file_name = get_history_file(info);

	if (!file_name)
		return (0);

	file_descriptor = open(file_name, O_RDONLY);
	free(file_name);

	if (file_descriptor == -1)
		return (0);

	if (!fstat(file_descriptor, &stats))
		file_size = stats.st_size;

	if (file_size < 2)
		return (0);

	buffer = malloc(sizeof(char) * (file_size + 1));
	if (!buffer)
		return (0);

	read_length = read(file_descriptor, buffer, file_size);
	buffer[file_size] = 0;

	if (read_length <= 0)
		return (free(buffer), 0);

	close(file_descriptor);

	for (i = 0; i < file_size; i++)
		if (buffer[i] == '\n')
		{
			buffer[i] = 0;
			build_history_list(info, buffer + last, line_count++);
			last = i + 1;
		}

	if (last != i)
		build_history_list(info, buffer + last, line_count++);

	free(buffer);
	info->history_count = line_count;

	while (info->history_count-- >= HISTORY_MAX)
		delete_node_at_index(&(info->history), 0);

	renumber_history(info);
	return (info->history_count);
}

/**
 * build_history_list - Add an entry to the history linked list.
 * @info: Parameter struct.
 * @buffer: Buffer containing the history entry.
 * @line_count: Line count (history count).
 *
 * Return: Always 0.
 */
int build_history_list(info_t *info, char *buffer, int line_count)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;

	add_node_end(&node, buffer, line_count);

	if (!info->history)
		info->history = node;

	return (0);
}

/**
 * renumber_history - Renumber the history linked list after changes.
 * @info: Parameter struct.
 *
 * Return: The new history count.
 */
int renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}

	return (info->history_count = i);
}
