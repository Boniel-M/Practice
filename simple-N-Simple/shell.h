#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORMAL 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* for convert_number() */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096

extern char **environ;

/**
 * struct ListNode - singly linked list node
 * @number: integer value
 * @string: a string
 * @next: points to the next node
 */
typedef struct ListNode
{
	int number;
	char *string;
	struct ListNode *next;
} list_t;

/**
 *struct ShellInfo - contains pseudo-arguments to pass into a function,
 *		allowing a uniform prototype for a function pointer struct
 *@arg_line: a string generated from getline containing arguments
 *@arguments: an array of strings generated from arg_line
 *@command_path: a string path for the current command
 *@arg_count: the argument count
 *@error_count: the error count
 *@error_number: the error code for exit()
 *@count_line: if on count this line of input
 *@filename: the program filename
 *@environment_list: linked list local copy of environ
 *@history_list: the history node
 *@alias_list: the alias node
 *@custom_environment: custom modified copy of environ from LL environment_list
 *@environment_changed: 1 if environ was changed
 *@last_command_status: the return status of the last executed command
 *@command_buffer: address of a pointer to command buffer, 1 if chaining
 *@command_buffer_type: CMD_type ||, &&, ;
 *@read_fd: the file descriptor from which to read line input
 *@history_count: the history line number count
 */
typedef struct ShellInfo
{
	char *arg_line;
	char **arguments;
	char *command_path;
	int arg_count;
	unsigned int error_count;
	int error_number;
	int count_line;
	char *filename;
	list_t *environment_list;
	list_t *history_list;
	list_t *alias_list;
	char **custom_environment;
	int environment_changed;
	int last_command_status;

	char **command_buffer; /* pointer to cmd ; chain buffer, for memory management */
	int command_buffer_type; /* CMD_type ||, &&, ; */
	int read_fd;
	int history_count;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct BuiltIn - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct BuiltIn
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* toem_shell_loop.c */
int shell_loop(info_t *, char **);
int find_builtin(info_t *);
void find_command(info_t *);
void fork_command(info_t *);

/* toem_parser.c */
int is_executable(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_command_path(info_t *, char *);

/* toem_shell_main.c */
int main(int, char **);

/* toem_errors.c */
void error_puts(char *);
int error_putchar(char);
int put_fd(char, int);
int puts_fd(char *, int);

/* toem_string.c */
int string_length(char *);
int string_compare(char *, char *);
char *starts_with_string(const char *, const char *);
char *string_concatenate(char *, char *);

/* toem_string1.c */
char *string_copy(char *, char *);
char *string_duplicate(const char *);
void print_string(char *);
int put_character(char);

/* toem_exits.c */
char *string_copy_n(char *, char *, int);
char *string_concatenate_n(char *, char *, int);
char *string_character(const char *, char);

/* toem_tokenizer.c */
char **tokenize_string(char *, char *);
char **tokenize_string2(char *, char);

/* toem_realloc.c */
char *memory_set(char *, char, unsigned int);
void free_strings(char **);
void *reallocate_memory(void *, unsigned int, unsigned int);

/* toem_memory.c */
int batch_free(void **);

/* toem_atoi.c */
int is_interactive(info_t *);
int is_delimiter(char, char *);
int is_alpha(int);
int string_to_integer(char *);

/* toem_errors1.c */
int error_string_to_integer(char *);
void print_shell_error(info_t *, char *);
int print_decimal(int, int);
char *convert_integer(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int exit_shell(info_t *);
int change_directory(info_t *);
int show_help(info_t *);

/* toem_builtin1.c */
int show_history(info_t *);
int manage_alias(info_t *);

/* toem_getline.c */
ssize_t get_input_line(info_t *);
int get_line(info_t *, char **, size_t *);
void sigint_handler(int);

/* toem_getinfo.c */
void clear_shell_info(info_t *);
void set_shell_info(info_t *, char **);
void free_shell_info(info_t *, int);

/* toem_environment.c */
char *get_shell_environment(info_t *, const char *);
int show_shell_environment(info_t *);
int set_shell_environment(info_t *);
int unset_shell_environment(info_t *);
int populate_environment_list(info_t *);

/* toem_getenv.c */
char **get_environment(info_t *);
int unset_environment(info_t *, char *);
int set_environment(info_t *, char *, char *);

/* toem_history.c */
char *get_history_file_path(info_t *info);
int write_history_file(info_t *info);
int read_history_file(info_t *info);
int build_history_list(info_t *info, char *buffer, int line_count);
int renumber_history_list(info_t *info);

/* toem_lists.c */
list_t *add_list_node(list_t **, const char *, int);
list_t *add_list_node_end(list_t **, const char *, int);
size_t print_string_list(const list_t *);
int delete_list_node_at_index(list_t **, unsigned int);
void free_list_nodes(list_t **);

/* toem_lists1.c */
size_t list_length(const list_t *);
char **list_to_string_array(list_t *);
size_t print_list(const list_t *);
list_t *find_list_node_starts_with(list_t *, char *, char);
ssize_t get_list_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_command_chain(info_t *, char *, size_t *);
void check_command_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_variables(info_t *);
int replace_string_value(char **, char *);

#endif
