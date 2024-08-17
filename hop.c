#include "hop.h"

#define MAX_PATH 256

// Function to resolve special symbols and return the absolute path
char *resolve_path(char *input, char *home_dir)
{
    char *path = (char *)malloc(MAX_PATH * sizeof(char));

    if (strcmp(input, "~") == 0)
    {
        strcpy(path, home_dir);
    }
    else if (strcmp(input, "-") == 0)
    {
        // Implement functionality to change to the previous directory if needed
        // For now, let's assume it behaves like ~
        strcpy(path, home_dir);
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

// Function to change directory and print the current directory
void hop(char *input, char *home_dir,char * prev_dir)
{
    char *path = resolve_path(input, home_dir);

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

    // Update the PWD environment variable (its indeed important)
    if (setenv("PWD", path, 1) != 0)
    {
        perror("setenv failed");
    }

    // Print the new current directory
    if (getcwd(home_dir, MAX_PATH) != NULL)
    {
        printf("Directory changed to: %s\n", home_dir);
    }
    else
    {
        perror("getcwd failed");
    }

    free(path);
}
