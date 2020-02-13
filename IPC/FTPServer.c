//2.Develop an client / server application for file transfer using shared memory.
//Server part
//Parent 

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

	printf("\nServer: \n");
	//Creating shared memory location
	int shmemid=shmget(146,1000,IPC_CREAT|00666);

	//Buffer to read from/write on the shared memory
	char *file_read=(char*)calloc(1000,sizeof(char));
	
	//Sleeping to accept file name as input from client
	sleep(2);

	//Attaching to shared memory
	file_read=shmat(shmemid,NULL,0);
	printf("\nFile to be read: %s\n",file_read);

	//Opening file to be read
	int sourcefd=open(file_read,O_RDONLY);
	if(sourcefd==-1){
		printf("No source file");
	}
	else{
		//Reading file contents onto the buffer
		int readfd=read(sourcefd,file_read,100);
		file_read[readfd]='\0';
		printf("\nFile read successfully\n");
		wait(NULL);
		close(sourcefd);
		//Detaching buffer from shared memory
		shmdt(file_read);
	}
	
}
