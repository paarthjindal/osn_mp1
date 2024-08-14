#include "display_requirement.h"
#include "proclore.h"
#include "log_commands.h"

#include <sys/types.h>
#include <string.h>
int main()
{
    char *home_dir;

    // Get the home directory from the HOME environment variable
    home_dir = getenv("HOME");
    if (home_dir == NULL)
    {
        perror("getenv failed");
        exit(EXIT_FAILURE);
    }
    int flag = 1;
    queue *q;
    q = create_queue(q);
    while (flag)
    {

        const char delimiters[] = " \t";
        // Update and display the prompt
        prompt(home_dir);

        // Read user input ans store in input
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            perror("error in taking  input from the user");
            exit(EXIT_FAILURE);
        }
        // Remove the newline character, if present
        int size = strlen(input);
        input[size - 1] = '\0';
        if (strcmp("log", input) != 0)
        {
            enqueue(q, input);
        }

        char *token = strtok(input, delimiters);
        while (token != NULL)
        {
            // Print each token (for demonstration purposes)
            printf("Token: %s\n", token);

            // Check if the first token is "EXIT"
            if (strcmp(token, "EXIT") == 0)
            {
                flag = 0;
                printf("thanks for using mine terminal , hope you had a pleasant experience\n");
                break;
            }
            else if (strcmp(token, "hop") == 0)
            {
            }
            else if (strcmp(token, "proclore") == 0)
            {
                token = strtok(NULL, " ");
                // printf("id = %s\n",token);
                pid_t process_id = getpid();
                if (token == NULL)
                {
                    describe_process(process_id);
                }
                else
                {
                    int value;
                    // it is used to convert a token ie char * to int
                    if (sscanf(token, "%d", &value) == 1)
                    {
                        // printf("The integer value is: %d\n", value);
                        describe_process(value);
                    }
                    else
                    {
                        printf("Conversion failed.\n");
                    }
                }
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

            // Get the next token
            token = strtok(NULL, delimiters);
        }
        // printf("\n");
    }

    return 0;
}
