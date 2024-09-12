#include "fg_and_bg.h"

extern int process_count;

void fg_command(pid_t pid)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    // Check if the status file exists
    FILE *status_file = fopen(path, "r");
    if (status_file == NULL)
    {
        if (errno == ENOENT)
        {
            // Process does not exist
            printf("No such process found\n");
            return;
        }
        else
        {
            perror("Failed to open status file");
        }
        return;
    }
    fclose(status_file);
    // Bring the process to the foreground by assigning it terminal control

    // Send a SIGCONT signal to the process to continue if it was stopped
    if (kill(pid, SIGCONT) == -1)
    {
        perror("Failed to send SIGCONT");
        return;
    }

    if (tcsetpgrp(STDIN_FILENO, getpgid(pid)) == -1)
    {
        perror("Failed to set terminal control to the process");
        return;
    }

    // Send a SIGCONT signal to the process to continue if it was stopped
    if (kill(pid, SIGCONT) == -1)
    {
        perror("Failed to send SIGCONT");
        return;
    }

    // Wait for the process to finish or be stopped
    int status;
    // if (waitpid(pid, &status, WUNTRACED) == -1)
    // {
    //     perror("Failed to wait for process");
    //     return;
    // }
    while (waitpid(pid, &status, WUNTRACED) == -1 && errno == EINTR)
    {
    }

    // Restore terminal control to the shell
    if (tcsetpgrp(STDIN_FILENO, getpgid(getpid())) == -1)
    {
        perror("Failed to restore terminal control to the shell");
        return;
    }
}

void bg_command(pid_t pid)
{
    // Send SIGCONT to the process to continue in background
    if (kill(pid, SIGCONT) == -1)
    {
        perror("No such process found");
        return;
    }

    printf("Process [%d] resumed in background\n", pid);
}