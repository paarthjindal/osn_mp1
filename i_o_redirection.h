#ifndef I_O_REDIRECTION_H
#define I_O_REDIRECTION_H


#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
int handle_redirection(char *cmd);


#endif
