#include "shell.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main ()
{
  // Uncomment this block to pass the first stage
  setbuf (stdout, NULL);
  setbuf (stderr, NULL);
  char *env_path = getenv ("PATH");
  Command *command = NULL;
  Shell_Builtin *builtins = init_shell_builtin_struct ();
  char input[MAX_COMMAND_LENGTH];
  char *exe_path_if_exist = NULL;
  memset (input, TERNARY_ZERO, MAX_COMMAND_LENGTH);
  while (1)
    {
      printf ("$ ");
      fflush (stdout);
      // Wait for user input
      fgets (input, MAX_COMMAND_LENGTH, stdin);
      command = init_command_struct (input);
      if (strcmp (command->args[0], "exit") == 0)
        {
          if (command->args_size > 0)
            {
              int return_code = *command->args[1] - ASCII_OFFSET_FOR_NUMBERS;
              free_shell_builtins (builtins);
              free (command);
              return return_code;
            }
          return 0;
        }
      else if (strcmp (command->args[0], "echo") == 0)
        {
          print_raw_command (command->args, command->args_size, 1);
          printf ("\n");
        }
      else if (strcmp (command->args[0], "type") == 0)
        {
          int i;
          for (i = 0; i < builtins->length; i++)
            {
              if (strcmp (command->args[1], builtins->values[i]) == 0)
                {
                  print_raw_command (command->args, command->args_size, 1);
                  printf (" is a shell builtin\n");
                  i = 0;
                  break;
                }
            }
          exe_path_if_exist
              = find_file_from_path_env (command->args[1], env_path);
          if (exe_path_if_exist && i)
            {
              print_raw_command (command->args, command->args_size, 1);
              printf (" is %s\n", exe_path_if_exist);
            }

          if (i && exe_path_if_exist == NULL)
            {
              print_raw_command (command->args, command->args_size, 1);
              printf (": not found\n");
            }
          free (exe_path_if_exist);
        }
      else if ((exe_path_if_exist
                = find_file_from_path_env (command->args[0], env_path))
               != NULL)
        {
          pid_t process;
          process = fork ();
          if (process < 0)
            printf ("fork failed\n");
          else if (process == 0)
            execv (exe_path_if_exist, command->args);
          else
            wait (NULL);
        }
      else if (strcmp (command->args[0], "pwd") == 0)
        {
          getcwd (input, sizeof (input));
          printf ("%s\n", input);
        }
      else
        {
          print_raw_command (command->args, command->args_size, 0);
          printf (": command not found\n");
        }
      memset (input, TERNARY_ZERO, command->raw_length);
    }
  free (command);
  return 0;
}
