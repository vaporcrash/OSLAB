//2.Develop an client / server application for file transfer using shared memory.
//Client part
//Child

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
	int pid;

	printf("\nClient: \n");
	//Creating shared memory location
	int shmemid=shmget(146,100,IPC_CREAT|00666);

	char *file_read=(char*)malloc(1000*sizeof(char));

	pid=fork();
	if(pid>0);
	else{
		sleep(5);
		printf("\nChild:\n");
		file_read=shmat(shmemid,NULL,0);
		printf("\n%s\n",file_read);
		shmdt(file_read);
	}
	shmctl(shmemid,IPC_RMID,NULL);
}