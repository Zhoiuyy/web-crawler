#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "queue.h"

void 
QueueInit(Queue* queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int QueueEmpty(Queue* queue)
{
        return (queue->size == 0);
}
 
void QueuePush(Queue* queue, char *data)
{       
        Link* node;
        node = (Link*)malloc(sizeof(Link));
        assert(node != NULL);
        
        node->data = data;
        node->next = NULL;
        
        if(QueueEmpty(queue))
        {
            queue->front = node;
            queue->rear = node;
        }
        else
        {            
            queue->rear->next = node;
            queue->rear = node;
        }
        ++queue->size;
        
        
}
 
int QueuePop(Queue* queue, char **data)
{
        if (QueueEmpty(queue))
        {
                return 0;
        }
        Link* tmp = queue->front;
        *data = queue->front->data;
        queue->front = queue->front->next;
        free(tmp);
        --queue->size;
 
        return 1;
}
 
void QueueDestroy(Queue* queue)
{
     Link* tmp;
     while(queue->front)
     {
         tmp = queue->front;
         queue->front = queue->front->next;
         free(tmp);
     }
}
