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

//Sorting using Insertion sort
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
void NonPriority(Process P[],int number_of_processes){
	//Total time of execution
	double sum=0;
	for(int i=0;i<number_of_processes;i++)
		sum+=P[i].burstTime;

	//Gantt chart
	char *Gantt_Chart[100];
	for(int i=0;i<100;i++)
		Gantt_Chart[i]=(char*)malloc(10*sizeof(char));

	//Start and end times of processes
	int interval=0;
	double start_times[100];
	double end_times[100];

	for(int time=0;time<sum;){
		
		Process tmp[100];
		for(int i=0;i<100;i++)
			initialise(&tmp[i]);

		int tctr=0;
		for(int i=0;i<number_of_processes;i++)
			if(P[i].arrivalTime<=time&&P[i].priority){
				tmp[tctr++]=P[i];
			}

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
				strcpy(Gantt_Chart[interval],tmp[i].PID);
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
						P[j].priority=0;
						P[j].waitTime=start_times[interval]-P[j].arrivalTime;
						P[j].turnTime=P[j].waitTime+P[j].burstTime;
						P[j].responseTime=P[j].waitTime;
					}
				}
				interval++;
			}
		}
		time=end_times[interval-1];
	}
	displayGanttChart(Gantt_Chart,interval,start_times,end_times);
	printWaitTime(P,number_of_processes);
	printTurnTime(P,number_of_processes);
	printRespTime(P,number_of_processes);
}

//Preemptive Priority Scheduling
void Priority(Process P[],int number_of_processes){
	//Total time of execution
	double sum=0;
	for(int i=0;i<number_of_processes;i++)
		sum+=P[i].burstTime;

	//Gantt chart
	char *Gantt_Chart[100];
	for(int i=0;i<100;i++)
		Gantt_Chart[i]=(char*)malloc(10*sizeof(char));

	//Start and end times of processes
	int interval=0;
	double start_times[100];
	double end_times[100];

	for(int time=0;time<sum;time++){
		int flag=0;

		Process tmp[100];
		for(int i=0;i<100;i++)
			initialise(&tmp[i]);

		int tctr=0;
		for(int i=0;i<number_of_processes;i++)
			if(P[i].arrivalTime<=time&&P[i].burstTime){
				tmp[tctr++]=P[i];
			}

		sortOnPriority(tmp,tctr);

		for(int i=0;i<number_of_processes;i++){
			if(strcmp(tmp[0].PID,P[i].PID)==0)
				P[i].burstTime--;
		}

		if(interval==0){
			strcpy(Gantt_Chart[interval],tmp[0].PID);
			start_times[interval]=0;
			flag=1;
			interval++;
		}
		else{
			if(strcmp(Gantt_Chart[interval-1],tmp[0].PID)!=0){
				end_times[interval-1]=time;
				strcpy(Gantt_Chart[interval],tmp[0].PID);
				start_times[interval]=end_times[interval-1];
				flag=1;
				interval++;
			}
		}
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
void RoundRobin(Process P[],int number_of_processes,int tq){
	//Total time of execution
	double sum=0;
	for(int i=0;i<number_of_processes;i++)
		sum+=P[i].burstTime;
	
	//Gantt chart
	char *Gantt_Chart[100];
	for(int i=0;i<100;i++)
		Gantt_Chart[i]=(char*)malloc(10*sizeof(char));
	
	//Start and end times of processes
	int interval=0;
	double start_times[100];
	double end_times[100];
	
	Queue RQ;
	initialiseQueue(&RQ);
	
	for(int time=0;time<sum;){
		
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
		if(isEmpty(&RQ))
			break;
		while(isEmpty(&RQ)==0){
		
			DQ=dequeue(&RQ);
			strcpy(Gantt_Chart[interval],DQ.PID);
			if(interval==0){
				start_times[interval]=0;
			}
			else{
				start_times[interval]=end_times[interval-1];
			}
			
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
			time=end_times[interval];
			interval++;
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