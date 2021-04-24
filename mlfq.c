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
#include "process_queue.h"
#include "print.h"
#define MAX 100
#define STAR1 9
#define STAR2 8

typedef char String[13]; // arbitrary, increased to 13 in case all 3 inputs for a, b, c have 3 digits each.... ex. "100 100 100"
typedef char Str100[101];

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
	/*
	// nP = num of processes
	// nQ = num of Queues
	
	// initialize the queues
	Queue *Q[]; // array of queue pointers
	for(i=0; i<nQ; i++)
		createQueue(Q[i]);
	
	
	*/
	/* Algo
	
		P[] is already sorted
		ct = 0
		While not all process are completed
			Enqueue(Q[0], P[x]) if it has arrived
			P[x].start = ct;
			ct += quantum
			P[x].end = ct;
			
			Process p = dequeue(Q[0])
			if(P[x] has been completed)
				dequeue(Q[0])
			else
				enqueue(Q[1], p)
				
			
	*/
	
	 /* Pseudo Code:     note: not taken into account io burst interval (input J), di ko pa gets part na yan
	ct = 0
	sort processes by arrival time
	create queues
	sort queues by priority
	
	while complete < num_process 
		while i < num_process && p[i].arrival <= ct && p[i].execution > 0
			if p.arrival == ct
				enqueue p[i] to highest priority queue
			i++	
			
		loop through queues
			currQ = highest prio queue index (0)
			if prev >= 0 && p[prev].ioburst < queue[prev].quantum && p[prev].execution > 0
				enqueue p[prev] to queue to same queue
			else if prev >= 0 && p[prev].ioburst >= queue[prev].quantum && p[prev].execution > 0
				enqueue p[prev] to queue one priority down	
									
			curr = dequeue 	
		
			check if job is ready
		
			if job is not ready, ct++, continue
		
			set start/end index 
			get start time
			get wait time
		
			if p[curr].exectuion >= q[currQ].quantum
				adjust ct 
				get end time
				decrease execution of p[curr]	
			else 
				adjust wait time
				adjust current time
				get end time
				decrease execution of p[curr]
			
			if(p[curr].execution <= 0)
				complete++
				
			prev = curr
			
			if ct == prio boost
				promote all to highest priority
			
			if q[currQ] is empty
				move to queue one priority down		
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
	}
	else{
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
		
		// Scan X lines of queues
		queue Q[x];
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
				queue q = {atoi(temp[0]), atoi(temp[1]), atoi(temp[2])};
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
		
		for(i = 0; i < x; i++){
			printf("Queue ID:%d, Priority:%d, Quantum:%d\n", Q[i].id, Q[i].priority, Q[i].quantum);
		}
		printf("\n");
		for(i = 0; i < y; i++){
			printf("Process ID:%d, Arrival:%d, Execution:%d, IO Burst Time:%d, IO Burst Interval:%d\n", P[i].id, P[i].arrival, P[i].execution, P[i].io_length, P[i].io_interval);
		}
	}
	*/

	return 0;
}
