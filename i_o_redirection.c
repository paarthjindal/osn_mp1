#include "i_o_redirection.h"

// Function to execute commands with redirection


int handle_redirection(char *cmd)
{
    char *infile = NULL;
    char *outfile = NULL;
    char *appendfile = NULL;
    char *token;
    int input_fd = 0, output_fd = 1;

    // Check for input redirection '<'
    if ((token = strchr(cmd, '<')))
    {
        *token = '\0';
        infile = strtok(token + 1, " \t");
        input_fd = open(infile, O_RDONLY);
        if (input_fd < 0)
        {
            perror("Error opening input file");
            return -1;
        }
        dup2(input_fd, STDIN_FILENO);
    }

    // Check for output redirection '>'
    if ((token = strchr(cmd, '>')))
    {
        if (*(token + 1) == '>') // Handle '>>' (append)
        {
            *token = '\0';
            appendfile = strtok(token + 2, " \t");
            output_fd = open(appendfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
        }
        else // Handle '>'
        {
            *token = '\0';
            outfile = strtok(token + 1, " \t");
            output_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }

        if (output_fd < 0)
        {
            perror("Error opening output file");
            return -1;
        }
        dup2(output_fd, STDOUT_FILENO);
    }
    return 0;
}