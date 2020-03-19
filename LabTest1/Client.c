//2. Implement using IPC such that the client request a file and server returns the first 5 lines of the file.
//Client part

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	
	//Buffer for working on shared memory
	char *file_read = (char*)calloc(1000,sizeof(char));
	
	//ID of shared memory
	int shmemid = shmget(146,1000,IPC_CREAT|00666);
	
	//Attaching buffer to shared memory
	file_read = shmat(shmemid,NULL,0);
	
	printf("\n Enter name of file to be read: ");scanf(" %[^\n]",file_read);
	
	//Sleep while server processes the request
	sleep(5);
	
	printf("\n First 5 lines of the file contents: \n");
	printf("%s",file_read);
	
	//Detach buffer from shared memory
	shmdt(file_read);
	
	//Destroying acquired memory
	shmctl(shmemid,IPC_RMID,NULL);
}
