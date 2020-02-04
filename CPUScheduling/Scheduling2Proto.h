/*Develop a menu driven C program to implement the CPU Scheduling Algorithms
Priootity (Non-Preemptive and Preemptive) and Round Robin */
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>

//Struture representing each process
struct Job{
	char *PID;
	double arrivalTime;
	double burstTime;
	double dummy;     //Copy of burst time
	double waitTime;
	double turnTime;
	double responseTime;
	int priority;
	int nope;        //Number of pre-emptions
	int chance;      //Keep track of chance of process in Round Robin scheduling
};

typedef struct Job Process;

void initialise(Process *p);

void acceptProcess(Process *p);

void sortOnArrivalTime(Process p[],int start_index,int end_index);
void sortOnPriority(Process p[],int number_of_processes);

void displayGanttChart(char *Gantt_Chart[],int number_of_interval,double start_times[],double end_times[]);

void printWaitTime(Process P[],int number_of_processes);
void printTurnTime(Process P[],int number_of_processes);
void printRespTime(Process P[],int number_of_processes);

void NonPriority(Process P[],int number_of_processes);
void Priority(Process P[],int number_of_processes);
void RoundRobin(Process P[],int number_of_processes,int tq);

typedef struct{
	int front,rear;
	Process data[100];
	int capacity,size;
}Queue;

void initialiseQueue(Queue *q);

int isFull(Queue *q);

int isEmpty(Queue *q);

void enqueue(Queue *q,Process x);

Process dequeue(Queue *q);

void display(Queue *q);

void initialiseQueue(Queue *q){
	q->front=q->rear=-1;
	q->capacity=100;
	q->size=0;
}

int isFull(Queue *q){
	if((q->rear==q->capacity-1&&q->front==0)||(q->rear==q->front-1))
		return 1;
	else
		return 0;
}
	
int isEmpty(Queue *q){
	if(q->front==-1)
		return 1;
	else
		return 0;
}	

void enqueue(Queue *q,Process x){
	if(isFull(q))
		printf("Queue is full ");
	else{
		if(q->front==-1)
			q->front++;
			
		if(q->rear==q->capacity-1)
			q->rear=0;
		else
			q->rear++;
			
		q->size++;
		q->data[q->rear]=x;
	}
}

Process dequeue(Queue *q){
	Process x;
	initialise(&x);
	if(isEmpty(q))
		printf("Queue is empty");
	else{
		x=q->data[q->front];
		q->size--;
		if(q->front==q->rear)
			q->front=q->rear=-1;
		else if(q->front==q->capacity-1)
			q->front=0;
		else
			q->front++;
	}
	return x;
}

void display(Queue *q){
	if(isEmpty(q))
		printf("\nQueue is empty\n");
	else{
			int i=q->front;
			while(i!=q->rear){
				printf("%s ",q->data[i].PID);
				
				if(i==q->capacity-1)
					i=0;
				else
					i++;
			}
			
			printf("%s ",q->data[i].PID);	
	}
}

int checkQueue(Process p,Queue *q){
	if(isEmpty(q))
		printf("\nQueue is empty\n");
	else{
			int i=q->front;
			while(i!=q->rear){
				if(strcmp(p.PID,q->data[i].PID)==0)
					return 1;
				
				if(i==q->capacity-1)
					i=0;
				else
					i++;
			}
			
			if(strcmp(p.PID,q->data[i].PID)==0)
				return 1;
			return 0;	
	}
}