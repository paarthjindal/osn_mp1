#ifndef NEONATE_H
#define NEONATE_H

#include "main.h"
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
int get_most_recent_pid() ;
int kbhit(void) ;
void neonate(int interval) ;


#endif
