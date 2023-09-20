#include "shell.h"

/**
 * exit_shell - Exits the shell.
 * @info: Structure with potential arguments.
 *
 * Return: Exits with a given exit status (0) if info->argv[0] != "exit".
 */
int exit_shell(info_t *info)
{
	int exit_status;

	if (info->argv[1])  /* Check for an exit argument */
	{
		exit_status = string_to_int(info->argv[1]);
		if (exit_status == -1)
		{
			info->status = 2;
			print_error_message(info, "Illegal number: ");
			print_error_string(info->argv[1]);
			print_error_char('\n');
			return 1;
		}
		info->err_num = string_to_int(info->argv[1]);
		return -2;
	}
	info->err_num = -1;
	return -2;
}

/**
 * change_directory - Changes the current directory of the process.
 * @info: Structure with potential arguments.
 *
 * Return: Always 0.
 */
int change_directory(info_t *info)
{
	char *current_directory, *dir, buffer[1024];
	int chdir_result;

	current_directory = get_current_directory(buffer, 1024);
	if (!current_directory)
		print_string("TODO: >>get_current_directory failure message here<<\n");
	if (!info->argv[1])
	{
		dir = get_environment_variable(info, "HOME=");
		if (!dir)
			chdir_result = /* TODO: What should this be? */
				change_directory((dir = get_environment_variable(info, "PWD=")) ? dir : "/");
		else
			chdir_result = change_directory(dir);
	}
	else if (compare_strings(info->argv[1], "-") == 0)
	{
		if (!get_environment_variable(info, "OLDPWD="))
		{
			print_string(current_directory);
			print_char('\n');
			return 1;
		}
		print_string(get_environment_variable(info, "OLDPWD="));
		print_char('\n');
		chdir_result = /* TODO: What should this be? */
			change_directory((dir = get_environment_variable(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_result = change_directory(info->argv[1]);
	if (chdir_result == -1)
	{
		print_error_message(info, "can't cd to ");
		print_error_string(info->argv[1]);
		print_error_char('\n');
	}
	else
	{
		set_environment_variable(info, "OLDPWD", get_environment_variable(info, "PWD="));
		set_environment_variable(info, "PWD", get_current_directory(buffer, 1024));
	}
	return 0;
}

/**
 * show_help - Displays help information.
 * @info: Structure with potential arguments.
 *
 * Return: Always 0.
 */
int show_help(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	print_string("Help call works. Function not yet implemented\n");
	if (0)
		print_string(*arg_array); /* Temporary workaround to avoid unused variable warning */
	return 0;
}
