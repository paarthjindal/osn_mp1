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

    if (!found) {
        printf("No such process found\n");
    }
}