#include "shell.h"

/**
 * initialize_info - Initializes the info_t struct.
 * @info: Pointer to the info_t struct.
 */
void initialize_info(info_t *info)
{
	info->arg_str = NULL;
	info->argv_str = NULL;
	info->path_str = NULL;
	info->argc = 0;
}

/**
 * fill_info - Fills the info_t struct with data.
 * @info: Pointer to the info_t struct.
 * @argv: The argument vector.
 */
void fill_info(info_t *info, char **argv)
{
	int i = 0;

	info->program_name = argv[0];
	if (info->arg_str)
	{
		info->argv_str = string_to_words(info->arg_str, " \t");
		if (!info->argv_str)
		{
			info->argv_str = malloc(sizeof(char *) * 2);
			if (info->argv_str)
			{
				info->argv_str[0] = _strdup(info->arg_str);
				info->argv_str[1] = NULL;
			}
		}
		for (i = 0; info->argv_str && info->argv_str[i]; i++)
			;
		info->argc = i;

		replace_aliases(info);
		replace_variables(info);
	}
}

/**
 * free_info - Frees the fields of the info_t struct.
 * @info: Pointer to the info_t struct.
 * @free_all: True if freeing all fields.
 */
void free_info(info_t *info, int free_all)
{
	ffree(info->argv_str);
	info->argv_str = NULL;
	info->path_str = NULL;
	if (free_all)
	{
		if (!info->cmd_buffer)
			free(info->arg_str);
		if (info->environment)
			free_list(&(info->environment));
		if (info->history)
			free_list(&(info->history));
		if (info->aliases)
			free_list(&(info->aliases));
		ffree(info->environment_strings);
		info->environment_strings = NULL;
		bfree((void **)info->cmd_buffer);
		if (info->read_file_descriptor > 2)
			close(info->read_file_descriptor);
		_putchar(BUF_FLUSH);
	}
}
