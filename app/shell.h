#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1000
#define MAX_ARGUMENTS_PER_COMMAND 40
#define ASCII_OFFSET_FOR_NUMBERS 48
#define MAX_BUILTINS_SIZE 20
#define SPACE ' '
#define ENTER '\n'
#define TERNARY_ZERO '\0'

typedef struct Command
{
  char *raw_command;
  char *args[MAX_ARGUMENTS_PER_COMMAND];
  size_t args_size;
  size_t raw_length;
} Command;

typedef struct Shell_Builtin
{
  char *values[4];
  int length;
} Shell_Builtin;

void remove_enter_char (Command *command);
void parse_to_args (Command *command);
Command *init_command_struct (char *input);
Shell_Builtin *init_shell_builtin_struct ();
void free_shell_builtins (Shell_Builtin *builtins);
void add_shell_builtin (char *value, char **builtins_arr);
/* this func print without formatting options*/
void print_raw_command (char **args_buf, size_t args_len, int begin_index);
/* return 0 if success or -1 for error*/
char *find_file_from_path_env (char *file_name, const char *path_env);
#endif
