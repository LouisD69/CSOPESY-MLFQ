/* 
	Names: Co, Sean Timothy S.
           Dela Cruz, Alexis Louis L.
	Group: 60
	Section: S16
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "intQueue.h"
//#include "print.h"
#define MAX 100
#define STAR1 9
#define STAR2 8

typedef char String[13]; // arbitrary, increased to 13 in case all 3 inputs for a, b, c have 3 digits each.... ex. "100 100 100"
typedef char Str100[101];

typedef struct{
    int id; // a
	int priority; // b
	int quantum; // c
	int *elements;
	int capacity;
    int size;
    int front;
    int rear; 
}queue;

queue * createQueue(int id, int priority, int quantum, int maxElements){
    /* Create a Queue */
    queue *Q;
    Q = (queue *)malloc(sizeof(queue));
    /* Initialise its properties */
    Q->id = id;
    Q->priority = priority;
    Q->quantum = quantum;
    Q->elements = (int *)malloc(sizeof(int)*maxElements);
    Q->size = 0;
    Q->capacity = maxElements;
    Q->front = 0;
    Q->rear = -1;
    /* Return the pointer */
    return Q;
}

typedef struct{
	int id; // f
	int arrival; // g
	int execution; // h (burst)
	int io_length; // i
	int io_interval; // j
}process;

int checkValid(int x, int y){
	int isValid = 1; // 1 means true
	if(x < 2 || x > 5){
		printf("Invalid input, X must be 2 <= X <= 5.\n");
		isValid = 0;
	}
	if(y < 3 || y > 100){
		printf("Invalid input, Y must be 3 <= Y <= 100.\n");
		isValid = 0;
	}
	
	return isValid;
}

// =======================================================================
// FUNCTIONS FOR PRINTING
// =======================================================================
void printProcess(process P){
	printStar(STAR1);
	printDivider(4);
	setColor(13);
	printf("PID: %3d ", P.id);
	resetColor();
	printDivider(4);
	setColor(10);
	printf("Arrival Time: %3d ", P.arrival);
	resetColor();
	printDivider(4);
	setColor(8);
	printf("Execution Time: %3d ", P.execution);
	resetColor();
	printDivider(4);
	printf("\n");
}

void printAnswer(process P[], int n, int st[], int et[], int wt[], int tt[], float awt, int star){
	int x;
	for(x = 0; x < n; x++){
		printStar(STAR2);
		
		setColor(13);
		printf("PID: %d\n", P[x].id);
		resetColor();
		printDivider(4);
		
		setColor(2);	
		printf("%-15s: %3d ", "Starting time", st[x]);
		resetColor();
		printDivider(4);
		
		setColor(3);
		printf("%-15s: %3d \n", "End time", et[x]);
		printVDivider(STAR2);
		resetColor();
		printDivider(4);
		
		setColor(6);
		printf("%-15s: %3d ", "Waiting time", wt[x]);
		resetColor();
		printDivider(4);
		
		setColor(11);
		printf("%-15s: %3d \n", "Turnaround time", tt[x]);
		resetColor();
	}
	printStar(STAR2);
	setColor(14);
	printf("Average waiting time: %.1f\n\n", awt);
	resetColor();
	printf("Scheduling Task Completed.\n");
}

// =======================================================================
// FUNCTIONS FOR SORTING
// =======================================================================
void swap(process P[], int p1, int p2)
{
	process temp;
	temp = P[p1];
	P[p1] = P[p2];
	P[p2] = temp;
}

int partition (process P[], int left, int right)
{
    int pivot = P[right].arrival; 
    int i = left - 1;
    int j;
 
    for (j=left; j<= right - 1; j++){
        if (P[j].arrival < pivot){
            i++;
            if(P[i].arrival < P[j].arrival)
            	swap(P, i, j);
        }
    }
    if(P[i+1].arrival > P[right].arrival)
    	swap(P, i+1, right);
    return i + 1;
}

void qsortByArrival(process P[], int left, int right){
	
	if(left<right){
		int pi = partition(P, left, right);	
		qsortByArrival(P, left, pi-1);
		qsortByArrival(P, pi+1, right);
	}		
	
}

void qsortByPriority(queue *Q[], int left, int right){	
	int i = left;
	int j = right;
	queue *temp = Q[left];
	if(left<right){
		while(i<j){
			while((Q[j])->priority <= (temp)->priority && i<j){
				j--;
			}
			Q[i] = Q[j];
			while((Q[i])->priority >= (temp)->priority && i<j){
				i++;
			}
			Q[j] = Q[i];
		}
		Q[i] = temp;
		
		qsortByPriority(Q, left, i-1);
		qsortByPriority(Q, j+1, right);	
	}			
}

// =======================================================================
// FUNCTIONS FOR SCHEDULING
// =======================================================================
float getAWT(int wt[], int n){
	float awt;
	int i, twt=0;
	for(i = 0; i < n; i++)
		twt += wt[i];
	
	awt = (float) twt / n;
	return awt;
}

