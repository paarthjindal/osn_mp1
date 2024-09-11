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
        }
        else
        {
            perror("Failed to open status file");
        }
        return;
    }
    fclose(status_file);
    // Attach the process to the terminal
    // Attempt to set the process group ID to the given PID
    if (setpgid(pid, pid) == -1)
    {
        perror("Failed to set process group ID");
        return;
    }
    // Wait for the process to finish or be stopped

    int status;
    if (waitpid(pid, &status, WUNTRACED) == -1)
    {
        perror("Failed to wait for process");
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