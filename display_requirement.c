#include "display_requirement.h"

#define x 256
#define y 256
#define z 256

#define RESET_COLOR "\033[0m"
#define USER_COLOR "\033[34m"   // Blue
#define HOST_COLOR "\033[32m"   // Green
#define DIR_COLOR "\033[33m"    // Yellow

void prompt(char *home_dir)
{
    char username[x];
    char systemname[y];
    char current_dir[z];
    char prompt[x + y + z];

    // Get the username
    if (getlogin_r(username, sizeof(username)) != 0)
    {
        perror("getlogin_r failed");
        exit(EXIT_FAILURE);
    }

    // Get the system name
    if (gethostname(systemname, sizeof(systemname)) != 0)
    {
        perror("gethostname failed");
        exit(EXIT_FAILURE);
    }

    // Get the current directory
    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        perror("getcwd failed");
        exit(EXIT_FAILURE);
    }

    // Check if the current directory is inside the home directory
    char *relative_path = current_dir;
    if (strncmp(current_dir, home_dir, strlen(home_dir)) == 0)
    {
        // Replace the home directory part with '~'
        relative_path = current_dir + strlen(home_dir);
        if (*relative_path == '/')
        {
            relative_path++;  // Skip the '/' after home_dir
        }
    }

    // Create a colored prompt
    snprintf(prompt, sizeof(prompt),
        "<" USER_COLOR "%s" RESET_COLOR "@" HOST_COLOR "%s" RESET_COLOR ":"
        DIR_COLOR "%s" RESET_COLOR "> ",
        username, systemname, (relative_path[0] == '\0') ? "~" : relative_path);

    // Display the prompt
    printf("%s", prompt);
}
