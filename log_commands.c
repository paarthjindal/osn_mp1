#include "log_commands.h"

void create_queue(queue* q)
{
    q->front=0;
    q->rear=-1;
    q->size=0;
}

void enqueue(queue *q, char *command)
{
    // first lets check the case that no previous command gets stored 
    
}

void dequeue(queue *q);

void dispaly_log(queue *q);

void execute(queue * q);

void purge(queue * q);
