#include "signals.h"


extern int process_count;


void handle_ping(char *pid_str, char *signal_num_str) {
    pid_t pid = atoi(pid_str);
    int signal_num = atoi(signal_num_str) % 32;

    // Check if the signal number is valid
    if (signal_num < 1) {
        fprintf(stderr, "Invalid signal number: %d\n", signal_num);
        return;
    }

    // Check if the process exists in the background_process_list
    int found = 0;
    for (int i = 0; i < process_count; i++) {
        if (background_process_list[i].process_id == pid) {
            // Send the specified signal to the process
            if (kill(pid, signal_num) == 0) {
                printf("Sent signal %d to process with PID %d\n", signal_num, pid);
            } else {
                perror("Failed to send signal");
            }
            found = 1;
            break;
        }
    }

    if (found==0) {
        printf("No such process found\n");
    }
}

void ctrlc_handler(int sig)
{
   
    if (foreground_process_pid.process_id != -1)
    {
        // If a foreground process exists, send SIGINT to it
        kill(foreground_process_pid.process_id, SIGINT);
        printf("\nForeground process with PID %d interrupted\n", foreground_process_pid.process_id);
        foreground_process_pid.process_id = -1; // Reset the foreground process PID after sending SIGINT
    }
    else
    {
        // perhaps i need to change something over here
        printf("\n");
        fflush(stdout);
    }
}

// Signal handler for SIGTSTP
void ctrlz_handler(int sig)
{
    if (foreground_process_pid.process_id != -1)
    {
        if (kill(foreground_process_pid.process_id, SIGTSTP) == -1)
        {
            perror("Failed to send SIGTSTP");
            return;
        }

        background_process_list[process_count].process_id = foreground_process_pid.process_id;
        strncpy(background_process_list[process_count].process_name, foreground_process_pid.process_name, 255);
        background_process_list[process_count].process_name[255] = '\0'; // Null-terminate string

        process_count++;

        printf("\nForeground process stopped\n");
    }
    else
    {
        printf("\nShell: No foreground process to interrupt\n");
        fflush(stdout);
    }
}

void ctrld_handler()
{
    printf("Ctrl+D detected. Terminating all ongoing processes...\n");

    for (int i = 0; i < process_count; i++)
    {
        if (kill(background_process_list[i].process_id, SIGKILL) == -1)
        {
            perror("Failed to kill background process");
        }
    }
    // Wait for all child processes to terminate

    // Now exit the terminal
    printf("All processes killed. Exiting terminal.\n");
    exit(0);
}

void sigchld_handler(int sig)
{
    int status;
    pid_t pid;
    // Loop to handle all terminated child processes
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        // Find the process in the background process list
        for (int i = 0; i < process_count; i++)
        {
            if (background_process_list[i].process_id == pid)
            {
                // Print termination message
                if (WIFEXITED(status))
                {
                    printf("Process '%s' with PID %d ended normally with exit status %d\n",
                           background_process_list[i].process_name, pid, WEXITSTATUS(status));
                }
                else if (WIFSIGNALED(status))
                {
                    printf("Process '%s' with PID %d ended abnormally due to signal %d\n",
                           background_process_list[i].process_name, pid, WTERMSIG(status));
                }

                // Remove the process from the background process list
                for (int j = i; j < process_count - 1; j++)
                {
                    background_process_list[j] = background_process_list[j + 1];
                }
                process_count--; // Reduce the count of background processes

                break;
            }
        }
    }
}
