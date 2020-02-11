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
	int pid;

	printf("\nServer: \n");
	//Creating shared memory location
	int shmemid=shmget(146,100,IPC_CREAT|00666);

	char *file_read=(char*)malloc(1000*sizeof(char));

	pid=fork();
	if(pid>0){
		printf("\nParent:\n");
		int sourcefd=open("source.txt",O_RDWR);
		if(sourcefd==-1){
			printf("No source file");
		}
		else{

			file_read=shmat(shmemid,NULL,0);
			int readfd=read(sourcefd,file_read,1000);
			file_read[readfd]='\0';
			printf("\nFile read:\n %s",file_read);
			wait(NULL);
			close(sourcefd);
			shmdt(file_read);
			//sleep(3);
		}
	}
	else;
	shmctl(shmemid,IPC_RMID,NULL);
}