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
#include "print.h"
#include "intQueue.h"
#define MAX 100
#define STAR0 8
#define STAR1 15
#define STAR2 10

typedef char String[13]; // arbitrary, increased to 13 in case all 3 inputs for a, b, c have 3 digits each.... ex. "100 100 100"
typedef char Str100[101];

typedef struct {
	int id;
	int arrival;
	int execution; // burst
	int io_length; // io burst time
	int io_interval; //io interval
	int completed; // how much is completed
	int relCompleted; // how much is completed relative to the quantum
} process;

typedef struct {
	int id;
	int priority;
	int quantum;
} pQueue;

// =======================================================================
// FUNCTIONS FOR PRINTING
// =======================================================================
void printQueue(pQueue Q){
	printStar(STAR0);
	printDivider(4);
	setColor(13);
	printf("QID: %3d ", Q.id);
	resetColor();
	printDivider(4);
	setColor(10);
	printf("Priority: %3d ", Q.priority);
	resetColor();
	printDivider(4);
	setColor(8);
	printf("Time Quantum: %3d ", Q.quantum);
	resetColor();
	printDivider(4);
	printf("\n");
}
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
	setColor(6);
	printf("IO Burst: %3d ", P.io_length);
	resetColor();
	printDivider(4);
	setColor(11);
	printf("IO Interval: %3d ", P.io_interval);
	resetColor();
	printf(" ");
	printDivider(4);
	printf("\n");
	
}
// =======================================================================
// FUNCTIONS FOR SORTING
// =======================================================================
void merge(process P[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    process L[n1], R[n2]; // temp array
 	
    // Copy data to temp array
    for (i = 0; i < n1; i++)
        L[i] = P[l + i];
    for (j = 0; j < n2; j++)
        R[j] = P[m + 1 + j];
        
    // merge
    i=0;
    j=0;
    k=l;
    while (i < n1 && j < n2) {
        if (L[i].arrival <= R[j].arrival)
            P[k++] = L[i++];
        else
            P[k++] = R[j++];
    }
    // copy remaining
    while (i < n1) 
        P[k++] = L[i++];
 
    while (j < n2) 
        P[k++] = R[j++];
    
}

void mSortByArrival(process P[], int l, int r)
{
    if (l < r) {
 		int m = (l + r) / 2;
        mSortByArrival(P, l, m);
        mSortByArrival(P, m + 1, r);
        merge(P, l, m, r);
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

void mlfq(process P[], pQueue q[], int nQ, int nP, int z, int boostTime){ 
// nP = num of processes, nQ = num of Queues // z is arbitrary for number of start/end times
	int debug = 0;
	int i, j, l, s, w, x;
	int st[nP][z], et[nP][z], seqn[nP][z];  // cpu start time, cpu end time, start/end queue number
	int ist[nP][z], iet[nP][z]; //io start time, io end time
	int wt[nP], tt[nP]; //  wait time, turnaround time
	int sei[z]; // cpu start/end index
	int ioi[z]; // io start/end index
	int ioc[nP]; // IO completed by the process
	int done = 0; // counts how many processes were completed
	int level = 0; // highest priority queue
	int ct = 0; // current time
	int curr; // current process in the CPU
	int currIO; // current process in IO
	int boostCount = 0; // number of times priority boosting was done
	float awt; // ave waiting time
	bool jobReady, resumeFromIO;
	Queue *ioQ[nQ]; // intQueue for IO
	Queue *Q[nQ]; // intQueue for CPU
	
	printf("Executing MLFQ...........\n");
	// initialize the queues
	for(i=0; i<nQ; i++)
	{
		Q[i] = createQueue(nP);
		ioQ[i] = createQueue(nP); 
	}
		
	// initialize start times and end times
	for(i=0; i<nP; i++){
		for(j=0; j<z; j++){
			st[i][j] = -1; // -1 is used to denote empty/vacant/null
			et[i][j] = -1;
			ist[i][j] = -1; 
			iet[i][j] = -1;
			seqn[i][j] = -1;
		}
	}
	// initialize start/end time indices 
	for(i=0;i<z; i++){
		sei[i] = -1;
		ioi[i] = -1;
	}
	// initialize completed IO burst for each process
	for(i=0;i<nP; i++)
		ioc[i] = 0;
	
	if(debug)
		printf("Initialize Done \n");
		
	i=0;
	while(done < nP) // while total completed processes < done total processes
	{
		level = 0;
		resumeFromIO = false; // to signal if a process from Higher Queue is returning from IO 
		
		while(i<nP && P[i].arrival <= ct && P[i].completed < P[i].execution)
		{ // enqueue all processes that have arrived
			enqueue(Q[level], i);
			i++;
		}
		
		while(level<nQ-1 && isEmpty(Q[level]) ) // check if higher queues are empty
			level++; // go to next level
		
		curr = dequeue(Q[level]); // the process to run in CPU
		
		if(debug)
		{
			printf("Current Time: %d, Current Process PID: %d, at Level: %d \n", ct, P[curr].id, level);
			printf("PID: %d		Completed: %d	Execution: %d\n", P[curr].id, P[curr].completed, P[curr].execution);
		}
			
		if(curr < 0)
			jobReady = false; // no job arrived
		else
			jobReady = true; // job/s arrived
			
		if(!jobReady){
			ct++;		
			if(debug) printf("No Process Arrived\n");
				
			continue;
		}	
		
		if(debug) printf("Process is ready for execution\n");
			
		for(j=0; j<q[level].quantum; j++) 
		{ // should not run for more than the quantum allotment
			if(j==0)
			{
				sei[curr]++; // record a new start/end time
				st[curr][sei[curr]] = ct; // start time
				seqn[curr][sei[curr]] = q[level].id; // QID where the the process ran
			}
			
			for(l=0; l<nQ; l++) // for each Level of Queue
			{ 
				int length = len(ioQ[l]);
				for(s=0; s<length; s++) 
				// for each process in that level of IO Queue, run it for 1ms
				{
					currIO = dequeue(ioQ[l]); 
					
					if(currIO >= 0)
					{
						if(ioc[currIO] == 0) // if process has just started in IO
							ist[currIO][ioi[currIO]+1] = ct; // io start time
							
						ioc[currIO]++; // process completes 1 io time
						
						if(ioc[currIO] < P[currIO].io_length) // if process has not completed IO burst
						{
							enqueue(ioQ[l], currIO); // process stays in IO queue 
							if(debug) printf("CT: %d,  Process PID: %d IS STAYING IN IO QUEUE\n", ct, P[currIO].id);
						}
						else // else if process completes io burst
						{
							enqueue(Q[l], currIO); // process returns to cpu queue
							if(debug) printf("CT: %d,  Process PID: %d IS RETURNING to PROCESS QUEUE %d\n", ct, P[currIO].id, l);			
							ioc[currIO] = 0; // reset IO completed to zero
							iet[currIO][ioi[currIO]+1] = ct+1; // io end time
							ioi[currIO]++; // record a new IO start/end time
						}	
					}
				}
			}

			ct++; // time passes by 
			P[curr].completed++; // process completes 1 ms
			P[curr].relCompleted++; // process completes 1 ms out of the allotted quantum
			if(debug)
			{
				printf("CT: %d,  Process PID: %d IS RAN for 1ms\n", ct, P[curr].id);
				printf("PID: %d		Completed: %d	Execution: %d\n", P[curr].id, P[curr].completed, P[curr].execution);
				printf("=============================\n");	
			}
			
			if(P[curr].completed == P[curr].execution)
				break; // break if the loop completes its full execution	
			
			if(P[curr].io_interval > 0 && P[curr].completed % P[curr].io_interval == 0 
			&& P[curr].relCompleted < q[level].quantum)
			// if process has to execute IO and it has not exceeded the quantum
			{
				enqueue(ioQ[level], curr); // move process to ioQ
				et[curr][sei[curr]] = ct; // end CPU time	
				if(debug) printf("CT: %d,  Process PID: %d IS GOING TO IO QUEUE\n", ct, P[curr].id);
				break; // break to get the next process while current process executes in IO
			}
			
			if(P[curr].relCompleted == q[level].quantum)
				break; // break if the process completes the quantum

			
			// check if there are higher priority processes
			int levelCheck;
			levelCheck = 0; // start by checking the highest queue
			while(levelCheck < level)
			{
				if(!isEmpty(Q[levelCheck]))
				{
					if(debug) printf("Queue is empty \n");
					enqueue(Q[level], curr); // pre-empt the current process
					P[curr].relCompleted = 0; // reset quantum used because moving to another queue
					resumeFromIO = true; // high prio process came from IO
					break; // high priority process must be executed now
				}
				else
					levelCheck++;
			}
		
			if(resumeFromIO)
				break; // break out of loop to run the high prio process in next iter
				
		}
			
		et[curr][sei[curr]] = ct; // end CPU time // This line will always run
		
		if(resumeFromIO)
			continue; // continue the outermost loop to enqueue the high prio process

		if(P[curr].completed < P[curr].execution && P[curr].relCompleted == q[level].quantum)
		// if process completes the quantum but not the full exec
		{
			P[curr].relCompleted = 0; // reset quantum used
			
			if(P[curr].io_interval > 0 && P[curr].completed % P[curr].io_interval == 0)
			{ // if process is entering IO
				if(level+1 < nQ) 
					enqueue(ioQ[level+1], curr); // enqueue to lower queue + run in IO
				else 
					enqueue(ioQ[level], curr); // enqueue at lowest queue + run in IO
			}
			else
			{
				if(level+1 < nQ)
					enqueue(Q[level+1], curr); // move to lower queue since it was not completed
				else 
					enqueue(Q[level], curr); // already at lowest queue
			}
			
			//et[curr][sei[curr]] = ct; // end CPU time

			if(debug)
			{
				printf("Moving To or Staying at Lowest Queue\n");
				printf("CT: %d	Level: %d\n", ct, level);
				printf("PID: %d		Completed: %d	Execution: %d\n", P[curr].id, P[curr].completed, P[curr].execution);	
			}
		}
		else if(P[curr].completed < P[curr].execution && P[curr].completed % q[level].quantum !=0)
		// if process is not complete and will not go to IO, just proceed to next iter basically
		{
			if(debug)
			{
				printf("Process PID: %d will return to queue after IO\n", P[curr].id);
				printf("CT: %d	Level: %d\n", ct, level);
				printf("PID: %d		Completed: %d	Execution: %d\n", P[curr].id, P[curr].completed, P[curr].execution);	
			}
		}
		else // process was completed 
		{
			done++; // the process was counted as completed
			if(debug) printf("Process PID: %d has been completed\n", P[curr].id);
		}
		
		// Priority Boost	
		if(boostCount < (ct / boostTime))
		{
			int d;
			for(l=1; l<nQ; l++)
			{	
				while(!isEmpty(Q[l]))
				{
					d = dequeue(Q[l]); // remove from lower queue
					P[d].relCompleted = 0; 
					enqueue(Q[0], d); // move to highest queue
				}
			}
			
			boostCount++; // priority boost was used
			if(debug)
			{
				printf("Priority Boosting!!!!\n");
				printQ(Q[0]); // prints the contents of the queue
			}
		}		
	}
	
	for(x=0; x<nP; x++) // compute turnaround time
		tt[x] = et[x][sei[x]] - P[x].arrival;
	
	for(x=0; x<nP; x++) // compute waiting time
	{
		wt[x] = st[x][0] - P[x].arrival;
		if(sei[x] > 0)
		{
			for(j=0; j<sei[x]; j++)
			{
				if(debug) printf("%d %d\n", st[x][j+1], et[x][j]);
				wt[x] += (st[x][j+1] - et[x][j]);
			}	
		}
		
	}
	
	awt = getAWT(wt, nP); // get average waiting time
	//Print
	printf("\n\nSimulation Complete!\n\n");
	for(x = 0; x < nP; x++){
		printStar(STAR2);
		
		setColor(13);
		printf("PID: %d\n", P[x].id);
		setColor(14);
		printf("CPU\n");
		resetColor();
		
		w = 0;
		while(st[x][w] != -1){
			printDivider(4);
			setColor(7);
			printf("%-5s: %3d ", "QID: ", seqn[x][w]);
			//printVDivider(STAR2);
			resetColor();
			
			printDivider(4);
			setColor(2);	
			printf("%-15s: %3d ", "Starting time", st[x][w]);
			resetColor();
			
			printDivider(4);
			setColor(3);
			printf("%-15s: %3d \n", "End time", et[x][w]);
			printVDivider(STAR2);
			resetColor();
			
			w++;
		}
		setColor(14);
		printf("IO\n");
		resetColor();
		w = 0;
		while(ist[x][w] != -1){
			printDivider(4);
			setColor(2);	
			printf("%-15s: %3d ", "Starting time", ist[x][w]);
			resetColor();
			
			printDivider(4);
			setColor(3);
			printf("%-15s: %3d \n", "End time", iet[x][w]);
			printVDivider(STAR2);
			resetColor();
			w++;
		}
		if(w==0) // if no IO time was printed
		{
			setColor(8);
			printf("This process did not run in IO\n");
			resetColor();
			printVDivider(STAR2);
		}
		setColor(14);
		printf("Summary\n");
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

int main(){
	FILE *fptr;
	Str100 filename; //filename can have max of 100 characters only
	String xys, abc_fghij;
	int x, y, s, i, j, k;
	pQueue Qtemp; // for sorting
	int min; // for sorting
	
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
		pQueue Q[x];
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
				pQueue q = {atoi(temp[0]), atoi(temp[1]), atoi(temp[2])};
				Q[i] = q;
				i++;
			}else{ // Scan Y lines of processes
				process p = {atoi(temp[0]), atoi(temp[1]), atoi(temp[2]), atoi(temp[3]), atoi(temp[4]), 0, 0};
				P[j] = p;
				j++;
			}	
		}
		
		fclose(fptr); // Scan Completed
		
		// Check Validity of first line input	
//		if(checkValid(x, y) == 0){
//			printf("Program will now end.");
//			return 0;
//		}
		
		// Sort Queue by Priority
		for(i=0; i<x-1; i++)
		{
			min = i;
			for(j=i+1; j<x; j++)
			{
				if(Q[j].priority < Q[min].priority)
				{
					Qtemp = Q[j];
					Q[j] = Q[min];
					Q[min] = Qtemp;
				}
			}
		}		
//		for(i = 0; i < x; i++){
//			printf("Queue ID:%d, Priority:%d, Quantum:%d\n", Q[i].id, Q[i].priority, Q[i].quantum);
//		}

//		printf("\n");
//		for(i = 0; i < y; i++){
//			printf("Process ID:%d, Arrival:%d, Execution:%d, IO Burst Time:%d, IO Burst Interval:%d, IO Completed:%d\n", 
//			P[i].id, P[i].arrival, P[i].execution, P[i].io_length, P[i].io_interval,  P[i].completed);
//		}

		mSortByArrival(P, 0, y-1); // sort processes by arrival
		for(i=0; i<x; i++)
			printQueue(Q[i]);
		printStar(STAR0);
		printf("\n");
		for(i=0; i<y; i++)
			printProcess(P[i]);
		printStar(STAR1);

		mlfq(P, Q, x, y, 100, s);
	}

	return 0;
}
