#include "Scheduling2Proto.h"

//Initialising the data members of each process
void initialise(Process *p){
	p->PID=(char*)malloc(100*sizeof(char));
	p->arrivalTime=0.0;
	p->burstTime=0.0;
	p->dummy=0.0;
	p->waitTime=0.0;
	p->turnTime=0.0;
	p->responseTime=-1.0;
	p->priority=0;
	p->nope=0;
	p->chance=0;
}

//Accepting data of each process
void acceptProcess(Process *p){
	printf("\n Enter Process ID: ");scanf(" %s",p->PID);
	printf("\n Enter arrival time: ");scanf("%lf",&p->arrivalTime);
	printf("\n Enter burst time: ");scanf("%lf",&p->burstTime);
	printf("\n Enter priority: ");scanf("%d",&p->priority);
	p->dummy=p->burstTime;
}

//Sorting on arrival time using Insertion sort
void sortOnArrivalTime(Process p[],int start_index,int end_index){
	for(int i=start_index;i<end_index;i++){
		Process key=p[i];
		int j=i-1;
		for(;j>=start_index&&key.arrivalTime<p[j].arrivalTime;j--)
			p[j+1]=p[j];
		p[j+1]=key;
	}
}

//Sorting based on Priority
void sortOnPriority(Process p[],int number_of_processes){

	for(int i=0;i<number_of_processes;i++){
		Process key=p[i];
		int j=i-1;
		for(;j>=0&&key.priority<p[j].priority;j--)
			p[j+1]=p[j];
		p[j+1]=key;
	}
}

//Display Gantt Chart
void displayGanttChart(char *Gantt_Chart[],int number_of_interval,double start_times[],double end_times[]){
	
	//Display top line
	printf("\n Gantt_Chart:\n");
	for(int i=0;i<number_of_interval;i++){
		printf("_______________");
	}
	
	//Display order of processes
	printf("\n|");
	for(int i=0;i<number_of_interval;i++)
		printf("_____%4s_____|",Gantt_Chart[i]);
	printf("\n");

	//Display time line
	int i=0;
	for(i=0;i<number_of_interval;i++)
		printf("%-15.0lf",start_times[i]);
	printf("%-15.0lf",end_times[i-1]);
	printf("\n\n");
}

//Print Wait Time
void printWaitTime(Process P[],int number_of_processes){
	int i=0;
	double sum=0.0;
	printf("\n Wait Time:\n");
	for(i=0;i<number_of_processes;i++){
		printf(" %-5.2lf",P[i].waitTime);
		sum+=P[i].waitTime;
	}
	printf("\nAverage: %-5.2lf",sum/number_of_processes);
	printf("\n");
}

//Print Turnaround Time
void printTurnTime(Process P[],int number_of_processes){
	int i=0;
	double sum=0.0;
	printf("\n Turnaround Time:\n");
	for(i=0;i<number_of_processes;i++){
		printf(" %-5.2lf",P[i].turnTime);
		sum+=P[i].turnTime;
	}
	printf("\nAverage: %-5.2lf",sum/number_of_processes);
	printf("\n");
}

//Print Response Time
void printRespTime(Process P[],int number_of_processes){
	int i=0;
	double sum=0.0;
	printf("\n Response Time:\n");
	for(i=0;i<number_of_processes;i++){
		if(P[i].responseTime<0)
			P[i].responseTime=0.0;
		printf(" %-5.2lf",P[i].responseTime);
		sum+=P[i].responseTime;
	}
	printf("\nAverage: %-5.2lf",sum/number_of_processes);
	printf("\n");
}

