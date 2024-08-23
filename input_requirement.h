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
#include "input_requirement.h"
#include "log_commands.h"
#include "hop.h"
#include "reveal.h"

void execute_terminal(char *s, queue *q, int *flag, char *home_dir, char *prev_dir);

void write_queue_to_file(queue *q, const char *filename, const char *home_dir);
void read_queue_from_file(queue *q, const char *filename, const char *home_dir);

#endif