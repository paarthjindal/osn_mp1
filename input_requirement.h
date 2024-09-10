#ifndef INPUT_REQUIREMENT_H
#define INPUT_REQUIREMENT_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

#include <sys/types.h>
#include "display_requirement.h"
#include "proclore.h"
#include "log_commands.h"
#include "hop.h"
#include "reveal.h"
#include <fcntl.h>


void restore_io(int saved_stdin, int saved_stdout);
void execute_terminal(char *s, queue *q, int *flag, char *home_dir, char *prev_dir);
void execute_final_terminal(char *s, queue *q, int *flag, char *home_dir, char *prev_dir);
void execute_piped_commands(char **commands, queue *q, int *flag, char *home_dir, char *prev_dir);
// int handle_redirection(char *cmd);
void execute_single_command(char *command, queue *q, int *flag, char *home_dir, char *prev_dir);
void write_queue_to_file(queue *q, const char *filename, const char *home_dir);
void read_queue_from_file(queue *q, const char *filename, const char *home_dir);

#endif