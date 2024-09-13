#include "main.h"
#define _XOPEN_SOURCE 700
// Assuming maximum number of background processes is 256
back_proc_list background_process_list[256];

char update_prompt[256] = "";

int process_count = 0; // Track the number of background processes

fore_process_list foreground_process_pid;
void initialize_foreground_process_pid()
{
    foreground_process_pid.process_id = -1;
}

void get_input(char *buffer, size_t size, char *save_dir)
{
    while (1)
    {
        errno = 0; // Clear errno before the input call
        if (fgets(buffer, size, stdin) != NULL)
        {
            return; // Input was successfully read, return from the function
        }

        // Check if the input was interrupted by a signal
        if (errno == EINTR)
        {
            // Interrupted by a signal, inform the user and retry the input
            // printf("\n");
            prompt(save_dir);
            getchar();
            fflush(stdout); // Flush the output buffer to ensure prompt display
            continue;       // Retry the input
        }
        else
        {
            // Handle end of file (Ctrl+D)
            if (feof(stdin))
            {

                ctrld_handler(); // Call your custom handler for Ctrl+D
                return;          // Return after handling Ctrl+D (this may exit the program as per your design)
            }
            else
            {
                perror("Error reading input");
                exit(EXIT_FAILURE); // Exit the program in case of any other error
            }
        }
    }
}
const char *PURPLE_FG = "\x1b[35m";   // Purple Foreground
const char *DARK_RED_BG = "\x1b[41m"; // Dark Red Background
const char *RESET = "\x1b[0m";        // Reset to default colors

int main()
{
    printf("%s%sWelcome to C shell Made by paarth%s\n\n", DARK_RED_BG, PURPLE_FG, RESET);

    initialize_foreground_process_pid();

    // Set up the SIGCHLD signal handler
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; // Restart interrupted system calls and don't call handler when children stop
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    char home_dir[256];
    char prev_dir[256] = "";
    if (getcwd(home_dir, sizeof(home_dir)) == NULL)
    {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
    char save_dir[256];
    if (getcwd(save_dir, sizeof(save_dir)) == NULL)
    {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
    int flag = 1;
    queue *q;
    q = create_queue(q);

    char *filename = "newfile.txt";
    read_queue_from_file(q, filename, save_dir);

    while (flag)
    {
        prompt(save_dir);
        strcpy(update_prompt, "");
        signal(SIGINT, ctrlc_handler);  // Handle Ctrl+C
        signal(SIGTSTP, ctrlz_handler); // Handle Ctrl+Z
        // Read user input and store in input
        char input[256];
        fflush(stdout);
        get_input(input, 256, save_dir);
        int size = strlen(input);
        if (size == 0 || (size == 1 && input[0] == '\n'))
        {
            continue; // No input, just prompt again
        }
        input[size - 1] = '\0';

        char save[256];
        strcpy(save, input);
        char delimiters[] = " \t";

        char *pa = strtok(save, delimiters);
        if (strcmp(pa, "log") != 0)
        {
            enqueue(q, input);
        }
        write_queue_to_file(q, filename, save_dir);

        char *temp = strtok(input, ";");
        char arr[256][1024];
        int command_count = 0; // Track the number of commands
        int background[256] = {0};

        while (temp != NULL)
        {
            // Trim leading and trailing whitespace
            while (*temp == ' ' || *temp == '\t')
                temp++;
            int len = strlen(temp);
            while (len > 0 && (temp[len - 1] == ' ' || temp[len - 1] == '\t'))
            {
                temp[--len] = '\0';
            }

            char *current_part = temp;
            int i = 0;
            // printf("stdghghj\n");
            // printf("%s\n",current_part);

            // Traverse the string to find '&'
            while (i < len)
            {
                if (current_part[i] == '&')
                {

                    // if (arr[command_count] != NULL)
                    {
                        strncpy(arr[command_count], current_part, i); // Copy the substring from current_part
                        arr[command_count][i] = '\0';                 // Null-terminate the string
                        background[command_count] = 1;                // Mark as background
                        command_count++;
                    }

                    // Move to the part after '&'
                    current_part = temp + i + 1;

                    len = strlen(current_part);
                    // i = 0;
                }
                else
                {
                    i++;
                }
            }

            // If there's still a part left after the last '&', add it as a foreground command
            if (*current_part != '\0')
            {
                // if (arr[command_count] != NULL)
                {
                    strcpy(arr[command_count], current_part);
                    background[command_count] = 0; // Mark as foreground
                    command_count++;
                }
            }
            // Move to the next tokenized command by ';'
            temp = strtok(NULL, ";");
        }

        for (int i = 0; i < command_count; i++)
        {
            if (background[i] == 1)
            {
                int saved_stdin = dup(STDIN_FILENO);
                int saved_stdout = dup(STDOUT_FILENO);
                execute_terminal_back(arr[i], q, &flag, home_dir, prev_dir);

                pid_t back_process = fork();
                if (back_process < 0)
                {
                    perror("fork failed");
                    continue;
                }
                else if (back_process == 0)
                {

                    char *token = strtok(arr[i], " \t");
                    char *arr1[100];
                    int j = 0;
                    while (token != NULL && j < 99)
                    {
                        arr1[j] = token;
                        j++;
                        token = strtok(NULL, " \t");
                    }
                    arr1[j] = NULL;

                    if (execvp(arr1[0], arr1) < 0)
                    {
                        perror("Invalid Command");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // Update process information after the fork is successful
                    sigset_t mask, old_mask;
                    // Block SIGCHLD temporarily while updating process_count to avoid race conditions
                    sigemptyset(&mask);
                    sigaddset(&mask, SIGCHLD);
                    sigprocmask(SIG_BLOCK, &mask, &old_mask);

                    // Store the background process details
                    background_process_list[process_count].process_id = back_process;
                    strncpy(background_process_list[process_count].process_name, arr[i], 255);
                    background_process_list[process_count].process_name[255] = '\0'; // Null-terminate string
                    process_count++;                                                 // Safely increment process_count

                    // Unblock SIGCHLD after updating process list
                    sigprocmask(SIG_SETMASK, &old_mask, NULL);

                    printf("Background process started: PID %d\n", back_process);
                    fflush(stdout);
                }
                restore_io(saved_stdin, saved_stdout);
            }
            else
            {
                // execute_terminal(arr[i], q, &flag, home_dir, prev_dir);
                // Measure execution time for foreground commands
                struct timespec start_time, end_time;
                clock_gettime(CLOCK_MONOTONIC, &start_time); // Record the start time

                execute_terminal(arr[i], q, &flag, home_dir, prev_dir);
                // handle_io_redirection_and_piping(arr[i],0,q,&flag,home_dir,prev_dir);

                clock_gettime(CLOCK_MONOTONIC, &end_time); // Record the end time

                // Calculate the elapsed time
                double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                                      (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

                // Print the time if the command took more than 2 second  i assumed that i need to print after 2 seconds
                if (elapsed_time > 2.0)
                {
                    // Round down the elapsed time to the nearest integer
                    int rounded_time = (int)elapsed_time;
                    printf("Foreground command '%s' took %d seconds to complete.\n", arr[i], rounded_time);
                    snprintf(update_prompt, sizeof(update_prompt), "%s : %ds", arr[i], rounded_time);
                    // printf("%s",update_prompt);

                    // printf("<%s@SYS:%s %s : %ds>\n", getenv("USER"), home_dir, arr[i], rounded_time);
                }
            }
        }
    }

    write_queue_to_file(q, filename, save_dir);

    return 0;
}
