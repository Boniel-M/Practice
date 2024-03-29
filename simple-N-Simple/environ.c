#include "shell.h"

/**
 * display_environment - Prints the current environment.
 * @info: Structure with potential arguments.
 * Return: Always 0
 */
int display_environment(info_t *info)
{
	print_string_list(info->env);
	return 0;
}

/**
 * get_environment_variable - Gets the value of an environment variable.
 * @info: Structure with potential arguments.
 * @name: Name of the environment variable.
 * Return: The value of the environment variable.
 */
char *get_environment_variable(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *value;

	while (node)
	{
		value = starts_with(node->str, name);
		if (value && *value)
			return value;
		node = node->next;
	}
	return NULL;
}

/**
 * set_environment_variable - Initializes a new environment variable or modifies an existing one.
 * @info: Structure with potential arguments.
 * Return: Always 0
 */
int set_environment_variable(info_t *info)
{
	if (info->argc != 3)
	{
		print_error("Incorrect number of arguments\n");
		return 1;
	}
	if (update_environment_variable(info, info->argv[1], info->argv[2]))
		return 0;
	return 1;
}

/**
 * unset_environment_variable - Removes an environment variable.
 * @info: Structure with potential arguments.
 * Return: Always 0
 */
int unset_environment_variable(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		print_error("Too few arguments.\n");
		return 1;
	}
	for (i = 1; i <= info->argc; i++)
		remove_environment_variable(info, info->argv[i]);

	return 0;
}

/**
 * populate_environment_list - Populates the environment linked list.
 * @info: Structure with potential arguments.
 * Return: Always 0
 */
int populate_environment_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	info->env = node;
	return 0;
}
