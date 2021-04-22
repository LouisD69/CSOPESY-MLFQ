#include <stdio.h>
#include <stdlib.h>
#define DEBUG 0

typedef struct Queue
{
    int capacity; // max
    int size; // current number of elements
    int head; // first in the queue
    int tail; // last in the queue
    int *arr;
}Queue;

Queue * createQueue(int max)
{
    Queue *Q;
    Q = (Queue *)malloc(sizeof(Queue));
    
    // initialize variables
    Q->arr = (int *)malloc(sizeof(int)*max);
    Q->head = 0;
    Q->tail = -1;
    Q->size = 0;
    Q->capacity = max;
    
   
    return Q; // return pointer
}

int head(Queue *Q)
{
    if(Q->size==0)
    {
    	if(DEBUG)
    	{
    		printf("Queue is Empty\n");
            exit(0);
		}
    }
    
    return Q->arr[Q->head]; // return head
}

int dequeue(Queue *Q)
{
	int h = -1; // return -1 if empty queue
 
    if(Q->size==0)
    {
		if(DEBUG)
			printf("Queue is Empty\n");
    }
    else
    {
		h = head(Q);
        Q->size--; // decrease size
        Q->head++; // move pointer to head to the element next to head
        if(Q->head==Q->capacity)
        {
            Q->head=0; // for circularity
        }
    }
    return h;
}

void enqueue(Queue *Q, int data)
{
    if(Q->size == Q->capacity)
    {
        if(DEBUG)
			printf("Queue is Full\n");
    }
    else
    {
        Q->size++;
        Q->tail = Q->tail + 1;

        if(Q->tail == Q->capacity) 
        {
                Q->tail = 0; // for circularity
        }
       
        Q->arr[Q->tail] = data; // insert     
    }
}
