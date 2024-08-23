#include "input_requirement.h"
#include "log_commands.h"
#include "seek.h"

void execute_terminal(char *s, queue *q, int *flag, char *home_dir, char *prev_dir)
{
    char delimiters[] = " \t";
    char *y = (char *)malloc(sizeof(char));
    strcpy(y, s);
    // printf("%s\n",s);

    char *token = strtok(s, delimiters);
    while (token != NULL)
    {
        // Print each token (for demonstration purposes)
        printf("Token: %s\n", token);
        // printf("%s\n", y);

        // Check if the first token is "EXIT"
        if (strcmp(token, "EXIT") == 0)
        {

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
                        printf("wrong input error\n");
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
        else if (strcmp(token, "seek") == 0)
        {
            int arr[3];
            arr[0] = 0;
            arr[1] = 0;
            arr[2] = 0;
            token = strtok(NULL, delimiters);
            char *path = (char *)malloc(sizeof(char)*1024);
            char *seek_name = (char *)malloc(sizeof(char)*1024);
            path="";
            seek_name="";
            int flag=0;
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
                    if(flag==0)
                    {
                        flag=1;
                        seek_name=token;
                    }
                    else
                    {
                        path=token;
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
                printf("Invalid flags\n");
            }
            else
            {
                char *resolved_path_seek = resolve_path_seek(path, home_dir, prev_dir);

                seek(resolved_path_seek, seek_name, arr[0], arr[1], arr[2]);

                free(resolved_path_seek);
            }
        }
        else
        {
            printf("wrong input\n");
        }
        // for (int i = 0; i < q->size; i++)
        // {
        //     printf("%s\n", q->log[i]);
        // }

        // Get the next token
        token = strtok(NULL, delimiters);
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
