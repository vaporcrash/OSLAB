//2. Modify the program as separate client / server process programs to generate ‘N’ random numbers in producer and write
//them into shared memory. Consumer process should read them from shared memory and display them in terminal

//Consumer

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
#include<unistd.h>

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

// Consumer function
void consume(int num,int *empty,int *full,int *mutex,int *cctr,char *buff,int *pctr,int N){
	int i=0;
	while (N--){

		//Acquire semaphore full
		printf("\nConsumer %d trying to aquire Semaphore Full \n",getpid());
		my_wait(full);
		printf("\nConsumer %d successfully aquired Semaphore Full \n",getpid());
		
		//Acquire semaphore mutex
		printf("\nConsumer %d trying to aquire Semaphore Mutex \n",getpid());
		my_wait(mutex);
		printf("\nConsumer %d successfully aquired Semaphore Mutex\n",getpid());
		
		//Critical Section
		printf("\nConsumer %d Consumed Item [ %c ] \n",getpid(),buff[*cctr]);
		buff[*cctr]=' ';
		(*cctr)++;
		
		printf("\nItems in Buffer %d \n",*pctr-*cctr);
		i++;
		
		//Release semaphore mutex
		my_signal(mutex);
		printf("\nConsumer %d released Semaphore Mutex \n",getpid());
		
		//Release semaphore empty
		my_signal(empty);
		printf("\nConsumer %d released Semaphore Empty \n",getpid());

		//Remainder Section
		sleep(1);
		//if(i==*pctr)
		//	break;
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
	
	consume(num,empty,full,mutex,&pctr,buff,&cctr,*N);
	
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
