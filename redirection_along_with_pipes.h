#ifndef REDIRECTION_ALONG_WITH_PIPES_H
#define REDIRECTION_ALONG_WITH_PIPES_H


#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

void handle_io_redirection_and_piping(char *command, int is_background, queue *q, int *flag, char *home_dir, char *prev_dir);

#endif
