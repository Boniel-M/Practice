#include "shell.h"

/**
 * get_environment_strings - Returns a copy of the environment strings.
 * @info: Structure containing potential arguments.
 * Return: A copy of the environment strings.
 */
char **get_environment_strings(info_t *info)
{
	if (!info->environment || info->environment_changed)
	{
		info->environment = list_to_strings(info->env);
		info->environment_changed = 0;
	}

	return info->environment;
}

/**
 * unset_environment_variable - Remove an environment variable.
 * @info: Structure containing potential arguments.
 * @var: The environment variable to remove.
 * Return: 1 on successful deletion, 0 otherwise.
 */
int unset_environment_variable(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t index = 0;
	char *p;

	if (!node || !var)
		return 0;

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->environment_changed = delete_node_at_index(&(info->env), index);
			index = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		index++;
	}
	return info->environment_changed;
}

/**
 * set_environment_variable - Initialize a new environment variable or modify an existing one.
 * @info: Structure containing potential arguments.
 * @var: The environment variable name.
 * @value: The environment variable value.
 * Return: Always 0.
 */
int set_environment_variable(info_t *info, char *var, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return 0;

	buffer = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buffer)
		return 1;
	_strcpy(buffer, var);
	_strcat(buffer, "=");
	_strcat(buffer, value);
	node = info->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buffer;
			info->environment_changed = 1;
			return 0;
		}
		node = node->next;
	}
	add_node_end(&(info->env), buffer, 0);
	free(buffer);
	info->environment_changed = 1;
	return 0;
}
