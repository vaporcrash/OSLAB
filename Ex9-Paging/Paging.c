//To implement paging techniques

#include "Queue.h"

struct Job{
	char *PID;
	int size;
	int no_of_pages;
	int page_table[20];
};

typedef struct Job Process;

void initialiseProcess(Process *p){
	
	p->PID=(char*)malloc(sizeof(15));
	p->size=0;
	p->no_of_pages=0;
	for(int i=0;i<20;i++)
		p->page_table[i]=0;
}

void acceptProcess(Process *p){
	printf("\nEnter process ID: ");scanf(" %s",p->PID);
	printf("\nEnter size of process: ");scanf("%d",&p->size);
}

void processDeallocation(Process p[],int *no_of_processes,char *PID,Queue *FreeFrameList){
	int pno=0;
	for(pno=0;pno<*no_of_processes;pno++){
		if(strcmp(p[pno].PID,PID)==0)
			break;
	}
	if(pno>=*no_of_processes)
		printf("\nSpecified process not found. \n");
	else{
		for(int i=0;i<p[pno].no_of_pages;i++){
			enqueue(FreeFrameList,p[pno].page_table[i]);
		}

		for(int i=pno;i<(*no_of_processes)-1;i++)
			p[i]=p[i+1];
		*(no_of_processes)--;
	}
}

void main(){

	srand(time(0));
	int Mem_size;
	int page_size;
	int no_of_frames;
	//Initial number of free frames
	int noff;

	Process p[10];

	int ctr=0;

	Queue FreeFrameList;
	initialise(&FreeFrameList);

	printf("\n Enter size of Physical Memory: ");scanf("%d",&Mem_size);
	printf("\n Enter size of page: ");scanf("%d",&page_size);

	if(Mem_size%page_size){
		no_of_frames = (Mem_size/page_size)+1;
	}
	else{
		no_of_frames = (Mem_size/page_size);
	}

	noff=rand()%15;

	for(int i=0;i<noff;i++){
		int x = rand()%no_of_frames;
		if(!checkQueue(&FreeFrameList,x))
			enqueue(&FreeFrameList,x);
		else
			i--;
	}
	int option;
	do{
	
		printf("\n Enter option: \n 1.Process Request \n 2.Deallocation ");
		printf("\n 3.Display Page Table \n 4.Display Free Frame List ");
		printf("\n 0.Exit \n Your choice: ");scanf("%d",&option);

		if(option==1){
			initialiseProcess(&p[ctr]);
			acceptProcess(&p[ctr]);
			
			if(p[ctr].size % page_size)
				p[ctr].no_of_pages = (p[ctr].size/page_size) +1;
			else
				p[ctr].no_of_pages = (p[ctr].size/page_size);

			for(int i=0;i<p[ctr].no_of_pages;i++){
				int x=dequeue(&FreeFrameList);
				if(x<0){
					printf("\nProcess cannot be accomodated fully.\n");
					break;
				}
				p[ctr].page_table[i]=x;
			}
			ctr++;
		}
		else if(option==2){
			char *PID=(char*)malloc(sizeof(15));
			printf("\n Enter process ID to deallocate: ");
			scanf(" %s",PID);
			processDeallocation(p,&ctr,PID,&FreeFrameList);
		}
		else if(option==3){
			for(int i=0;i<ctr;i++){
				printf("\n Process %s: \n",p[i].PID);
				for(int j=0;j<p[i].no_of_pages;j++){
					printf("\n Page %d ---> Frame %d\n",j,p[i].page_table[j]);
				}
			}
		}
		else if(option==4){
			displayQueue(&FreeFrameList);
		}
		else if(option!=0){
			printf("\nInvalid option. \n");
		}
		else;

	}while(option);	

}

/*
Output:


 Enter size of Physical Memory: 32

 Enter size of page: 1

 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 4
30 11 19 26 18 5 31 3 2 
 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 1

Enter process ID: P1

Enter size of process: 4

 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 1

Enter process ID: P2

Enter size of process: 2

 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 1

Enter process ID: P3

Enter size of process: 2

 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 4
2 
 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 3

 Process P1: 

 Page 0 ---> Frame 30

 Page 1 ---> Frame 11

 Page 2 ---> Frame 19

 Page 3 ---> Frame 26

 Process P2: 

 Page 0 ---> Frame 18

 Page 1 ---> Frame 5

 Process P3: 

 Page 0 ---> Frame 31

 Page 1 ---> Frame 3

 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 2

 Enter process ID to deallocate: P2

 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 4
2 18 5 
 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 3

 Process P1: 

 Page 0 ---> Frame 30

 Page 1 ---> Frame 11

 Page 2 ---> Frame 19

 Page 3 ---> Frame 26

 Process P3: 

 Page 0 ---> Frame 31

 Page 1 ---> Frame 3

 
 Enter option: 
 1.Process Request 
 2.Deallocation 
 3.Display Page Table 
 4.Display Free Frame List 
 0.Exit 
 Your choice: 0
*/