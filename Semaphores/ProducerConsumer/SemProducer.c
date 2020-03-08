//2. Modify the program as separate client / server process programs to generate ‘N’ random numbers in producer and write
//them into shared memory. Consumer process should read them from shared memory and display them in terminal

//Producer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h> // for semaphore operations sem_init,sem_wait,sem_post
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

extern int errno;

//Size of the shared buffer
#define SIZE 10 
//Size of shared variable=1 byte
#define VARSIZE 1 
//Maximum input limit
#define INPUTSIZE 20
//shared memory permissions 
#define SHMPERM 0666 

void my_wait(int *sem){
	while(*sem<=0);
	(*sem)--;
}

void my_signal(int *sem){
	(*sem)++;
}

// Producer function
void produce(int num,int *empty,int *full,int *mutex,int *pctr,char *buff,int *cctr,int N){

	
	while(N--){
		srand(N);
		num=rand()%10;
		//Acquire semaphore empty
		printf("\nProducer %d trying to aquire Semaphore Empty \n",getpid());
		my_wait(empty);
		printf("\nProducer %d successfully aquired Semaphore Empty \n",getpid());
	
		//Acquire semaphore mutex
		printf("\nProducer %d trying to aquire Semaphore Mutex \n",getpid());
		my_wait(mutex);
		printf("\nProducer %d successfully aquired Semaphore Mutex \n",getpid());

		//Critical Section
		buff[*pctr]=(char)(num+48);
	
		printf("\nProducer %d Produced Item [ %d ] \n",getpid(),num);
	
		(*pctr)++;
		printf("\nItems in Buffer %d \n",*pctr-*cctr);
	
		//Release semaphore mutex
		my_signal(mutex);
		printf("\nProducer %d released Semaphore Mutex \n",getpid());
	
		//Release semaphore full
		my_signal(full);
		printf("\nProducer %d released Semaphore Full \n",getpid());

		//Remainder section
		sleep(2);
	}
}

int main(){
	//ID for shared memory bufer
	int shmid;
	int N_id;
	N_id=shmget(100,SIZE,IPC_CREAT|IPC_EXCL|SHMPERM);
	int *N=(int*)malloc(sizeof(int));
	N=shmat(N_id,(int*)0,0);
	/*
	
	IMPORTANT:
	The semaphores, integer pointers in this case are all initialised to NULL for now, instead of the respective values they should be taking.
	Initialisation is done later. Refer further notes.
	
	*/

	//Semaphore empty
	int empty_id;
	int *empty=(int*)malloc(sizeof(int));
	
	//Semaphore full
	int full_id;
	int *full=(int*)malloc(sizeof(int));
	
	//Semaphore mutex
	int mutex_id;
	int *mutex=(int*)malloc(sizeof(int));
	
	//Buffer to read from/write onto the shared memory
	char *buff;

	//Input string
	int num;
	
	//Buffer counters in producer and consumer respectively
	int pctr=0,cctr=0;

	//Acquiring memory for shared memory
	shmid   = shmget(143, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM );
	full_id = shmget(144, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);
	empty_id= shmget(146, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);
	mutex_id= shmget(150, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);

	//Attaching buffer to memory location
	buff  = shmat(shmid,(char*)0,0);
	full  = shmat(full_id,(int*)0,0);
	empty = shmat(empty_id,(int*)0,0);
	mutex =shmat(mutex_id,(int*)0,0);

	//Initialising the semaphores
	/*
	
	IMPORTANT:
	Note that the initialisation is done after the attachment to the memory locations.
	Doing these two operations in reverse, for some yet unknown reason, leads to all the semaphores being initialised to 0. 
	
	*/
	
	*empty=SIZE;
	*full=0;
	*mutex=1;

	printf("\n Main Process \n");
	printf("\n Enter value of N: ");scanf("%d",N);
	
	produce(num,empty,full,mutex,&pctr,buff,&cctr,*N);
	
	//Detaching buffer from memory location 
	shmdt(buff);
	shmdt(mutex);
	shmdt(empty);
	shmdt(full);

	//Destroying acquired location
	shmctl(shmid, IPC_RMID, NULL);
	shmctl(mutex_id, IPC_RMID, NULL);
	shmctl(empty_id, IPC_RMID, NULL);
	shmctl(full_id, IPC_RMID, NULL);


	printf("\n Main process exited \n\n");
	return(0);
} 
