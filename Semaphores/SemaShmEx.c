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

// Producer function
void produce(char *input_string,sem_t *empty,sem_t *full,sem_t *mutex,int *p,char *buff){
	int i=0;
	while (1){
	
		if(i>=strlen(input_string)){
			printf("\n Producer %d exited \n",getpid());
			wait(NULL);
			exit(1);
		}

		printf("\nProducer %d trying to aquire Semaphore Empty \n",getpid());
		sem_wait(empty);
		printf("\nProducer %d successfully aquired Semaphore Empty \n",getpid());
		
		printf("\nProducer %d trying to aquire Semaphore Mutex \n",getpid());
		sem_wait(mutex);
		printf("\nProducer %d successfully aquired Semaphore Mutex \n",getpid());

		buff[*p]=input_string[i];
		
		printf("\nProducer %d Produced Item [ %c ] \n",getpid(),input_string[i]);
		i++;
		
		(*p)++;
		printf("\nItems in Buffer %ld \n",strlen(buff));
		
		sem_post(mutex);
		printf("\nProducer %d released Semaphore Mutex \n",getpid());
		
		sem_post(full);
		printf("\nProducer %d released Semaphore Full \n",getpid());
		
		sleep(2/random());
	}
} 

// Consumer function
void consume(char *input_string,sem_t *empty,sem_t *full,sem_t *mutex,int *p,char *buff,int *c){
	int i=0;
	while (1){
		if(i>=strlen(input_string)){
			printf("\n Consumer %d exited \n",getpid());
			exit(1);
		}

		printf("\nConsumer %d trying to aquire Semaphore Full \n",getpid());
		sem_wait(full);
		printf("\nConsumer %d successfully aquired Semaphore Full \n",getpid());
		
		printf("\nConsumer %d trying to aquire Semaphore Mutex \n",getpid());
		sem_wait(mutex);
		printf("\nConsumer %d successfully aquired Semaphore Mutex\n",getpid());
		
		printf("\nConsumer %d Consumed Item [ %c ] \n",getpid(),buff[*c]);
		buff[*c]=' ';
		(*c)++;
		
		printf("\nItems in Buffer %ld \n",strlen(buff)-*c);
		i++;
		
		sem_post(mutex);
		printf("\nConsumer %d released Semaphore Mutex \n",getpid());
		
		sem_post(empty);
		printf("\nConsumer %d released Semaphore Empty \n",getpid());
		
		sleep(1);
	}
} 

int main(){
	//ID for shared memory bufer
	int shmid;
	
	//ID for empty semaphore
	int empty_id;
	//Semaphore empty
	sem_t *empty;

	//ID for full semaphore
	int full_id;
	//Semaphore full
	sem_t *full;

	//ID for mutex semaphore
	int mutex_id;
	//Semaphore mutex
	sem_t *mutex;

	//Buffer to read from/write onto the shared memory
	char *buff;
	//Input string
	char *input_string=(char*)malloc(20);
	
	int p=0,c=0;

	int i=0;
	pid_t temp_pid;

	//Acquiring memory for shared memory
	shmid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM );

	//Acquiring memory for semaphore empty
	empty_id=shmget(IPC_PRIVATE,sizeof(sem_t),IPC_CREAT|IPC_EXCL|SHMPERM);
	
	//Acquiring memory for semaphore full
	full_id=shmget(IPC_PRIVATE,sizeof(sem_t),IPC_CREAT|IPC_EXCL|SHMPERM);
	
	//Acquiring memory for semaphore mutex
	mutex_id=shmget(IPC_PRIVATE,sizeof(sem_t),IPC_CREAT|IPC_EXCL|SHMPERM);
	
	//Attaching buffers to memory locations
	buff = shmat(shmid,(char*)0,0);
	empty = shmat(empty_id,(char*)0,0);
	full = shmat(full_id,(char*)0,0);
	mutex = shmat(mutex_id,(char*)0,0);
	
	//Initializing Semaphores Empty, Full & Mutex
	sem_init(empty,1,SIZE);
	sem_init(full,1,0);
	sem_init(mutex,1,1);
	
	printf("\n Main Process Started \n");
	printf("\nEnter string: ");scanf(" %s",input_string);
	printf("Entered string : %s",input_string);
	
	temp_pid=fork();
	if(temp_pid>0){
		produce(input_string,empty,full,mutex,&p,buff);
	}
	else{ 
		consume(input_string,empty,full,mutex,&p,buff,&c);
	}
	
	//Detaching buffers from memory locations 
	shmdt(buff);
	shmdt(empty);
	shmdt(full);
	shmdt(mutex);

	//Destroying acquired locations
	shmctl(shmid, IPC_RMID, NULL);
	semctl( empty_id, 0, IPC_RMID, NULL);
	semctl( full_id, 0, IPC_RMID, NULL);
	semctl( mutex_id, 0, IPC_RMID, NULL);
	
	//Destroying acquired semaphores
	sem_destroy(empty);
	sem_destroy(full);
	sem_destroy(mutex);
	printf("\n Main process exited \n\n");
	return(0);
} 

