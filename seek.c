#include "seek.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> // For basename function (POSIX)


// i am assumming that while printing i am printing the whole path instead of mentioned in doc that cwd should be replaced by ./
// cause according to me both almost means the same thinng so should not be an issue

#define MAX_PATH 1024

char *resolve_path_seek(char *input, char *home_dir, char *prev_dir)
{
    
    char *path = (char *)malloc(MAX_PATH * sizeof(char)); // Dynamically allocate memory
    if (strncmp(input, "~/", 2) == 0)
    {
        snprintf(path, 1024, "%s%s", home_dir, input + 1);
    }
    else if (strcmp(input, "~") == 0)
    {
        strcpy(path, home_dir);
    }
    else if (strcmp(input, "-") == 0)
    {
        if (prev_dir[0] != '\0')
        {
            strcpy(path, prev_dir);
        }
        else
        {
            printf("hop: no previous directory\n");
            strcpy(path, home_dir);
        }
    }
    else if (input[0] == '/')
    {
        strcpy(path, input);
    }
    else if (strcmp(input, ".") == 0)
    {
        if (getcwd(path, 1024) == NULL)
        {
            perror("getcwd() error");
        }
    }
    else if (strcmp(input, "..") == 0)
    {
        if (getcwd(path, MAX_PATH) != NULL)
        {
            char *last_slash = strrchr(path, '/');
            if (last_slash != NULL && last_slash != path)
            {
                *last_slash = '\0';
            }
            else
            {
                strcpy(path, "/");
            }
        }
        else
        {
            perror("getcwd() error");
        }
        
    }
    else
    {
        if (getcwd(path, MAX_PATH) != NULL)
        {
            snprintf(path + strlen(path), MAX_PATH - strlen(path), "/%s", input);
        }
        else
        {
            perror("getcwd() error");
        }
    }
    // printf("%s",path);
    return path;
}

void print_with_color(const char *path, const char *name, int is_directory)
{
    if (is_directory)
    {
        printf("\033[1;34m%s/%s\033[0m\n", path, name); // Blue for directories
    }
    else
    {
        printf("\033[1;32m%s/%s\033[0m\n", path, name); // Green for files
    }
}

int search_directory(const char *resolved_path_seek, const char *seek_name, int a, int b, int c, char *found_path)
{
    DIR *dir = opendir(resolved_path_seek);
    if (!dir)
    {
        // printf("being invalid\n");
        // printf("No such directory exists\n");
        return 0;
    }

    struct dirent *entry;
    int match_count = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char full_path[MAX_PATH];
        if (strlen(resolved_path_seek) + strlen(entry->d_name) + 2 > MAX_PATH)
        {
            fprintf(stderr, "Path too long\n");
            continue;
        }
        snprintf(full_path, MAX_PATH, "%s/%s", resolved_path_seek, entry->d_name);

        struct stat file_stat;
        if (stat(full_path, &file_stat) != 0)
        {
            perror("stat");
            continue;
        }

        int is_directory = S_ISDIR(file_stat.st_mode);
        int is_regular_file = S_ISREG(file_stat.st_mode);

        // Extract base name (without extension)
        char *dot_position = strrchr(entry->d_name, '.');
        size_t base_name_length = dot_position ? (size_t)(dot_position - entry->d_name) : strlen(entry->d_name);
        if (base_name_length >= MAX_PATH)
        {
            base_name_length = MAX_PATH - 1;
        }
        char base_name[MAX_PATH];
        strncpy(base_name, entry->d_name, base_name_length);
        base_name[base_name_length] = '\0'; // Null-terminate the base name

        // Compare the base name with seek_name
        if (strcmp(base_name, seek_name) == 0)
        {
            if ((a && is_directory) || (b && is_regular_file) || (!a && !b))
            {
                match_count++;
                strcpy(found_path, full_path);
                print_with_color(resolved_path_seek, entry->d_name, is_directory);
            }
        }

        if (is_directory)
        {
            match_count += search_directory(full_path, seek_name, a, b, c, found_path);
        }
    }

    closedir(dir);
    return match_count;
}

void seek(char *resolved_path_seek, char *seek_name, int a, int b, int c)
{
    char found_path[MAX_PATH];
    if(a<0)
    {
        printf("error");
        return;
    }
    if(b<0)
    {
        printf("error");
        return;
    }
    if(c<0)
    {
        printf("error");
        return;
    }
    // printf("%s\n", resolved_path_seek);

    int match_count = search_directory(resolved_path_seek, seek_name, a, b, c, found_path);
    // printf("what \n");
    printf("%d\n", match_count);
    if (match_count == 0)
    {
        printf("No Match Found!\n");
        return;
    }

    if (c == 1)
    {
        if (match_count == 1)
        {
            struct stat file_stat;
            stat(found_path, &file_stat);

            if (S_ISDIR(file_stat.st_mode))
            {
                if (access(found_path, X_OK) == 0)
                {
                    if (chdir(found_path) != 0)
                    {
                        perror("chdir failed");
                    }
                }
                else
                {
                    printf("Missing permissions for task!\n");
                }
            }
            else if (S_ISREG(file_stat.st_mode))
            {
                if (access(found_path, R_OK) == 0)
                {
                    // over here i need to change a bit what to do for e=1 and one file case 
                    printf("File found: %s\n", found_path);
                }
                else
                {
                    printf("Missing permissions for task!\n");
                }
            }
        }
        else
        {
            printf("No of matched files/directory more than one.\n");
        }
    }
}
