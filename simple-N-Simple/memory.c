#include "shell.h"

/**
 * freeAndNull - Frees a pointer and sets it to NULL
 * @pointer: Address of the pointer to free
 *
 * Returns: 1 if freed, otherwise 0.
 */
int freeAndNull(void **pointer)
{
	if (pointer && *pointer)
	{
		free(*pointer);
		*pointer = NULL;
		return (1);
	}
	return (0);
}
