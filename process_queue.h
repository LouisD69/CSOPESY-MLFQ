#include <stdio.h>

typedef struct {
	int id;
	int arrival;
	int execution; // burst
	int ioburst;
	int interval;
} process;

typedef struct {
	int head;
	int tail;
	int curr; // current no. of process in the queue
	int max;
	process *P;
} queue;

void initQ(queue *Q, int n){
	Q->head = 0;
	Q->tail = n-1;
	Q->curr = 0;
	Q->max = n;
	process new_P[n];
	Q->P = new_P;
}
process peek(queue *Q){
	return Q->P[Q->head];
}

bool isEmpty(queue *Q) {
   return Q->curr == 0;
}

bool isFull(queue *Q) {
   return Q->curr == Q->max;
}

int count(queue *Q) {
   return Q->curr;
} 

void enqueue(queue *Q, process p) {
	
	if(isFull(Q)){
		printf("QUEUE IS FULL");
		return;
	}

	Q->tail = (Q->tail + 1) % Q->max;          
	Q->curr++;
	Q->P[Q->tail] = p;
	printf("Inserted %d \n", Q->P[Q->tail].id);
} 

process dequeue(queue *Q) {
	process p;
	p.id = -1; // -1 pid means null
	if(!isEmpty(Q)){
		p = Q->P[Q->head];
		printf("Removing %d \n", p.id);
		Q->head = (Q->head + 1) % Q->max;
		Q->curr--;
	}			 
	return p;  
}

