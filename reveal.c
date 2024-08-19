#include "reveal.h"
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#define MAX_PATH 256

char *resolve_path_reveal(char *input, char *home_dir, char *prev_dir)
{
    char *path = (char *)malloc(MAX_PATH * sizeof(char));
    char temp_dir[256];
    if (getcwd(temp_dir, sizeof(temp_dir)) == NULL)
    {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }

    if (strcmp(input, "~") == 0)
    {
        // Home directory
        strcpy(path, home_dir);
    }
    else if(strcmp(input,".")==0)
    {
        strcpy(path,temp_dir);
    }
    else if (strcmp(input, "-") == 0)
    {
        // Previous directory
        strcpy(path, prev_dir);
    }
    else if (input[0] == '/')
    {
        // Absolute path
        strcpy(path, input);
    }
    else
    {
        // Relative path
        snprintf(path, MAX_PATH, "%s/%s", home_dir, input);
    }

    return path;
}

// Function to print file details when the -l flag is used
// Includes permissions, number of links, owner, group, size, and modification time
void print_file_details(const char *path, const char *name)
{
    struct stat file_stat;
    char full_path[MAX_PATH];
    snprintf(full_path, MAX_PATH, "%s/%s", path, name);

    if (stat(full_path, &file_stat) == 0)
    {
        // Print file type and permissions
        printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
        printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
        printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
        printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
        printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
        printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
        printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
        printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
        printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
        printf((file_stat.st_mode & S_IXOTH) ? "x " : "- ");

        // Print number of links
        printf("%ld ", file_stat.st_nlink);

        // Print owner and group
        printf("%s ", getpwuid(file_stat.st_uid)->pw_name);
        printf("%s ", getgrgid(file_stat.st_gid)->gr_name);

        // Print size
        printf("%5ld ", file_stat.st_size);

        // Print last modification time
        char time_str[20];
        strftime(time_str, 20, "%b %d %H:%M", localtime(&file_stat.st_mtime));
        printf("%s ", time_str);

        // Print file name with color coding
        if (S_ISDIR(file_stat.st_mode))
        {
            printf("\033[1;34m%s\033[0m\n", name); // Blue for directories
        }
        else if (file_stat.st_mode & S_IXUSR)
        {
            printf("\033[1;32m%s\033[0m\n", name); // Green for executables
        }
        else
        {
            printf("%s\n", name); // White for regular files
        }
    }
}

// Function to list files and directories
// Supports lexicographic sorting, -a for hidden files, and -l for detailed info
void reveal(const char *path, int show_all, int long_format)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        perror("opendir failed");
        return;
    }

    struct dirent *entry;
    char **files = NULL;
    int count = 0;

    // Read directory contents and store filenames in an array
    while ((entry = readdir(dir)) != NULL)
    {
        // Skip hidden files if -a flag is not present
        if (!show_all && entry->d_name[0] == '.')
        {
            continue;
        }

        files = realloc(files, sizeof(char *) * (count + 1));
        files[count] = strdup(entry->d_name);
        count++;
    }
    closedir(dir);

    // Sort files lexicographically
    qsort(files, count, sizeof(char *), (int (*)(const void *, const void *))strcmp);

    // Print files or directories
    for (int i = 0; i < count; i++)
    {
        if (long_format)
        {
            // Print detailed information if -l flag is present
            print_file_details(path, files[i]);
        }
        else
        {
            // Print just the name otherwise
            printf("%s\n", files[i]);
        }
        free(files[i]);
    }
    free(files);
}
