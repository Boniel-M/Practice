#include "shell.h"

/**
 * list_length - Counts the number of nodes in a linked list.
 * @head: Pointer to the first node.
 *
 * Return: Size of the list.
 */
size_t list_length(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		head = head->next;
		count++;
	}
	return count;
}

/**
 * list_to_array - Converts a linked list of strings into an array of strings.
 * @head: Pointer to the first node.
 *
 * Return: Array of strings.
 */
char **list_to_array(list_t *head)
{
	list_t *node = head;
	size_t size = list_length(head);
	char **strings;
	char *str;
	size_t i;

	if (!head || !size)
		return NULL;

	strings = malloc(sizeof(char *) * (size + 1));
	if (!strings)
		return NULL;

	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (size_t j = 0; j < i; j++)
				free(strings[j]);
			free(strings);
			return NULL;
		}

		str = _strcpy(str, node->str);
		strings[i] = str;
	}
	strings[i] = NULL;
	return strings;
}

/**
 * print_list_elements - Prints all elements of a linked list.
 * @head: Pointer to the first node.
 *
 * Return: Size of the list.
 */
size_t print_list_elements(const list_t *head)
{
	size_t count = 0;

	while (head)
	{
		_puts(convert_number(head->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(head->str ? head->str : "(nil)");
		_puts("\n");
		head = head->next;
		count++;
	}
	return count;
}

/**
 * find_node_starts_with - Returns the node whose string starts with a given prefix.
 * @head: Pointer to the list head.
 * @prefix: String to match as a prefix.
 * @c: The next character after the prefix to match.
 *
 * Return: Matching node or NULL.
 */
list_t *find_node_starts_with(list_t *head, char *prefix, char c)
{
	char *p = NULL;

	while (head)
	{
		p = starts_with(head->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return head;
		head = head->next;
	}
	return NULL;
}

/**
 * get_node_index - Gets the index of a specific node in a linked list.
 * @head: Pointer to the list head.
 * @node: Pointer to the node.
 *
 * Return: Index of the node or -1 if not found.
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t index = 0;

	while (head)
	{
		if (head == node)
			return index;
		head = head->next;
		index++;
	}
	return -1;
}
