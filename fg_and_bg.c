#include "fg_and_bg.h"

extern int process_count;

void fg_command(pid_t pid)
{
    int found = 0;
    for (int i = 0; i < process_count; i++)
    {
        if (background_process_list[i].process_id == pid)
        {
            found = 1;

            // Check if the process exists
            if (kill(pid, 0) == -1)
            {
                perror("No such process");
                return;
            }

            // Bring the process to the foreground
            if (tcsetpgrp(STDIN_FILENO, pid) == -1)
            {
                perror("tcsetpgrp failed");
                return;
            }

            // Send SIGCONT to continue the stopped background process (if it was stopped)
            int status;
            if (waitpid(pid, &status, WNOHANG) == 0)
            { // WNOHANG checks if the process is not finished
                if (kill(pid, SIGCONT) == -1)
                {
                    perror("kill(SIGCONT) failed");
                    return;
                }
            }

            // Update the foreground process information
            foreground_process_pid.process_id = pid;
            strcpy(foreground_process_pid.process_name, background_process_list[i].process_name);

            // Wait for the foreground process to complete
            if (waitpid(pid, &status, WUNTRACED) == -1)
            {
                perror("waitpid failed");
                return;
            }

            // Check if the process was stopped or exited
            if (WIFSTOPPED(status))
            {
                printf("Process [%d] was stopped\n", pid);
            }
            else if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                printf("Process [%d] exited\n", pid);
                // Optionally, you can remove the process from background_process_list
            }

            // Restore terminal control to the shell
            if (tcsetpgrp(STDIN_FILENO, getpgrp()) == -1)
            {
                perror("tcsetpgrp restore failed");
            }

            // Clear the foreground process info
            foreground_process_pid.process_id = -1;
            printf("Brought process [%d] to foreground\n", pid);
            return;
        }
    }
    if (!found)
    {
        printf("No such process found\n");
    }
}

void bg_command(pid_t pid)
{
    int found = 0;
    for (int i = 0; i < process_count; i++)
    {
        if (background_process_list[i].process_id == pid)
        {
            found = 1;

            // Check if the process exists
            if (kill(pid, 0) == -1)
            {
                perror("No such process");
                return;
            }

            // Send SIGCONT to continue the stopped background process
            int status;
            if (waitpid(pid, &status, WNOHANG) == 0)
            { // Process is not finished
                if (kill(pid, SIGCONT) == -1)
                {
                    perror("kill(SIGCONT) failed");
                    return;
                }
                printf("Process [%d] '%s' is now running in the background\n", pid, background_process_list[i].process_name);
            }
            else
            {
                printf("Process [%d] is already running in the background\n", pid);
            }
            return;
        }
    }
    if (!found)
    {
        printf("No such process found\n");
    }
}
