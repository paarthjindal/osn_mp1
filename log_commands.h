#ifndef LOG_COMMANDS_H
#define LOG_COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>


#define max_length 256
#define max_size 15

typedef struct queue
{
    int front;
    int rear;
    int size;
    char** log;
} queue;

extern queue *log_queue;

queue* create_queue(queue *q);

void enqueue(queue *q, char *command);

void dequeue(queue *q);

void dispaly_log(queue *q);

void execute(queue * q,int index);

void purge(queue * q);

#endif