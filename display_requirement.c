#include "display_requirement.h"

#define x 256
#define y 256
#define z 256

#define RESET_COLOR "\033[0m"
#define USER_COLOR "\033[34m" // Blue
#define HOST_COLOR "\033[32m" // Green
#define DIR_COLOR "\033[33m"  // Yellow

void prompt(char *home_dir)
{
 
    char cwd[x];
    char *relative_dir;

    // Get current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        return;
    }
    // printf("mine current is%s\n",cwd);

    // Check if the current directory is inside the home directory
    if (strncmp(cwd, home_dir, strlen(home_dir)) == 0)
    {
        if (strcmp(cwd, home_dir) == 0)
        {
            relative_dir = "~";
        }
        else
        {
            relative_dir = cwd + strlen(home_dir);
        }
    }
    else
    {
        relative_dir = cwd;
    }

    // Get the username
    char username[x];
    if (getlogin_r(username, sizeof(username)) != 0)
    {
        perror("getlogin_r failed");
        exit(EXIT_FAILURE);
    }

    // Get the system name
    char systemname[y];
    if (gethostname(systemname, sizeof(systemname)) != 0)
    {
        perror("gethostname failed");
        exit(EXIT_FAILURE);
    }

    // Display the prompt with colors
    printf("%s<%s%s%s@%s%s%s:%s%s%s>%s ",
           RESET_COLOR,
           USER_COLOR, username,
           RESET_COLOR,
           HOST_COLOR, systemname,
           RESET_COLOR,
           DIR_COLOR, relative_dir,
           RESET_COLOR,
           RESET_COLOR);
}
