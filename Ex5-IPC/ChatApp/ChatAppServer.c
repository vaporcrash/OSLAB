//3. Develop an client/server chat application using shared memory.
//Server part

#include <sys/ipc.h> 
#define NULL 0
#include <sys/shm.h> 
#include <sys/types.h> 
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/wait.h>
#include <stdio_ext.h>
#include<fcntl.h>
#include<ctype.h>

int main(){
	//Creating shared memory location
	int shmemid=shmget(146,1000,IPC_CREAT|00666);
	
	//Buffer to read from/write onto the shared memory location
	char *file_read=(char*)calloc(1000,sizeof(char));	

	//Attaching to shared memory
	file_read=shmat(shmemid,NULL,0);
	
	while(strcmp(file_read,"end")!=0){

		//Sleeping to get contents of file from server
		printf("\nClient:%s\n",file_read);
		printf("\nYou: ");scanf(" %[^\n]",file_read);
		sleep(5);
		wait(NULL);
		
	}
	//Detaching from shared memory
	shmdt(file_read);

	//Deallocating the acquired memory location
	shmctl(shmemid,IPC_RMID,NULL);
}

/*
Output:
Client:

You: hi client

Client:hi server

You: hello client

Client:how r u

You: how u doinhh

Client:ggg

You: how about u

Client:im fine

You: nice
*/