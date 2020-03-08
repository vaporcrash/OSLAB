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
void produce(char *input_string,int *empty,int *full,int *mutex,int *pctr,char *buff,int *cctr){
	int i=0;
	while (1){
	
		if(i>=strlen(input_string)){
			printf("\n Producer %d exited \n",getpid());
			wait(NULL);
			exit(1);
		}

		//Acquire semaphore empty
		printf("\nProducer %d trying to aquire Semaphore Empty \n",getpid());
		my_wait(empty);
		printf("\nProducer %d successfully aquired Semaphore Empty \n",getpid());
		
		//Acquire semaphore mutex
		printf("\nProducer %d trying to aquire Semaphore Mutex \n",getpid());
		my_wait(mutex);
		printf("\nProducer %d successfully aquired Semaphore Mutex \n",getpid());

		//Critical Section
		buff[*pctr]=input_string[i];
		
		printf("\nProducer %d Produced Item [ %c ] \n",getpid(),input_string[i]);
		i++;
		
		(*pctr)++;
		printf("\nItems in Buffer %d \n",*pctr-*cctr);
		
		//Release semaphore mutex
		my_signal(mutex);
		printf("\nProducer %d released Semaphore Mutex \n",getpid());
		
		//Release semaphore full
		my_signal(full);
		printf("\nProducer %d released Semaphore Full \n",getpid());

		//Remainder section
		sleep(2/random());
	}
}

// Consumer function
void consume(char *input_string,int *empty,int *full,int *mutex,int *cctr,char *buff,int *pctr){
	int i=0;
	while (1){
		if(i>=strlen(input_string)){
			printf("\n Consumer %d exited \n",getpid());
			exit(1);
		}

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
		
		printf("\nItems in Buffer %ld \n",strlen(buff)-*cctr);
		i++;
		
		//Release semaphore mutex
		my_signal(mutex);
		printf("\nConsumer %d released Semaphore Mutex \n",getpid());
		
		//Release semaphore empty
		my_signal(empty);
		printf("\nConsumer %d released Semaphore Empty \n",getpid());

		//Remainder Section
		sleep(1);
	}
} 

