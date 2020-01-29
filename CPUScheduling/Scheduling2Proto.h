/*Develop a menu driven C program to implement the CPU Scheduling Algorithms
Priootity (Non-Preemptive and Preemptive) and Round Robin */
#include<stdio.h>
#include <stdlib.h>
#include<string.h>

//Struture representing each process
struct Job{
	char *PID;
	double arrivalTime;
	double burstTime;
	double dummy;
	double waitTime;
	double turnTime;
	double responseTime;
	int priority;
	int nope;
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
void RoundRobin(Process P[],int number_of_processes);