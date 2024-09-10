#include "hop.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define RED "\033[31m"
#define RESET "\033[0m"
#define MAX_PATH 256

// Function to resolve special symbols and return the absolute path
char *resolve_path(char *input, char *home_dir, char *prev_dir)
{
    char *path = (char *)malloc(MAX_PATH * sizeof(char));
    if (strncmp(input, "~/", 2) == 0)
    {
        // If the input starts with "~/", replace it with the home directory
        snprintf(path, MAX_PATH, "%s%s", home_dir, input + 1);
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
            printf("no previous dir existx");
            strcpy(path, home_dir); // Default to home directory if no previous directory
        }
    }
    else if (input[0] == '/')
    {
        // Absolute path
        strcpy(path, input);
    }
    else if (strcmp(input, ".") == 0)
    {
        // Current directory
        if (getcwd(path, MAX_PATH) == NULL)
        {
            perror("getcwd() error");
        }
    }
    else if (strcmp(input, "..") == 0)
    {
        // Parent directory
        if (getcwd(path, MAX_PATH) != NULL)
        {
            char *last_slash = strrchr(path, '/');
            if (last_slash != NULL && last_slash != path) // Ensure we don't remove the leading /
            {
                *last_slash = '\0';
            }
            else
            {
                strcpy(path, "/"); // If removing the last slash results in empty, make it root "/"
            }
        }
        else
        {
            perror("getcwd() error");
        }
    }
    else
    {
        // Relative path
        if (getcwd(path, MAX_PATH) != NULL)
        {
            snprintf(path + strlen(path), MAX_PATH - strlen(path), "/%s", input);
        }
        else
        {
            perror("getcwd() error");
        }
    }

    return path;
}

// Function to change directory and print the current directory
void hop(char *input, char *home_dir, char *prev_dir)
{
    // printf("%s why\n",input);
    // Get the current directory before changing
    char current_dir[MAX_PATH];
    if (getcwd(current_dir, MAX_PATH) == NULL)
    {
        perror("getcwd failed");
        return;
    }

    // Resolve the path
    char *path = resolve_path(input, home_dir, prev_dir);

    // Check if the directory exists
    if (access(path, F_OK) != 0)
    {
        perror("No such file or directory");
        free(path);
        return;
    }

    // Attempt to change the directory
    if (chdir(path) != 0)
    {
        perror("chdir failed");
        free(path);
        return;
    }

    // Update the previous directory
    strcpy(prev_dir, current_dir);

    // Update the PWD environment variable
    if (setenv("PWD", path, 1) != 0)
    {
        perror("setenv failed");
    }

    // Print the new current directory
    if (getcwd(current_dir, MAX_PATH) != NULL)
    {
        printf(RED "%s\n" RESET, current_dir);
    }
    else
    {
        perror("getcwd failed");
    }
    // printf("%s\n", home_dir);
    // printf("%s\n", prev_dir);

    free(path);
}
