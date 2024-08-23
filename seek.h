#ifndef SEEK_H
#define SEEK_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>


char *resolve_path_seek(char *input, char *home_dir, char *prev_dir);

void seek(char *resolved_path_seek,char * seek_name, int a, int b, int c);
void print_with_color(const char *path, const char *name, int is_directory);
int search_directory(const char *resolved_path_seek, const char *seek_name, int a, int b, int c, char *found_path);



#endif