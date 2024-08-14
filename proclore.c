#include "proclore.h"

void describe_process(pid_t process_id)
{
    // printf("%d\n",process_id);
    printf("pid : %d\n", process_id);
    char path[256];
    sprintf(path, "/proc/%d/status", process_id);
    FILE *status_file = fopen(path, "r");
    if (status_file == NULL)
    {
        perror("Failed to open /proc/[PID]/status");
        return;
    }

    char line[256];
    char process_status[3];
    int process_group;
    unsigned long virtual_memory = 0;

    while (fgets(line, sizeof(line), status_file))
    {
        if (strncmp(line, "State:", 6) == 0)
        {
            sscanf(line, "State: %s", process_status);
        }
        if (strncmp(line, "Tgid:", 5) == 0)
        {
            sscanf(line, "Tgid: %d", &process_group);
        }
        if (strncmp(line, "VmSize:", 7) == 0)
        {
            sscanf(line, "VmSize: %lu", &virtual_memory);
        }
    }
    fclose(status_file);

    printf("Process Status: %s\n", process_status);
    printf("Process Group ID: %d\n", process_group);
    printf("Virtual Memory Size: %lu KB\n", virtual_memory);

    // 2. Get the executable path
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