//Non-Preemptive Priority Scheduling
/*
Logic:
1.Maintain arrays for start and end times of the intervals in the Gantt chart
2.Run a timer from 0 to maximum time elapsed
3.In each iteration, add the processes that have arrived to a temporary array, provided they are incomplete
4.Sort the processes in the temporary array based on their priorities
5.Insert the processes in the temporary array into the gantt chart. Set the inserted processes as complete
6.Assign start and end times for the intervals.
7.Move time to the end time of the intervals.
8.Repeat steps 3 to 7.
9.Compute wait, response and turnaround times
*/
void NonPriority(Process P[],int number_of_processes){
	//Total time of execution
	double sum=0;
	for(int i=0;i<number_of_processes;i++)
		sum+=P[i].burstTime;

	//Gantt chart
	char *Gantt_Chart[100];
	for(int i=0;i<100;i++)
		Gantt_Chart[i]=(char*)malloc(10*sizeof(char));

	//Step 1.
	//Start and end times of processes
	int interval=0;
	double start_times[100];
	double end_times[100];

	//Step 2.
	for(int time=0;time<sum;){
		
		Process tmp[100];
		for(int i=0;i<100;i++)
			initialise(&tmp[i]);

		//Step 3.
		int tctr=0;
		for(int i=0;i<number_of_processes;i++)
			if(P[i].arrivalTime<=time&&P[i].priority){
				tmp[tctr++]=P[i];
			}

		//Step 4.
		sortOnPriority(tmp,tctr);
		
		if(tctr==0){
			strcpy(Gantt_Chart[interval],"////");
			if(interval==0){
				start_times[interval]=0;
			}
			else{
				start_times[interval]=end_times[interval-1];
			}
		}
		else{
			for(int i=0;i<tctr;i++){
				//Step 5.
				strcpy(Gantt_Chart[interval],tmp[i].PID);
				
				//Step 6.
				if(interval==0){
					start_times[interval]=0;
				}
				else{
					start_times[interval]=end_times[interval-1];
				}
				end_times[interval]=start_times[interval]+tmp[i].burstTime;
				int j=0;
				for(j=0;j<number_of_processes;j++){
					if(strcmp(tmp[i].PID,P[j].PID)==0){
						//Step 9.
						P[j].priority=0;
						P[j].waitTime=start_times[interval]-P[j].arrivalTime;
						P[j].turnTime=P[j].waitTime+P[j].burstTime;
						P[j].responseTime=P[j].waitTime;
					}
				}
				interval++;
			}
		}
		//Step 7.
		time=end_times[interval-1];
	}
	displayGanttChart(Gantt_Chart,interval,start_times,end_times);
	printWaitTime(P,number_of_processes);
	printTurnTime(P,number_of_processes);
	printRespTime(P,number_of_processes);
}

//Preemptive Priority Scheduling
/*
Logic:
1.Maintain arrays for start and end times of the intervals in the Gantt chart
2.Run a timer from 0 to maximum time elapsed
3.In each iteration, add the processes that have arrived to a temporary array, provided they are incomplete
4.Sort the processes in the temporary array based on their priorities
5.Insert the process at the zeroth index of the temporary array into the gantt chart. 
	5.1 Decrement the burst time of the inserted process by the necessary amount
6.Assign start and end times for that interval
7.Increment value of time by 1
8.Repeat steps 3 to 7.
9.Compute wait, response and turnaround times
*/
void Priority(Process P[],int number_of_processes){
	//Total time of execution
	double sum=0;
	for(int i=0;i<number_of_processes;i++)
		sum+=P[i].burstTime;

	//Gantt chart
	char *Gantt_Chart[100];
	for(int i=0;i<100;i++)
		Gantt_Chart[i]=(char*)malloc(10*sizeof(char));

	//Step 1.
	//Start and end times of processes
	int interval=0;
	double start_times[100];
	double end_times[100];

	//Step 2.
	//Step 7. Note time++ instead of time = end_times[interval-1] in Non-preemptive Priority
	for(int time=0;time<sum;time++){
		int flag=0;

		Process tmp[100];
		for(int i=0;i<100;i++)
			initialise(&tmp[i]);

		//Step 3.
		int tctr=0;
		for(int i=0;i<number_of_processes;i++)
			if(P[i].arrivalTime<=time&&P[i].burstTime){
				tmp[tctr++]=P[i];
			}

		//Step 4.
		sortOnPriority(tmp,tctr);

		//Step 5.1
		for(int i=0;i<number_of_processes;i++){
			if(strcmp(tmp[0].PID,P[i].PID)==0)
				P[i].burstTime--;
		}

		//Step 5.
		//Step 6.
		if(interval==0){
			strcpy(Gantt_Chart[interval],tmp[0].PID);
			start_times[interval]=0;
			flag=1;
			interval++;
		}
		else{
			//Step 6.
			if(strcmp(Gantt_Chart[interval-1],tmp[0].PID)!=0){
				end_times[interval-1]=time;
				strcpy(Gantt_Chart[interval],tmp[0].PID);
				start_times[interval]=end_times[interval-1];
				flag=1;
				interval++;
			}
		}
		//Step 9.
		int j=0;
		for(j=0;j<number_of_processes;j++){
			if(flag&&strcmp(tmp[0].PID,P[j].PID)==0){
				P[j].waitTime+=start_times[interval-1]-P[j].arrivalTime;
				if(P[j].waitTime>0.0){
					P[j].nope++;
					P[j].waitTime-=(P[j].dummy-P[j].burstTime-P[j].nope);
					printf("\n%s %d",P[j].PID,P[j].nope);
					if(P[j].nope>1){
						P[j].waitTime-=P[j].nope;
					}
				}

				P[j].turnTime=P[j].waitTime+P[j].dummy;
				if(P[j].responseTime<0.0)
					P[j].responseTime=start_times[interval-1]-P[j].arrivalTime;
			}
		}
	}
	end_times[interval-1]=sum;
	displayGanttChart(Gantt_Chart,interval,start_times,end_times);
	printWaitTime(P,number_of_processes);
	printTurnTime(P,number_of_processes);
	printRespTime(P,number_of_processes);
}

