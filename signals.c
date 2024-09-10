#include "signals.h"


extern int process_count;


void handle_ping(char *pid_str, char *signal_num_str) {
    pid_t pid = atoi(pid_str);
    int signal_num = atoi(signal_num_str) % 32;
    int signal = (signal_num == 0) ? SIGKILL : signal_num;  // Default to SIGKILL for signal number 0

    // Check if the process exists in the background_process_list
    int found = 0;
    for (int i = 0; i < process_count; i++) {
        if (background_process_list[i].process_id == pid) {
            if (kill(pid, signal) == 0) {
                printf("Sent signal %d to process with PID %d\n", signal, pid);
            } else {
                perror("Failed to send signal");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No such process found\n");
    }
}