#include "shell.h"

/**
 * runShell - Main shell loop
 * @info: Struct for parameter and return information
 * @av: Argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int runShell(info_t *info, char **av)
{
	ssize_t bytesRead = 0;
	int builtinReturn = 0;

	while (bytesRead != -1 && builtinReturn != -2)
	{
		resetInfo(info);
		if (isInteractive(info))
			printPrompt("$ ");
		else
			printErrorPrompt();
		bytesRead = getInput(info);
		if (bytesRead != -1)
		{
			setCommandInfo(info, av);
			builtinReturn = findAndExecuteBuiltin(info);
			if (builtinReturn == -1)
				findAndExecuteCommand(info);
		}
		else if (isInteractive(info))
			printNewLine();
		freeInfo(info, 0);
	}
	writeHistory(info);
	freeInfo(info, 1);
	if (!isInteractive(info) && info->status)
		exit(info->status);
	if (builtinReturn == -2)
	{
		if (info->errorNumber == -1)
			exit(info->status);
		exit(info->errorNumber);
	}
	return (builtinReturn);
}

/**
 * findAndExecuteBuiltin - Finds and executes a builtin command
 * @info: Struct for parameter and return information
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int findAndExecuteBuiltin(info_t *info)
{
	int i, builtinReturn = -1;
	builtinTable builtins[] = {
		{"exit", myExit},
		{"env", myEnv},
		{"help", myHelp},
		{"history", myHistory},
		{"setenv", mySetenv},
		{"unsetenv", myUnsetenv},
		{"cd", myCd},
		{"alias", myAlias},
		{NULL, NULL}
	};

	for (i = 0; builtins[i].type; i++)
		if (compareCommand(info->argv[0], builtins[i].type) == 0)
		{
			info->lineCount++;
			builtinReturn = builtins[i].func(info);
			break;
		}
	return (builtinReturn);
}

/**
 * findAndExecuteCommand - Finds and executes a command in PATH
 * @info: Struct for parameter and return information
 *
 * Return: void
 */
void findAndExecuteCommand(info_t *info)
{
	char *path = NULL;
	int i, argCount;

	info->path = info->argv[0];
	if (info->lineCountFlag == 1)
	{
		info->lineCount++;
		info->lineCountFlag = 0;
	}
	for (i = 0, argCount = 0; info->arg[i]; i++)
		if (!isDelimiter(info->arg[i], " \t\n"))
			argCount++;
	if (!argCount)
		return;

	path = findCommandInPath(info, getEnvironmentVariable(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		forkAndExecuteCommand(info);
	}
	else
	{
		if ((isInteractive(info) || getEnvironmentVariable(info, "PATH=")
			|| info->argv[0][0] == '/') && isCommand(info, info->argv[0]))
			forkAndExecuteCommand(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			printError(info, "not found\n");
		}
	}
}

/**
 * forkAndExecuteCommand - Forks and executes a command
 * @info: Struct for parameter and return information
 *
 * Return: void
 */
void forkAndExecuteCommand(info_t *info)
{
	pid_t childPid;

	childPid = fork();
	if (childPid == -1)
	{
		printError(info, "Error:");
		perror("");
		return;
	}
	if (childPid == 0)
	{
		if (executeCommand(info->path, info->argv, getEnvironment(info)) == -1)
		{
			freeInfo(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		waitForChild(info);
		if (childExitedNormally(info))
		{
			info->status = getChildExitStatus(info);
			if (info->status == 126)
				printError(info, "Permission denied\n");
		}
	}
}
