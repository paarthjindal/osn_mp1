#include "main.h"
// dont know for some reason there is infinte loop going on if i type yes in the command line
#include "alias.h"
char lvda[128] = ".myshrc";
// divij
// Function to read bash file and store aliases in the linked list


int handle_redirection(char *cmd)
{
    char *infile = NULL, *outfile = NULL;
    int append = 0; // for '>>' case
    char *token;
    int fd;

    // Handling input redirection
    if ((token = strstr(cmd, "<")))
    {
        *token = '\0';
        infile = strtok(token + 1, " \t");
    }

    // Handling output redirection
    if ((token = strstr(cmd, ">>")))
    {
        *token = '\0';
        outfile = strtok(token + 2, " \t");
        append = 1;
    }
    else if ((token = strstr(cmd, ">")))
    {
        *token = '\0';
        outfile = strtok(token + 1, " \t");
    }

    // Handle input file
    if (infile)
    {
        fd = open(infile, O_RDONLY);
        if (fd < 0)
        {
            perror("open");
            return -1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    // Handle output file
    if (outfile)
    {
        fd = open(outfile, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
        if (fd < 0)
        {
            perror("error in opening");
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    return 0;
}

void restore_io(int saved_stdin, int saved_stdout)
{
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}
void execute_single_command(char *command, queue *q, int *flag, char *home_dir, char *prev_dir)
{
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

    // Handle redirection if any
    if (handle_redirection(command) == -1)
    {
        perror("Redirection failed");
        return;
    }

    // Execute command after handling redirection
    execute_final_terminal(command, q, flag, home_dir, prev_dir);

    // Restore original I/O for the next command
    restore_io(saved_stdin, saved_stdout);
}

void execute_piped_commands(char **commands, queue *q, int *flag, char *home_dir, char *prev_dir)
{
    int i = 0, in_fd = 0, fd[2];
    pid_t pid;
    int save_in, save_out;

    // Save the original stdin and stdout
    save_in = dup(STDIN_FILENO);
    save_out = dup(STDOUT_FILENO);

    while (commands[i] != NULL)
    {
        // Create a pipe for the next command
        if (commands[i + 1] != NULL)
        {
            if (pipe(fd) == -1)
            {
                perror("Pipe failed");
                exit(1);
            }
        }

        // Fork a new process to execute the command
        pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) // Child process
        {
            // Redirect input from the previous command (if any)
            if (in_fd != 0)
            {
                if (dup2(in_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 failed for stdin");
                    exit(1);
                }
                close(in_fd);
            }

            // If there's a next command, redirect output to the pipe
            if (commands[i + 1] != NULL)
            {
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 failed for stdout");
                    exit(1);
                }
                close(fd[1]);
            }

            // Close read end of the pipe in the child process
            close(fd[0]);

            // Execute the command (replace this with your command execution function)
            execute_single_command(commands[i], q, flag, home_dir, prev_dir);

            // Exit the child process
            exit(0);
        }
        else // Parent process
        {
            // Wait for the child to finish if it's the last command
            if (commands[i + 1] == NULL)
            {
                wait(NULL);
            }

            // Close write end of the pipe
            close(fd[1]);

            // Set the input for the next command to the read end of the pipe
            in_fd = fd[0];

            // Parent continues to the next command
            i++;
        }
    }

    // Restore stdin and stdout
    dup2(save_in, STDIN_FILENO);
    close(save_in);
    dup2(save_out, STDOUT_FILENO);
    close(save_out);

    // Wait for all child processes to finish
    while (wait(NULL) > 0)
        ;
}

void execute_terminal(char *s, queue *q, int *flag, char *home_dir, char *prev_dir)
{

    while (*s == ' ')
        s++; // Trim leading spaces
    int len = strlen(s);
    while (len > 0 && s[len - 1] == ' ')
        s[--len] = '\0';
    // Check if the command contains the pipe symbol "|"
    if (strchr(s, '|') == NULL)
    {
        // If there's no pipe, execute the command directly
        execute_single_command(s, q, flag, home_dir, prev_dir);
    }
    else
    {
        // Error check: command should not start or end with a pipe
        if (s[0] == '|' || s[len - 1] == '|')
        {
            printf("Invalid use of pipe\n");
            return; // Exit due to invalid pipe usage
        }
        // If a pipe is found, tokenize and process the piped commands
        char *split_into_pipes[256];
        char *command;
        char save_original_command[256];
        int pipe_count = 0;

        // Copy the original command to preserve it
        strcpy(save_original_command, s);

        // Tokenize the string into separate commands based on the pipe symbol "|"
        command = strtok(save_original_command, "|");
        while (command != NULL)
        {
            // Store each command in the array
            split_into_pipes[pipe_count] = command;
            pipe_count++;

            // Continue tokenizing to the next command
            command = strtok(NULL, "|");
        }
        split_into_pipes[pipe_count] = NULL; // Mark the end of the command array

        // Execute the commands using the piped execution function
        execute_piped_commands(split_into_pipes, q, flag, home_dir, prev_dir);
    }
}
void write_queue_to_file(queue *q, const char *filename, const char *home_dir)
{
    char file_path[256];
    snprintf(file_path, 256, "%s/%s", home_dir, filename);

    FILE *file = fopen(file_path, "w");
    if (file == NULL)
    {
        perror("Could not open file for writing");
        return;
    }

    for (int i = 0; i < q->size; i++)
    {
        int index = (q->front + i) % 15;
        fprintf(file, "%s\n", q->log[index]);
    }

    fclose(file);
}

void read_queue_from_file(queue *q, const char *filename, const char *home_dir)
{
    char file_path[256];
    snprintf(file_path, 256, "%s/%s", home_dir, filename);

    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("Could not open file for reading");
        return;
    }

    char buffer[256];
    while (fgets(buffer, 256, file))
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        enqueue(q, buffer);
    }

    fclose(file);
}

void execute_final_terminal(char *s, queue *q, int *flag, char *home_dir, char *prev_dir)
{
   
    // print(start);
    if(find_alias(start,s)==NULL)
    {

    }
    else
    {
        s=find_alias(start,s);
    }

    char delimiters[] = " \t";
    char y[256];
    strcpy(y, s);
    // printf("%s\n",s);

    char *token = strtok(s, delimiters);

    while (token != NULL)
    {
        // printf("Token: %s\n", token);
        // printf("%s\n", y);

        // Check if the first token is "EXIT"3
        if (strcmp(token, "EXIT") == 0)
        {

            *flag = 0;
            fflush(stdout);
            printf("thanks for using mine terminal , hope you had a pleasant experience\n");

            exit(1);

            break;
        }
        else if (strcmp(token, "hop") == 0)
        {
            token = strtok(NULL, delimiters);
            if (token == NULL)
            {
                hop("~", home_dir, prev_dir);
            }
            else
            {
                while (token != NULL)
                {
                    hop(token, home_dir, prev_dir);
                    token = strtok(NULL, delimiters);
                }
            }
        }
        else if (strcmp(token, "reveal") == 0)
        {
            int arr[2];
            arr[0] = 0;
            arr[1] = 0;
            token = strtok(NULL, delimiters);
            char *path = "";
            while (token != NULL)
            {
                if (token[0] == '-')
                {
                    if (strchr(token, 'a'))
                        arr[0] = 1;
                    if (strchr(token, 'l'))
                        arr[1] = 1;
                }
                else
                {
                    path = token;
                }
                token = strtok(NULL, delimiters);
            }
            if (path == "")
            {
                path = ".";
            }
            // Resolve the path to an absolute path
            char *resolved_path = resolve_path_reveal(path, home_dir, prev_dir);

            // Call reveal to list files and directories
            reveal(resolved_path, arr[0], arr[1]);
            free(resolved_path);
        }
        else if (strcmp(token, "proclore") == 0)
        {
            // Process the 'proclore' command
            token = strtok(NULL, " ");
            pid_t process_id = getpid();
            if (token == NULL)
            {
                describe_process(process_id);
                // After processing, enqueue the full command
            }
            else
            {
                int value;
                // It is used to convert a token ie char * to int
                if (sscanf(token, "%d", &value) == 1)
                {
                    describe_process(value);
                    // After processing, enqueue the full command
                }
                else
                {
                    printf("Conversion failed.\n");
                }
            }
            // printf("%s\n", s);
        }
        else if (strcmp(token, "log") == 0)
        {
            token = strtok(NULL, delimiters);
            if (token == NULL)
            {
                // printf("are you working");
                // printf("%d", q->size);
                dispaly_log(q);
            }
            else
            {
                if (strcmp(token, "purge") == 0)
                {
                    purge(q);
                }
                else if (strcmp(token, "execute") == 0)
                {
                    token = strtok(NULL, delimiters);
                    if (token == NULL)
                    {
                        printf("wrong input error \n");
                    }
                    else
                    {
                        int value;
                        // it is used to convert a token ie char * to int
                        if (sscanf(token, "%d", &value) == 1)
                        {
                            // printf("The integer value is: %d\n", value);
                            int z = q->size - value;
                            if (z < 0)
                            {
                                printf("wrong index \n");
                            }
                            else
                            {
                                execute_terminal(q->log[z], q, flag, home_dir, prev_dir);
                            }
                        }
                        else
                        {
                            printf("Conversion failed. \n");
                        }
                        // need to think over here if there is invaid command in log execute  then need to handle it
                        // token = strtok(NULL, delimiters);
                        // if (token != NULL)
                        // {
                        //     printf("Invalid command\n");
                        // }
                    }
                }
                else
                {

                    printf("wrong input erorr \n");
                }
            }
        }
        else if (strcmp(token, "seek") == 0)
        {
            int arr[3];
            arr[0] = 0;
            arr[1] = 0;
            arr[2] = 0;
            token = strtok(NULL, delimiters);
            char *path;
            char *seek_name;
            path = "";
            seek_name = "";
            int flag = 0;
            while (token != NULL)
            {
                if (token[0] == '-')
                {
                    if (strchr(token, 'd'))
                    {
                        arr[0] = 1;
                    }
                    if (strchr(token, 'f'))
                    {
                        arr[1] = 1;
                    }
                    if (strchr(token, 'e'))
                    {
                        arr[2] = 1;
                    }
                }
                // else if (strncmp(token, "./", 2) == 0)
                // {
                //     path = token;
                // }
                // else
                // {
                //     seek_name = token;
                // }
                else
                {
                    if (flag == 0)
                    {
                        flag = 1;
                        seek_name = token;
                    }
                    else
                    {
                        path = token;
                    }
                }
                token = strtok(NULL, delimiters);
            }
            if (path == "")
            {
                path = ".";
            }
            if (arr[0] == 1 && arr[1] == 1)
            {
                printf("Invalid flags \n");
            }
            else
            {
                char *resolved_path_seek = resolve_path_seek(path, home_dir, prev_dir);

                seek(resolved_path_seek, seek_name, arr[0], arr[1], arr[2]);

                free(resolved_path_seek);
            }
        }
        else if (strcmp(token, "activities") == 0)
        {
            // printf("hek");
            print_activities();
        }
        else if (strcmp(token, "bg") == 0)
        {
            token = strtok(NULL, delimiters);
            if (token == NULL)
            {
                printf("wrong input error\n");
            }
            else
            {
                int value;
                // it is used to convert a token ie char * to int
                if (sscanf(token, "%d", &value) == 1)
                {
                    bg_command(value);
                    // printf("The integer value is: %d\n", value);
                }
                else
                {
                    printf("Conversion failed.\n");
                }
            }
        }
        else if (strcmp(token, "fg") == 0)
        {
            token = strtok(NULL, delimiters);
            if (token == NULL)
            {
                printf("wrong input error\n");
            }
            else
            {
                int value;
                // it is used to convert a token ie char * to int
                if (sscanf(token, "%d", &value) == 1)
                {
                    fg_command(value);

                    // printf("The integer value is: %d\n", value);
                }
                else
                {
                    printf("Conversion failed.\n");
                }
            }
        }
        else if (strcmp(token, "ping") == 0)
        {
            char *pid_str = strtok(NULL, " \t");
            char *signal_num_str = strtok(NULL, " \t");
            if (pid_str != NULL && signal_num_str != NULL)
            {
                handle_ping(pid_str, signal_num_str);
            }
            else
            {
                printf("Usage: ping <pid> <signal_number>\n");
            }
        }
        else if (strcmp(token, "neonate") == 0)
        {
            token = strtok(NULL, delimiters);
            if (token == NULL)
            {
                printf("Usage: neonate -n [time_arg]\n");
            }
            else if (strcmp(token, "-n") == 0)
            {
                token = strtok(NULL, delimiters);
                if (token != NULL)
                {
                    int time_arg = atoi(token);
                    if (time_arg > 0)
                    {
                        neonate(time_arg);
                    }
                    else
                    {
                        printf("Invalid time argument.\n");
                    }
                }
                else
                {
                    printf("Time argument missing.\n");
                }
            }
            else
            {
                printf("Invalid command\n");
            }
        }
        else if (strcmp(token, "iMan") == 0)
        {
            token = strtok(NULL, delimiters);
            if (token == NULL)
            {
                printf("Invalid command\n");
            }
            else
            {
                fetchManPage(token);
            }
        }
        else
        {
            // printf("wrong input\n");
            // now over here i need to implement mine sixth functionality
            pid_t pid = fork();
            foreground_process_pid.process_id = pid;
            // foreground_process_pid.process_name=arr;
            strncpy(foreground_process_pid.process_name, y, 255);
            foreground_process_pid.process_name[255] = '\0'; // Null-terminate string
            if (pid < 0)
            {
                perror("fork");
                return;
            }
            else if (pid == 0)
            {
                char *arr[100]; // i am assuming that no of arguments cant exceed 100
                int i = 0;
                int is_background = 0;
                while (token != NULL && i <= 99)
                {

                    arr[i] = token;
                    i++;

                    token = strtok(NULL, delimiters);
                }
                // printf("is it a background %d", is_background);
                arr[i] = NULL;
                // for (int j = 0; j <i; j++)
                // {
                //     printf("%s\n",arr[j]);
                // }

                if (execvp(arr[0], arr) < 0)
                {
                    // printf("hello\n");
                    printf("Invalid Command\n");
                }
            }
            else if (pid > 0)
            {
                wait(NULL); // its because i want to execute the child process before the parent
            }
            return;
        }
        // for (int i = 0; i < q->size; i++)
        // {
        //     printf("%s\n", q->log[i]);
        // }

        // Get the next token
        token = strtok(NULL, delimiters);
    }
}
