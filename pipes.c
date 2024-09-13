#include "main.h"

// Function to execute piped commands
#include "seek.h"
#include "main.h"
#include "neonate.h"
// dont know for some reason there is infinte loop going on if i type yes in the command line

int handle_redirection_back(char *cmd)
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
            perror("open");
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    return 0;
}

void restore_io_back(int saved_stdin, int saved_stdout)
{
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}
void execute_single_command_back(char *command, queue *q, int *flag, char *home_dir, char *prev_dir)
{
    // Handle redirection if any
    if (handle_redirection_back(command) == -1)
    {
        perror("Redirection failed");
        return;
    }

    // Execute command after handling redirection
    // execute_final_terminal(command, q, flag, home_dir, prev_dir);

    // Restore original I/O for the next command
    // restore_io(saved_stdin, saved_stdout);
    return;
}

void execute_piped_commands_back(char **commands, queue *q, int *flag, char *home_dir, char *prev_dir)
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
            execute_single_command_back(commands[i], q, flag, home_dir, prev_dir);

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

void execute_terminal_back(char *s, queue *q, int *flag, char *home_dir, char *prev_dir)  
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

        execute_single_command_back(s, q, flag, home_dir, prev_dir);
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
        execute_piped_commands_back(split_into_pipes, q, flag, home_dir, prev_dir);
    }
}
