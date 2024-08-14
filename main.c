#include "display_requirement.h"
#include "proclore.h"
#include "input_requirement.h"
#include "log_commands.h"

#include <sys/types.h>
#include <string.h>
int main()
{
    char *home_dir;

    // Get the home directory from the HOME environment variable
    home_dir = getenv("HOME");
    if (home_dir == NULL)
    {
        perror("getenv failed");
        exit(EXIT_FAILURE);
    }
    int flag = 1;
    queue *q;
    q = create_queue(q);
    while (flag)
    {

        // Update and display the prompt
        prompt(home_dir);

        // Read user input ans store in input
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            perror("error in taking  input from the user");
            exit(EXIT_FAILURE);
        }
        // Remove the newline character, if present
        int size = strlen(input);
        input[size - 1] = '\0';

        char *temp = strtok(input, ";");
        char *arr[256];
        int count = 0;
        while (temp != NULL)
        {
            arr[count] = temp;
            temp = strtok(NULL, ";");
            count++;
        }
        for (int i = 0; i < count; i++)
        {
            // printf("%s",arr[i]);
            execute_terminal(arr[i], q, &flag);
        }
    }

    return 0;
}
