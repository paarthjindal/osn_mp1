#include "activites.h"

extern int process_count;
int compare_processes(const void *a, const void *b) {
    back_proc_list *procA = (back_proc_list *)a;
    back_proc_list *procB = (back_proc_list *)b;
    return strcmp(procA->process_name, procB->process_name);
}

void print_activities() {
    // Sort the background processes lexicographically by command name
    qsort(background_process_list, process_count, sizeof(back_proc_list), compare_processes);
    // printf("is this function called %d\n",process_count);
    // Print the background processes with their state
    for (int i = 0; i < process_count; i++) {
        if (background_process_list[i].is_running == 1) {
            printf("[%d] : %s - Running\n", background_process_list[i].process_id, background_process_list[i].process_name);
        } else {
            printf("[%d] : %s - Stopped\n", background_process_list[i].process_id, background_process_list[i].process_name);
        }
    }
}
