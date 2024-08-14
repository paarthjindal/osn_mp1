#include "log_commands.h"
#include <sys/wait.h>

#define max_length 256
#define max_size 15

queue *create_queue(queue *q)
{
    q = (queue *)malloc(sizeof(queue));
    q->log = (char **)malloc(sizeof(char *) * 15);
    for (int i = 0; i < 15; i++)
    {
        q->log[i] = (char *)malloc(sizeof(char) * 256);
    }

    q->front = -1;
    q->rear = -1;
    q->size = 0;
    return q;
}

void enqueue(queue *q, char *command)
{
    if (q->size == 0)
    {
        q->front = 0;
        q->rear = 0;
        q->size = q->size + 1;
        strcpy(q->log[q->rear], command);
        return;
    }
    // first lets check the case that no previous command gets stored
    if (q->size != 0 && strcmp(q->log[q->size - 1], command) == 0)
    {
        return; // dont add that
    }
    if (q->size == 15)
    {
        // i need to overwrite the oldest commandl
        q->front = (q->front + 1) % 15;
    }
    else
    {
        q->size = q->size + 1;
    }
    q->rear = (q->rear + 1) % 15;

    strcpy(q->log[q->size-1], command);
}

void dequeue(queue *q)
{
    if (q->size == 0)
    {
        return;
    }
    else
    {
        q->front = (q->front + 1) % 15;
        q->size = q->size - 1;
    }
}

void dispaly_log(queue *q)
{
    if (q->size == 0)
    {
        printf("the log is empty\n");
    }
    else
    {
        for (int i = q->front; i < q->size; i++)
        {
            printf("%s\n", q->log[i]);
        }
    }
}

void execute(queue *q, int index)
{
    if (index > q->size)
    {
        printf("not a valid command\n");
        return;
    }
    pid_t id;
    id = fork();
    if (id < 0)
    {
        printf("erorr in creating the child process\n");
        exit(EXIT_FAILURE);
    }
    else if (id == 0)
    {
    }
    else
    {
        wait(NULL); // so that child process get executed properly
    }
}

void purge(queue *q)
{
    create_queue(q);
}
