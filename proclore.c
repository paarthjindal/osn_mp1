#include "proclore.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void describe_process(pid_t process_id)
{
    printf("PID: %d\n", process_id);
    
    // Construct the path to the /proc/[PID]/status file
    char path[256];
    sprintf(path, "/proc/%d/status", process_id);
    
    FILE *status_file = fopen(path, "r");
    if (status_file == NULL)
    {
        perror("Failed to open /proc/[PID]/status");
        return;
    }

    char line[256];
    char process_status[3] = "";
    int process_group = 0;
    unsigned long virtual_memory = 0;
    int foreground_group = tcgetpgrp(STDOUT_FILENO); // Foreground process group ID

    while (fgets(line, sizeof(line), status_file))
    {
        // Process status (e.g., R, S, Z)
        if (strncmp(line, "State:", 6) == 0)
        {
            sscanf(line, "State: %s", process_status);
        }
        // Process group ID
        if (strncmp(line, "Tgid:", 5) == 0)
        {
            sscanf(line, "Tgid: %d", &process_group);
        }
        // Virtual memory size
        if (strncmp(line, "VmSize:", 7) == 0)
        {
            sscanf(line, "VmSize: %lu", &virtual_memory);
        }
    }
    fclose(status_file);

    // Determine if the process is in the foreground or background
    char status_with_annotation[4] = "";
    if (strcmp(process_status, "R") == 0 || strcmp(process_status, "S") == 0)
    {
        if (process_group == foreground_group)
        {
            sprintf(status_with_annotation, "%s+", process_status); // Foreground process
        }
        else
        {
            strcpy(status_with_annotation, process_status); // Background process
        }
    }
    else
    {
        strcpy(status_with_annotation, process_status); // Other states (e.g., Z)
    }

    // Print Process Information
    printf("Process Status: %s\n", status_with_annotation);
    printf("Process Group ID: %d\n", process_group);
    printf("Virtual Memory Size: %lu KB\n", virtual_memory);

    // Get the executable path
    sprintf(path, "/proc/%d/exe", process_id);
    char exec_path[256];
    ssize_t len = readlink(path, exec_path, sizeof(exec_path) - 1);
    if (len == -1)
    {
        perror("Failed to read /proc/[PID]/exe");
        return;
    }
    exec_path[len] = '\0';
    printf("Executable Path: %s\n", exec_path);
}
