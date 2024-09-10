#ifndef MAIN_H
#define MAIN_H
#define _XOPEN_SOURCE 700


#include "display_requirement.h"
#include "proclore.h"
#include "input_requirement.h"
#include "log_commands.h"
#include "pipes.h"
#include "i_o_redirection.h"
#include "redirection_along_with_pipes.h"
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "activites.h"
#include "log_commands.h"
#include "signals.h"
#include <errno.h>

typedef struct back_proc_list
{
    char process_name[256];
    pid_t process_id;
    int is_running; // 1 for Running, 0 for Stopped according to convention 
} back_proc_list;

typedef struct fore_proc_list
{
    char process_name[256];
    pid_t process_id;
}fore_process_list;

extern back_proc_list background_process_list[256];

extern fore_process_list foreground_process_pid;


#endif