int main(){
	//ID for shared memory bufer
	int shmid;
	
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
	char *input_string=(char*)malloc(20*sizeof(char));
	
	//Buffer counters in producer and consumer respectively
	int pctr=0,cctr=0;

	pid_t temp_pid;

	//Acquiring memory for shared memory
	shmid   = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM );
	full_id = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);
	empty_id= shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);
	mutex_id= shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);

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
	printf("\nEnter string: ");scanf(" %s",input_string);
	printf("Entered string : %s",input_string);
	
	temp_pid=fork();
	if(temp_pid>0){
		produce(input_string,empty,full,mutex,&pctr,buff,&cctr);
	}
	else{ 
		consume(input_string,empty,full,mutex,&cctr,buff,&pctr);
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

/*
Output:

 Main Process 

Enter string: asdfgh
Entered string : asdfgh
Producer 3465 trying to aquire Semaphore Empty 

Producer 3465 successfully aquired Semaphore Empty 

Producer 3465 trying to aquire Semaphore Mutex 

Producer 3465 successfully aquired Semaphore Mutex 

Producer 3465 Produced Item [ a ] 

Items in Buffer 1 
Entered string : asdfgh

Producer 3465 released Semaphore Mutex 
Consumer 3466 trying to aquire Semaphore Full 

Producer 3465 released Semaphore Full 

Consumer 3466 successfully aquired Semaphore Full 

Consumer 3466 trying to aquire Semaphore Mutex 

Consumer 3466 successfully aquired Semaphore Mutex

Consumer 3466 Consumed Item [ a ] 


Producer 3465 trying to aquire Semaphore Empty 
Items in Buffer 0 


Producer 3465 successfully aquired Semaphore Empty 
Consumer 3466 released Semaphore Mutex 

Producer 3465 trying to aquire Semaphore Mutex 


Consumer 3466 released Semaphore Empty 
Producer 3465 successfully aquired Semaphore Mutex 

Producer 3465 Produced Item [ s ] 

Items in Buffer 2 

Producer 3465 released Semaphore Mutex 

Producer 3465 released Semaphore Full 

Producer 3465 trying to aquire Semaphore Empty 

Producer 3465 successfully aquired Semaphore Empty 

Producer 3465 trying to aquire Semaphore Mutex 

Producer 3465 successfully aquired Semaphore Mutex 

Producer 3465 Produced Item [ d ] 

Items in Buffer 3 

Producer 3465 released Semaphore Mutex 

Producer 3465 released Semaphore Full 

Producer 3465 trying to aquire Semaphore Empty 

Producer 3465 successfully aquired Semaphore Empty 

Producer 3465 trying to aquire Semaphore Mutex 

Producer 3465 successfully aquired Semaphore Mutex 

Producer 3465 Produced Item [ f ] 

Items in Buffer 4 

Producer 3465 released Semaphore Mutex 

Producer 3465 released Semaphore Full 

Producer 3465 trying to aquire Semaphore Empty 

Producer 3465 successfully aquired Semaphore Empty 

Producer 3465 trying to aquire Semaphore Mutex 

Producer 3465 successfully aquired Semaphore Mutex 

Producer 3465 Produced Item [ g ] 

Items in Buffer 5 

Producer 3465 released Semaphore Mutex 

Producer 3465 released Semaphore Full 

Producer 3465 trying to aquire Semaphore Empty 

Producer 3465 successfully aquired Semaphore Empty 

Producer 3465 trying to aquire Semaphore Mutex 

Producer 3465 successfully aquired Semaphore Mutex 

Producer 3465 Produced Item [ h ] 

Items in Buffer 6 

Producer 3465 released Semaphore Mutex 

Producer 3465 released Semaphore Full 

 Producer 3465 exited 

Consumer 3466 trying to aquire Semaphore Full 

Consumer 3466 successfully aquired Semaphore Full 

Consumer 3466 trying to aquire Semaphore Mutex 

Consumer 3466 successfully aquired Semaphore Mutex

Consumer 3466 Consumed Item [ s ] 

Items in Buffer 4 

Consumer 3466 released Semaphore Mutex 

Consumer 3466 released Semaphore Empty 

Consumer 3466 trying to aquire Semaphore Full 

Consumer 3466 successfully aquired Semaphore Full 

Consumer 3466 trying to aquire Semaphore Mutex 

Consumer 3466 successfully aquired Semaphore Mutex

Consumer 3466 Consumed Item [ d ] 

Items in Buffer 3 

Consumer 3466 released Semaphore Mutex 

Consumer 3466 released Semaphore Empty 

Consumer 3466 trying to aquire Semaphore Full 

Consumer 3466 successfully aquired Semaphore Full 

Consumer 3466 trying to aquire Semaphore Mutex 

Consumer 3466 successfully aquired Semaphore Mutex

Consumer 3466 Consumed Item [ f ] 

Items in Buffer 2 

Consumer 3466 released Semaphore Mutex 

Consumer 3466 released Semaphore Empty 

Consumer 3466 trying to aquire Semaphore Full 

Consumer 3466 successfully aquired Semaphore Full 

Consumer 3466 trying to aquire Semaphore Mutex 

Consumer 3466 successfully aquired Semaphore Mutex

Consumer 3466 Consumed Item [ g ] 

Items in Buffer 1 

Consumer 3466 released Semaphore Mutex 

Consumer 3466 released Semaphore Empty 

Consumer 3466 trying to aquire Semaphore Full 

Consumer 3466 successfully aquired Semaphore Full 

Consumer 3466 trying to aquire Semaphore Mutex 

Consumer 3466 successfully aquired Semaphore Mutex

Consumer 3466 Consumed Item [ h ] 

Items in Buffer 0 

Consumer 3466 released Semaphore Mutex 

Consumer 3466 released Semaphore Empty 

*/	
