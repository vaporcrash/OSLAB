//2.Develop an client / server application for file transfer using shared memory.
//Client part

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

	printf("\nClient: \n");
	//Creating shared memory location
	int shmemid=shmget(146,1000,IPC_CREAT|00666);
	
	//Buffer to read from/write onto the shared memory location
	char *file_read=(char*)calloc(1000,sizeof(char));	

	//Attaching to shared memory
	file_read=shmat(shmemid,NULL,0);

	printf("\nEnter file name: ");
	scanf(" %s",file_read);
	
	//Sleeping to get contents of file from server
	sleep(5);
	printf("\nFile Contents:\n");
	printf("\n%s\n",file_read);

	//Detaching from shared memory
	shmdt(file_read);

	//Deallocating the acquired memory location
	shmctl(shmemid,IPC_RMID,NULL);
}

/*
Output:
Client: 

Enter file name: source.txt

File Contents:

asdfgf ;lkjhj
shiva
sharvan
shashu
siddharth
*/