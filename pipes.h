#ifndef PIPES_H
#define PIPES_H

#include "main.h"
int handle_redirection_back(char *cmd);

void restore_io_back(int saved_stdin, int saved_stdout);

void execute_single_command_back(char *command, queue *q, int *flag, char *home_dir, char *prev_dir);
void execute_piped_commands_back(char **commands, queue *q, int *flag, char *home_dir, char *prev_dir);
void execute_terminal_back(char *s, queue *q, int *flag, char *home_dir, char *prev_dir);

#endif
