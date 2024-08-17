#ifndef HOP_H
#define HOP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

char* resolve_path( char* input_path, char* home_dir);

void hop(char* input, char* home_dir,char * prev_dir);



#endif