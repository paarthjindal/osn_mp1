#ifndef REVEAL_H
#define REVEAL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

char *resolve_path_reveal(char *input, char *home_dir, char *prev_dir);
void print_file_details(const char *path, const char *name);
void reveal(const char *path, int show_all, int long_format);




#endif