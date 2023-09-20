#include "shell.h"

/**
 **_setMemory - Fills memory with a constant byte
 *@destination: Pointer to the memory area
 *@value: The byte to fill *destination with
 *@size: The number of bytes to be filled
 *Return: (destination) A pointer to the memory area destination
 */
char *_setMemory(char *destination, char value, unsigned int size)
{
	unsigned int i;

	for (i = 0; i < size; i++)
		destination[i] = value;
	return (destination);
}

/**
 * freeStringArray - Frees an array of strings
 * @strArray: Array of strings
 */
void freeStringArray(char **strArray)
{
	char **array = strArray;

	if (!strArray)
		return;
	while (*strArray)
		free(*strArray++);
	free(array);
}

/**
 * reallocateMemory - Reallocates a block of memory
 * @pointer: Pointer to the previous malloc'ated block
 * @oldSize: Byte size of the previous block
 * @newSize: Byte size of the new block
 *
 * Return: Pointer to the reallocated block
 */
void *reallocateMemory(void *pointer, unsigned int oldSize, unsigned int newSize)
{
	char *newPointer;

	if (!pointer)
		return (malloc(newSize));
	if (!newSize)
		return (free(pointer), NULL);
	if (newSize == oldSize)
		return (pointer);

	newPointer = malloc(newSize);
	if (!newPointer)
		return (NULL);

	oldSize = oldSize < newSize ? oldSize : newSize;
	while (oldSize--)
		newPointer[oldSize] = ((char *)pointer)[oldSize];
	free(pointer);
	return (newPointer);
}
