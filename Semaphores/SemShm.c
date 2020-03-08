//To write a C program to create parent/child processes to implement the producer/consumer problem using semaphores in pthread library.

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

// Producer function
void produce(char *input_string,int *empty,int *full,int *mutex,int *pctr,char *buff){
	int i=0;
	while (1){
	
		if(i>=strlen(input_string)){
			printf("\n Producer %d exited \n",getpid());
			wait(NULL);
			exit(1);
		}

		printf("\nProducer Empty: %d Full: %d Mutex: %d \n",*empty,*full,*mutex);

		printf("\nProducer %d trying to aquire Semaphore Empty \n",getpid());
		my_wait(empty);
		printf("\nProducer %d successfully aquired Semaphore Empty \n",getpid());
		
		printf("\nProducer %d trying to aquire Semaphore Mutex \n",getpid());
		my_wait(mutex);
		printf("\nProducer %d successfully aquired Semaphore Mutex \n",getpid());

		buff[*pctr]=input_string[i];
		
		printf("\nProducer %d Produced Item [ %c ] \n",getpid(),input_string[i]);
		i++;
		
		(*pctr)++;
		printf("\nItems in Buffer %ld \n",strlen(buff));
		
		my_signal(mutex);
		printf("\nProducer %d released Semaphore Mutex \n",getpid());
		
		my_signal(full);
		printf("\nProducer %d released Semaphore Full \n",getpid());
		
		printf("\nProducer Empty: %d Full: %d Mutex: %d \n",*empty,*full,*mutex);
		sleep(2/random());
	}
}

// Consumer function
void consume(char *input_string,int *empty,int *full,int *mutex,int *cctr,char *buff){
	int i=0;
	while (1){
		if(i>=strlen(input_string)){
			printf("\n Consumer %d exited \n",getpid());
			exit(1);
		}

		printf("\nConsumer Empty: %d Full: %d Mutex: %d \n",*empty,*full,*mutex);

		printf("\nConsumer %d trying to aquire Semaphore Full \n",getpid());
		my_wait(full);
		printf("\nConsumer %d successfully aquired Semaphore Full \n",getpid());
		
		printf("\nConsumer %d trying to aquire Semaphore Mutex \n",getpid());
		my_wait(mutex);
		printf("\nConsumer %d successfully aquired Semaphore Mutex\n",getpid());
		
		printf("\nConsumer %d Consumed Item [ %c ] \n",getpid(),buff[*cctr]);
		buff[*cctr]=' ';
		(*cctr)++;
		
		printf("\nItems in Buffer %ld \n",strlen(buff)-*cctr);
		i++;
		
		my_signal(mutex);
		printf("\nConsumer %d released Semaphore Mutex \n",getpid());
		
		my_signal(empty);
		printf("\nConsumer %d released Semaphore Empty \n",getpid());
		
		printf("\nConsumer Empty: %d Full: %d Mutex: %d \n",*empty,*full,*mutex);
		sleep(1);
	}
} 

int main(){
	//ID for shared memory bufer
	int shmid;
	
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
	char *input_string=(char*)malloc(20*sizeof(char));
	
	//Buffer counters in producer and consumer respectively
	int pctr=0,cctr=0;

	pid_t temp_pid;

	//Acquiring memory for shared memory
	shmid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM );
	full_id=shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);
	empty_id=shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);
	mutex_id=shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);

	//Attaching buffer to memory location
	buff = shmat(shmid,(char*)0,0);
	full = shmat(full_id,(int*)0,0);
	empty = shmat(empty_id,(int*)0,0);
	mutex=shmat(mutex_id,(int*)0,0);

	*empty=SIZE;
	*full=0;
	*mutex=1;

	printf("\n Main Process Started \n");
	printf("\nEnter string: ");scanf(" %s",input_string);
	printf("Entered string : %s",input_string);
	
	temp_pid=fork();
	if(temp_pid>0){
		printf("\nProducer Empty: %d Full: %d Mutex: %d \n",*empty,*full,*mutex);
		produce(input_string,empty,full,mutex,&pctr,buff);
		printf("\n P:%d\n",*full);
	}
	else{ 
		printf("\n C:%d\n",*full);
		consume(input_string,empty,full,mutex,&cctr,buff);
	}
	
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