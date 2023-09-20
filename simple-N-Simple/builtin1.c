#include "shell.h"

/**
 * display_history - Displays the command history with line numbers.
 * @info: Structure with potential arguments.
 * Return: Always 0.
 */
int display_history(info_t *info)
{
	print_list(info->history);
	return 0;
}

/**
 * remove_alias - Removes an alias assignment.
 * @info: Parameter struct.
 * @alias_str: The alias string to remove.
 * Return: 0 on success, 1 on error.
 */
int remove_alias(info_t *info, char *alias_str)
{
	char *equals_pos, saved_char;
	int result;

	equals_pos = _strchr(alias_str, '=');
	if (!equals_pos)
		return 1;

	saved_char = *equals_pos;
	*equals_pos = '\0';
	result = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, alias_str, -1)));
	*equals_pos = saved_char;

	return result;
}

/**
 * set_alias_assignment - Sets an alias assignment.
 * @info: Parameter struct.
 * @alias_str: The alias string to set.
 * Return: 0 on success, 1 on error.
 */
int set_alias_assignment(info_t *info, char *alias_str)
{
	char *equals_pos;

	equals_pos = _strchr(alias_str, '=');
	if (!equals_pos)
		return 1;

	if (!*++equals_pos)
		return remove_alias(info, alias_str);

	remove_alias(info, alias_str);
	return (add_node_end(&(info->alias), alias_str, 0) == NULL);
}

/**
 * print_alias_assignment - Prints an alias assignment.
 * @node: The alias node.
 * Return: 0 on success, 1 on error.
 */
int print_alias_assignment(list_t *node)
{
	char *equals_pos, *alias_name;

	if (node)
	{
		equals_pos = _strchr(node->str, '=');
		for (alias_name = node->str; alias_name <= equals_pos; alias_name++)
			_putchar(*alias_name);
		_putchar('\'');
		_puts(equals_pos + 1);
		_puts("'\n");
		return 0;
	}
	return 1;
}

/**
 * manage_aliases - Manages alias commands (mimics 'alias' built-in).
 * @info: Structure with potential arguments.
 * Return: Always 0.
 */
int manage_aliases(info_t *info)
{
	int i = 0;
	char *equals_pos = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias_assignment(node);
			node = node->next;
		}
		return 0;
	}

	for (i = 1; info->argv[i]; i++)
	{
		equals_pos = _strchr(info->argv[i], '=');
		if (equals_pos)
			set_alias_assignment(info, info->argv[i]);
		else
			print_alias_assignment(node_starts_with(info->alias, info->argv[i], '='));
	}

	return 0;
}
