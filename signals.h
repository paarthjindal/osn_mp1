#ifndef SIGNALS_H
#define SIGNALS_H


#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

void execute_piped_commands(char *commands[], int command_count);
#endif
