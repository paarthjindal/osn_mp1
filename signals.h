#ifndef SIGNALS_H
#define SIGNALS_H

#include "main.h"
void handle_ping(char *pid_str, char *signal_num_str) ;
void ctrlc_handler(int sig);
void ctrlz_handler(int sig);
void ctrld_handler();
void sigchld_handler(int sig);

#endif