/*
Output:

 Main Process Started 

Enter string: asdfgh
Entered string : asdfgh
Producer 6131 trying to aquire Semaphore Empty 

Producer 6131 successfully aquired Semaphore Empty 

Producer 6131 trying to aquire Semaphore Mutex 

Producer 6131 successfully aquired Semaphore Mutex 

Producer 6131 Produced Item [ a ] 

Entered string : asdfgh
Items in Buffer 1 

Consumer 6133 trying to aquire Semaphore Full 
Producer 6131 released Semaphore Mutex 


Producer 6131 released Semaphore Full 
Consumer 6133 successfully aquired Semaphore Full 

Consumer 6133 trying to aquire Semaphore Mutex 

Consumer 6133 successfully aquired Semaphore Mutex

Consumer 6133 Consumed Item [ a ] 

Items in Buffer 0 


Producer 6131 trying to aquire Semaphore Empty 
Consumer 6133 released Semaphore Mutex 


Producer 6131 successfully aquired Semaphore Empty 
Consumer 6133 released Semaphore Empty 

Producer 6131 trying to aquire Semaphore Mutex 

Producer 6131 successfully aquired Semaphore Mutex 

Producer 6131 Produced Item [ s ] 

Items in Buffer 2 

Producer 6131 released Semaphore Mutex 

Producer 6131 released Semaphore Full 

Producer 6131 trying to aquire Semaphore Empty 

Producer 6131 successfully aquired Semaphore Empty 

Producer 6131 trying to aquire Semaphore Mutex 

Producer 6131 successfully aquired Semaphore Mutex 

Producer 6131 Produced Item [ d ] 

Items in Buffer 3 

Producer 6131 released Semaphore Mutex 

Producer 6131 released Semaphore Full 

Producer 6131 trying to aquire Semaphore Empty 

Producer 6131 successfully aquired Semaphore Empty 

Producer 6131 trying to aquire Semaphore Mutex 

Producer 6131 successfully aquired Semaphore Mutex 

Producer 6131 Produced Item [ f ] 

Items in Buffer 4 

Producer 6131 released Semaphore Mutex 

Producer 6131 released Semaphore Full 

Producer 6131 trying to aquire Semaphore Empty 

Producer 6131 successfully aquired Semaphore Empty 

Producer 6131 trying to aquire Semaphore Mutex 

Producer 6131 successfully aquired Semaphore Mutex 

Producer 6131 Produced Item [ g ] 

Items in Buffer 5 

Producer 6131 released Semaphore Mutex 

Producer 6131 released Semaphore Full 

Producer 6131 trying to aquire Semaphore Empty 

Producer 6131 successfully aquired Semaphore Empty 

Producer 6131 trying to aquire Semaphore Mutex 

Producer 6131 successfully aquired Semaphore Mutex 

Producer 6131 Produced Item [ h ] 

Items in Buffer 6 

Producer 6131 released Semaphore Mutex 

Producer 6131 released Semaphore Full 

 Producer 6131 exited 

Consumer 6133 trying to aquire Semaphore Full 

Consumer 6133 successfully aquired Semaphore Full 

Consumer 6133 trying to aquire Semaphore Mutex 

Consumer 6133 successfully aquired Semaphore Mutex

Consumer 6133 Consumed Item [ s ] 

Items in Buffer 4 

Consumer 6133 released Semaphore Mutex 

Consumer 6133 released Semaphore Empty 

Consumer 6133 trying to aquire Semaphore Full 

Consumer 6133 successfully aquired Semaphore Full 

Consumer 6133 trying to aquire Semaphore Mutex 

Consumer 6133 successfully aquired Semaphore Mutex

Consumer 6133 Consumed Item [ d ] 

Items in Buffer 3 

Consumer 6133 released Semaphore Mutex 

Consumer 6133 released Semaphore Empty 

Consumer 6133 trying to aquire Semaphore Full 

Consumer 6133 successfully aquired Semaphore Full 

Consumer 6133 trying to aquire Semaphore Mutex 

Consumer 6133 successfully aquired Semaphore Mutex

Consumer 6133 Consumed Item [ f ] 

Items in Buffer 2 

Consumer 6133 released Semaphore Mutex 

Consumer 6133 released Semaphore Empty 

Consumer 6133 trying to aquire Semaphore Full 

Consumer 6133 successfully aquired Semaphore Full 

Consumer 6133 trying to aquire Semaphore Mutex 

Consumer 6133 successfully aquired Semaphore Mutex

Consumer 6133 Consumed Item [ g ] 

Items in Buffer 1 

Consumer 6133 released Semaphore Mutex 

Consumer 6133 released Semaphore Empty 

Consumer 6133 trying to aquire Semaphore Full 

Consumer 6133 successfully aquired Semaphore Full 

Consumer 6133 trying to aquire Semaphore Mutex 

Consumer 6133 successfully aquired Semaphore Mutex

Consumer 6133 Consumed Item [ h ] 

Items in Buffer 0 

Consumer 6133 released Semaphore Mutex 

Consumer 6133 released Semaphore Empty 

Consumer 6133 exited 
*/