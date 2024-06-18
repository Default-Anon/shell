#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Command *
init_command_struct (char *input)
{
  Command *command = (Command *)malloc (sizeof (*command));
  command->raw_command = input;
  remove_enter_char (command);
  parse_to_args (command);
  return command;
}

void
add_shell_builtin (char *value, char **arr_builtins)
{
  *arr_builtins = (char *)malloc (sizeof (char) * MAX_BUILTINS_SIZE);
  memset (*arr_builtins, TERNARY_ZERO, MAX_BUILTINS_SIZE);
  memcpy (*arr_builtins, value, strlen (value));
}
Shell_Builtin *
init_shell_builtin_struct ()
{
  Shell_Builtin *builtin = (Shell_Builtin *)malloc (sizeof (*builtin));
  add_shell_builtin ("echo", &builtin->values[0]);
  add_shell_builtin ("type", &builtin->values[1]);
  add_shell_builtin ("pwd", &builtin->values[2]);
  add_shell_builtin ("exit", &builtin->values[3]);
  builtin->length = 4;
  return builtin;
}

void
remove_enter_char (Command *command)
{
  char *enter_symbol = strchr (command->raw_command, ENTER);
  *enter_symbol = TERNARY_ZERO;
  command->raw_length = enter_symbol - command->raw_command;
}
void
parse_to_args (Command *command)
{
  char *temp_raw = command->raw_command;
  command->args_size = 0;
  char *symbol = NULL;
  while ((symbol = strchr (temp_raw, SPACE)) != NULL)
    {
      command->args[command->args_size] = temp_raw;
      *symbol = TERNARY_ZERO;
      temp_raw += (symbol - temp_raw) + 1;
      ++command->args_size;
    }
  if (command->args_size == 0)
    {
      command->args[command->args_size] = command->raw_command;
    }
  else
    {
      command->args[command->args_size] = temp_raw;
    }
}
void
print_raw_command (char **args_buf, size_t args_length, int begin_index)
{
  int i = begin_index;
  while (i <= args_length)
    {
      if (i == args_length)
        printf ("%s", args_buf[i]);
      else
        printf ("%s ", args_buf[i]);
      i++;
    }
}
void
free_shell_builtins (Shell_Builtin *builtins)
{
  for (int i = --builtins->length; i >= 0; i--)
    {
      free (builtins->values[i]);
    }
  free (builtins);
}

char *
find_file_from_path_env (char *file_name, const char *path_env)
{
  FILE *fdesk;
  size_t buf_sz = strlen (file_name) + strlen (path_env) + 2;
  char *buf = (char *)malloc (sizeof (char) * buf_sz);
  char env_path_buf[strlen (path_env) + 1];
  snprintf (env_path_buf, sizeof (env_path_buf), "%s", path_env);
  memset (buf, TERNARY_ZERO, buf_sz);
  char *token = strtok (env_path_buf, ":");
  while (token != NULL)
    {
      snprintf (buf, buf_sz, "%s/%s", token, file_name);
      fdesk = fopen (buf, "r");
      if (fdesk)
        return buf;
      memset (buf, TERNARY_ZERO, strlen (buf));
      token = strtok (NULL, ":");
    }
  return NULL;
}