//Round Robin Scheduling
/*
Logic:
1. Maintain arrays for the gantt chart, start and end times of the intervals
2. Maintain a queue object to serve as the Ready queue
3. Run a timer from 0 to maximum time elapsed
4. At each iteration, enqueue all the processed that have arrived, if they are incomplete, to the ready queue
5. If queue is empty, break the loop
6. Dequeue and add the process to the gantt chart. 
	6.1 Decrement the burst time of that process by the time quantum or remaining burst time, whichever is smaller.
7. If the process is still incomplete, add it back to the Ready Queue. 
	7.1 Ensure no process has more than one instance in the ready queue at any given point of time
8. Allot start and end times for that interval.
9. Move time to the end of that interval  
10. Repeat steps 4. to 9.
11. Compute wait times, turnaround times and response times. 
*/
void RoundRobin(Process P[],int number_of_processes,int tq){
	//Total time of execution
	double sum=0;
	for(int i=0;i<number_of_processes;i++)
		sum+=P[i].burstTime;
	
	//Gantt chart
	char *Gantt_Chart[100];
	for(int i=0;i<100;i++)
		Gantt_Chart[i]=(char*)malloc(10*sizeof(char));
	
	//Step 1.
	//Start and end times of processes
	int interval=0;
	double start_times[100];
	double end_times[100];
	
	//Step 2.
	Queue RQ;
	initialiseQueue(&RQ);
	
	//Step 3.
	for(int time=0;time<sum;){
		
		//Step 4.
		for(int i=0;i<number_of_processes;i++){
			if(isEmpty(&RQ)){
				if(P[i].arrivalTime<=time&&P[i].burstTime>0&&P[i].chance==0){
					enqueue(&RQ,P[i]);
					P[i].chance=1;
				}
			}
			else{
				if(P[i].arrivalTime<=time&&P[i].burstTime>0&&P[i].chance==0&&!checkQueue(P[i],&RQ)){
					enqueue(&RQ,P[i]);
					P[i].chance=1;
				}
			}
		}
		
		Process DQ;
		initialise(&DQ);
		
		//Step 5.
		if(isEmpty(&RQ))
			break;
		while(isEmpty(&RQ)==0){
			//Step 6.
			DQ=dequeue(&RQ);
			strcpy(Gantt_Chart[interval],DQ.PID);
			//Step 8.
			if(interval==0){
				start_times[interval]=0;
			}
			else{
				start_times[interval]=end_times[interval-1];
			}

			//Step 6.1
			//Step 8.
			for(int i=0;i<number_of_processes;i++){
				if(strcmp(P[i].PID,Gantt_Chart[interval])==0){
					if(P[i].burstTime<tq)
						end_times[interval]=start_times[interval]+P[i].burstTime;
					else
						end_times[interval]=start_times[interval]+tq;
					P[i].burstTime-=tq;
					P[i].burstTime=(P[i].burstTime<0)?0:P[i].burstTime;
				}
			}

			//Step 9.
			time=end_times[interval];
			interval++;

			//Step 11.
			int j=0;
			for(j=0;j<number_of_processes;j++){
				if(strcmp(DQ.PID,P[j].PID)==0){
					
					P[j].waitTime+=start_times[interval-1]-P[j].arrivalTime;

					if(P[j].burstTime>0)
						P[j].nope++;
					if(P[j].waitTime>0.0){
						P[j].waitTime-=(P[j].dummy-P[j].burstTime-P[j].nope);
						if(P[j].waitTime<0.0)
							P[j].waitTime+=(P[j].dummy-P[j].burstTime);
						if(P[j].nope>=1){
							P[j].waitTime-=P[j].nope;
						}
					}

					P[j].turnTime=P[j].waitTime+P[j].dummy;
					if(P[j].responseTime<0.0)
						P[j].responseTime=start_times[interval-1]-P[j].arrivalTime;
				}
			}
			
			//Step 7.1 Note chance keeps track of number of instances of a process in the ready queue
			//Chance takes value 1 if the process is already in the ready queue, and 0 otherwise.
			for(int i=0;i<number_of_processes;i++){
				if(isEmpty(&RQ)){
					if(P[i].arrivalTime<=time&&P[i].burstTime>0&&P[i].chance==0){
						enqueue(&RQ,P[i]);
						P[i].chance=1;
					}
				}
				else{
					if(P[i].arrivalTime<=time&&P[i].burstTime>0&&P[i].chance==0&&!checkQueue(P[i],&RQ)){
						enqueue(&RQ,P[i]);
						P[i].chance=1;
					}
				}

			}
			//Step 7.
			for(int i=0;i<number_of_processes;i++){
				if(strcmp(DQ.PID,P[i].PID)==0)
					if(P[i].burstTime>0)
						enqueue(&RQ,P[i]);
			}
			
		}
		
	}
	end_times[interval-1]=sum;
	displayGanttChart(Gantt_Chart,interval,start_times,end_times);
	printWaitTime(P,number_of_processes);
	printTurnTime(P,number_of_processes);
	printRespTime(P,number_of_processes);
}