void mlfq(process P[], int nP, int nQ){
	// foreground and background queue
	 // initialize values
	
	// int ct; // current time
	
	/* functions sample calls
	
		queue(foreQ, process1)
		queue(foreQ, process[i]
		process = dequeue(backQ);  returns the process that was removed from the queue
	*/
	
	/* Algo
	
		P[] is already sorted
		ct = 0
		While not all process are completed
			Enqueue all p with p.arrival = ct
			
	*/
	
	 /* Pseudo Code:    
	int ct = 0;
	int curr = 0;
	int complete = 0;
	
	int i = 0;
	while(complete < y){
		// enqueue newly arrived processes to highest priority queue
		while(i < y && P[i].arrival <= ct && P[i].execution > 0){
			enqueue(Q[0], i);
			i++;
		}
			
		// starting from highest priority queue that is not empty, perform rr on processes in queue
		runningP = dequeue process at head of queue
		if runningP != NULL
			if runningP.remainingTimeOnQueue = -1
				runningP.remainingTimeOnQueue = queue[curr].quantum
			while runningP.execution > 0 && ctr < queue[curr].quantum && runningP.remainingTimeOnQueue > 0	
				ctr += 1
				runningP.remainingTimeOnQueue -= 1
				runningP.execution -= 1
				if runningP.execution == 0
					complete += 1	
				ct += 1
				// enqueue newly arrived processes to highest priority queue
				while(i < y && P[i].arrival <= ct && P[i].execution > 0){
					enqueue(Q[0], i);
					i++;
				}
				
				Sctr += 1 // to check if ready for priority boost
				if runningP.totalRunTime % ioInterval = 0 && runningP.totalRunTime != 0
					give up cpu = true
					runningP.remainingTimeOnQueue -= ctr
					update st, et, wt, ioTime accordingly
					enqueue runningP to current queue	
		
			if give up cpu = false	
				update st, et, wt accordingly
			
			if runningP.execution > 0 && queue[curr] != queue[x-1] // not at lowest priority level
				runningP.remainingTimeOnQueue = -1
				enqueue runningP to queue one priority level lower
			else
				runningP.remainingTimeOnQueue = -1
				enqueue runningP to current queue		
			
			if Sctr == s // priority boost
				dequeue all processes from all queues
				enqueue all dequeued processes to highest priority queue // queue[curr] = queue[0]	

			check for highest priority queue that is not empty
				queue[curr] = set to highest priority queue that is not empty
				
		else 
			ct +=1
			Sctr += 1		
	} 
			
	*/
	
}

int main(){
	
	int i;
	queue q; 
	process P[5];
	printf("CP1\n");
	for(i=0; i<5; i++)
		P[i].id = i;
	
	for(i=0; i<5; i++)
		printf("%d ", P[i].id);
		
	printf("\nCP2\n");
	initQ(&q, 5);
	
	printf("\nCP3\n");
	for(i=0; i<5; i++)
		enqueue(&q, P[i]);
		
	printf("CP4\n");
	
	process p;
	for(i = 0; i<5; i++)
    {  
        p = dequeue(&q);
        printf("Dequeued PID: %d\n", p.id);
    }
	
	/*
	FILE *fptr;
	Str100 filename; //filename can have max of 100 characters only
	String xys, abc_fghij;
	int x, y, s, i, j, k;
	
	printf("Enter filename of text file: ");
	scanf("%s", filename);
	if((fptr = fopen(filename, "r")) == NULL){
		printf("%s not found. Program will now end.", filename);
		fclose(fptr);
		return 0;
	}else{
		fptr = fopen(filename, "r");
		
		printf("File contents: \n");
		
		// Scan X, Y, S from text file
		fgets(xys, 13, fptr);
        //printf("%s", xys);
        
        // Split X, Y, S
		char *temp[5];
		char *split = strtok(xys, " ");
		i=0;
		while(split != NULL){
			temp[i] = split;
			//printf("%c ", *temp[i]);
			split = strtok(NULL, " ");
			i++;
		}
		
		x = atoi(temp[0]);
		y = atoi(temp[1]);
		s = atoi(temp[2]);
		printf("X: %d, Y: %d, Z: %d\n", x, y, s);	
		
		// Scan X lines of queues and Y lines of processes
		queue *Q[x];
		process P[y];
		i = 0;
		j = 0;
		while(fgets(abc_fghij, 21, fptr) != NULL){
			//printf("%s\n", abc_fghij);
			k = 0;
			split = strtok(abc_fghij, " ");
			while(split != NULL){
				temp[k] = split;
				split = strtok(NULL, " ");
				k++;
			}
			if(k < 4){ // Scan X lines of queues
				queue *q = createQueue(atoi(temp[0]), atoi(temp[1]), atoi(temp[2]), y);
				Q[i] = q;
				i++;
			}else{ // Scan Y lines of processes
				process p = {atoi(temp[0]), atoi(temp[1]), atoi(temp[2]), atoi(temp[3]), atoi(temp[4])};
				P[j] = p;
				j++;
			}	
		}
		
		fclose(fptr);
		// Scan Completed
		// Check Validity of first line input
		if(checkValid(x, y) == 0){
			printf("Program will now end.");
			return 0;
		}
		
		qsortByPriority(Q, 0, x-1);
		for(i = 0; i < x; i++){
			printf("Queue ID:%d, Priority:%d, Quantum:%d\n", (Q[i])->id, (Q[i])->priority, (Q[i])->quantum);
		}
		printf("\n");
		qsortByArrival(P, 0, y-1);
		for(i = 0; i < y; i++){
			printf("Process ID:%d, Arrival:%d, Execution:%d, IO Burst Time:%d, IO Burst Interval:%d\n", P[i].id, P[i].arrival, P[i].execution, P[i].io_length, P[i].io_interval);
		}
	}
	*/

	return 0;
}
