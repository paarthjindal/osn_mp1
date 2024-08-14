#include "input_requirement.h"
#include "log_commands.h"

void execute_terminal(char * s,queue * q,int * flag)
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
                    enqueue(q, s);
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
