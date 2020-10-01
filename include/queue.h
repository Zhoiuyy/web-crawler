#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#ifndef QUEUE_H
#define QUEUE_H

 
typedef struct Link
{
    char *data;
    struct Link* next;
}Link;

typedef struct Queue
{
    Link* front;
    Link* rear;
    int size;
}Queue;

void 
QueueInit(Queue* queue);

int QueueEmpty(Queue* queue);
 
void QueuePush(Queue* queue, char *data);
 
int QueuePop(Queue* queue, char **data);
 
void QueueDestroy(Queue* queue);

#endif