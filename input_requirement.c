#include "input_requirement.h"
#include "log_commands.h"

void execute_terminal(char *s, queue *q, int *flag, char *home_dir, char *prev_dir)
{
    char delimiters[] = " \t";

    char *token = strtok(s, delimiters);
    while (token != NULL)
    {
        // Print each token (for demonstration purposes)
        printf("Token: %s\n", token);

        // Check if the first token is "EXIT"
        if (strcmp(token, "EXIT") == 0)
        {
            enqueue(q, s);

            *flag = 0;
            printf("thanks for using mine terminal , hope you had a pleasant experience\n");
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
            enqueue(q, s);
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
            enqueue(q, s);
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
                if (token == "purge")
                {
                    purge(q);
                }
                else if (token == "execute")
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
                            // printf("The integer value is: %d\n", value);
                        }
                        else
                        {
                            printf("Conversion failed.\n");
                        }
                    }
                }
                else
                {
                    printf("wrong input erorr\n");
                }
            }
        }
        else
        {
            printf("wrong input\n");
        }

        // Get the next token
        token = strtok(NULL, delimiters);
    }
}
