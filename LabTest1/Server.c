//2. Implement using IPC such that the client request a file and server returns the first 5 lines of the file.
//Server part

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
	
	//Sleep while client requests for file
	sleep(2);
	
	printf("\n Entered file name: %s \n",file_read);
	
	//Attempt to open file
	int sourcefd=open(file_read,O_RDWR);
	
	if(sourcefd<0){
		printf(" File not found ");
	}
	else{
		
		//Buffer for reading from file
		char *tmpline=(char*)calloc(1000,sizeof(char));
		int readfd=read(sourcefd,tmpline,100);
		
		tmpline[readfd]='\0';
		
		char *token=(char*)calloc(100,sizeof(char));
		
		//Tokenise the file contents into lines
		token=strtok(tmpline,"\n");
		int lctr=0;
		while(lctr<5 && token!=NULL){
			if(lctr==0)
				strcpy(file_read,token);
			else
				strcat(file_read,token);
			strcat(file_read,"\n");
			token=strtok(NULL,"\n");
			lctr++;
		}
		printf("\n File contents read successfully\n");
	}
	
	//Detach buffer from shared memory
	shmdt(file_read);
	
	//Destroying acquired memory
	shmctl(shmemid,IPC_RMID,NULL);
	
	//Close opened file 
	close(sourcefd);
}
