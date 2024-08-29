#include "display_requirement.h"
#include "proclore.h"
#include "input_requirement.h"
#include "log_commands.h"

#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

// Signal handler for SIGCHLD
void sigchld_handler(int sig)
{
    int status;
    pid_t pid;

    // Loop to handle all terminated child processes
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Process with PID %d ended normally with exit status %d\n", pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Process with PID %d ended abnormally due to signal %d\n", pid, WTERMSIG(status));
        }
    }
}

int main()
{
    // Set up the SIGCHLD signal handler
    // i have included signal.h library overe here but still it is showing me erorr in the following lines
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; // Restart interrupted system calls and don't call handler when children stop
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    char home_dir[256];
    char prev_dir[256] = "";
    if (getcwd(home_dir, sizeof(home_dir)) == NULL)
    {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
    char save_dir[256];
    if (getcwd(save_dir, sizeof(save_dir)) == NULL)
    {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
    int flag = 1;
    queue *q;
    q = create_queue(q);

    char *filename = "newfile.txt";
    read_queue_from_file(q, filename, save_dir);

    while (flag)
    {
        // Update and display the prompt
        prompt(save_dir);

        // Read user input and store in input
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            perror("error in taking input from the user");
            exit(EXIT_FAILURE);
        }
        int size = strlen(input);
        input[size - 1] = '\0';

        char save[256];
        strcpy(save, input);
        char delimiters[] = " \t";

        char *pa = strtok(save, delimiters);
        if (strcmp(pa, "log") != 0)
        {
            enqueue(q, input);
        }
        write_queue_to_file(q, filename, save_dir);

        char *temp = strtok(input, ";");
        char *arr[256];
        int count = 0;
        int background[256] = {0};

        while (temp != NULL)
        {
            while (*temp == ' ' || *temp == '\t')
                temp++;
            int len = strlen(temp);
            while (len > 0 && (temp[len - 1] == ' ' || temp[len - 1] == '\t'))
            {
                temp[--len] = '\0';
            }

            if (len > 0 && temp[len - 1] == '&')
            {
                background[count] = 1;
                temp[len - 1] = '\0';
            }

            arr[count] = temp;
            count++;

            temp = strtok(NULL, ";");
        }

        for (int i = 0; i < count; i++)
        {
            if (background[i] == 1)
            {
                pid_t back_process = fork();
                if (back_process < 0)
                {
                    perror("fork failed");
                    continue;
                }
                else if (back_process == 0)
                {
                    char *token = strtok(arr[i], " \t");
                    char *arr1[100];
                    int j = 0;
                    while (token != NULL && j < 99)
                    {
                        arr1[j] = token;
                        j++;
                        token = strtok(NULL, " \t");
                    }
                    arr1[j] = NULL;

                    if (execvp(arr1[0], arr1) < 0)
                    {
                        perror("Invalid Command");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // Parent process continues to the next command
                    printf("%d\n", back_process);
                }
            }
            else
            {
                execute_terminal(arr[i], q, &flag, home_dir, prev_dir);
            }
        }
    }

    write_queue_to_file(q, filename, save_dir);

    return 0;
}
