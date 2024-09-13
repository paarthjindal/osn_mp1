#include "redirection_along_with_pipes.h"
#include "input_requirement.h"
#include "log_commands.h"
// Helper function to handle I/O redirection and piping

// Helper function to handle I/O redirection and piping
// void handle_io_redirection_and_piping(char *command, int is_background, queue *q, int *flag, char *home_dir, char *prev_dir)
// {
//     int fd_in = 0, fd_out = 0;
//     int fd[2]; // Pipe file descriptors
//     int num_pipes = 0;

//     char *commands[10]; // Assuming max 10 commands in a pipe chain
//     char *redirect_in = NULL, *redirect_out = NULL, *append_out = NULL;

//     commands[num_pipes++] = strtok(command, "|");
//     while ((commands[num_pipes] = strtok(NULL, "|")) != NULL)
//         num_pipes++;

//     for (int i = 0; i < num_pipes; i++)
//     {
//         char *sub_command = commands[i];
//         char *args[100];
//         int j = 0;

//         // Check for I/O redirection
//         char *redir = strchr(sub_command, '>');
//         if (redir)
//         {
//             *redir = '\0';
//             redir++;
//             if (*redir == '>')
//             {
//                 append_out = strtok(redir + 1, " \t");
//             }
//             else
//             {
//                 redirect_out = strtok(redir, " \t");
//             }
//         }

//         redir = strchr(sub_command, '<');
//         if (redir)
//         {
//             *redir = '\0';
//             redirect_in = strtok(redir + 1, " \t");
//         }

//         char *token = strtok(sub_command, " \t");
//         while (token != NULL)
//         {
//             args[j++] = token;
//             token = strtok(NULL, " \t");
//         }
//         args[j] = NULL;

//         // Handle piping logic
//         if (i > 0)
//         {
//             dup2(fd_in, STDIN_FILENO);
//             close(fd_in);
//         }

//         if (i < num_pipes - 1)
//         {
//             pipe(fd);
//             fd_in = fd[0];
//             dup2(fd[1], STDOUT_FILENO);
//             close(fd[1]);
//         }
//         else
//         {
//             // Handle I/O redirection for the last command in the pipe
//             if (redirect_out)
//             {
//                 fd_out = open(redirect_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//                 if (fd_out < 0)
//                 {
//                     perror("open failed");
//                     exit(EXIT_FAILURE);
//                 }
//                 dup2(fd_out, STDOUT_FILENO);
//                 close(fd_out);
//             }
//             else if (append_out)
//             {
//                 fd_out = open(append_out, O_WRONLY | O_CREAT | O_APPEND, 0644);
//                 if (fd_out < 0)
//                 {
//                     perror("open failed");
//                     exit(EXIT_FAILURE);
//                 }
//                 dup2(fd_out, STDOUT_FILENO);
//                 close(fd_out);
//             }

//             if (redirect_in)
//             {
//                 fd_in = open(redirect_in, O_RDONLY);
//                 if (fd_in < 0)
//                 {
//                     perror("open failed");
//                     exit(EXIT_FAILURE);
//                 }
//                 dup2(fd_in, STDIN_FILENO);
//                 close(fd_in);
//             }
//         }

//         // Execute the command
//         if (is_background)
//         {
//             if (fork() == 0)
//             {
//                 execvp(args[0], args);
//                 perror("execvp failed");
//                 exit(EXIT_FAILURE);
//             }
//         }
//         else
//         {
//             // Call your custom execute_terminal() for foreground commands
//             execute_terminal(sub_command, q, flag, home_dir, prev_dir);
//         }

//         wait(NULL); // Wait for the child process to finish
//     }
// }