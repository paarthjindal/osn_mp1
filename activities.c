#include "activites.h"

extern int process_count;
int compare_processes(const void *a, const void *b)
{
    back_proc_list *procA = (back_proc_list *)a;
    back_proc_list *procB = (back_proc_list *)b;
    return strcmp(procA->process_name, procB->process_name);
}

void print_activities()
{
    // Sort the background processes lexicographically by command name
    qsort(background_process_list, process_count, sizeof(back_proc_list), compare_processes);

    // Iterate through the background processes and check their actual status
    for (int i = 0; i < process_count; i++)
    {
        pid_t pid = background_process_list[i].process_id;
        char path[256];
        sprintf(path, "/proc/%d/status", pid);
        
        FILE *status_file = fopen(path, "r");
        if (status_file == NULL)
        {
            // Process no longer exists
            printf("[%d] : %s - Exited\n", pid, background_process_list[i].process_name);
            continue;
        }

        char line[256];
        char process_status[3] = "";
        int process_group = 0;
        unsigned long virtual_memory = 0;
        int foreground_group = tcgetpgrp(STDOUT_FILENO); // Foreground process group ID

        while (fgets(line, sizeof(line), status_file))
        {
            // Process status (e.g., R, S, Z, T)
            if (strncmp(line, "State:", 6) == 0)
            {
                sscanf(line, "State: %s", process_status);
            }
        }
        fclose(status_file);

        // Check if the process is running or stopped
        char *status_description;
        if (strcmp(process_status, "T") == 0)
        {
            status_description = "Stopped";
        }
        else if (strcmp(process_status, "R") == 0 || strcmp(process_status, "S") == 0)
        {
            status_description = "Running";
        }
        else if (strcmp(process_status, "Z") == 0)
        {
            status_description = "Zombie";
        }
        else
        {
            status_description = "Unknown";
        }

        // Print the process status
        printf("[%d] : %s - %s\n", pid, background_process_list[i].process_name, status_description);
    }
}
