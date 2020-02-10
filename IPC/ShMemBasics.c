//1.Develop an application for getting a name in parent and convert it into uppercase in child using shared memory. 
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
#include<ctype.h>


int main(){
	int pid;
	
	//Input buffer in parent
	char *parent_ip;
	parent_ip=(char*)malloc(100*sizeof(char));

	//Output buffer in child
	char *child_op;
	child_op=(char*)malloc(100*sizeof(char));

	//Creating shared memory location
	int shmemid=shmget(146,100,IPC_CREAT|00666);

	pid=fork();

	//Parent process
	if(pid>0){
		printf("\nParent Process:\n");
		
		//Attaching memory location
		parent_ip=shmat(shmemid,NULL,0);

		printf("\nEnter a string: \n");scanf(" %[^\n]",parent_ip);
		wait(NULL);
		shmdt(parent_ip);
		
	}
	//Child Process
	else{
		sleep(5);
		
		printf("\nChild Process:\n");
		
		//Attaching memory location 
		child_op=shmat(shmemid,NULL,0);

		printf("\nEntered string: %s\n",child_op);
		
		printf("\nString in upper case: ");
		int i=0;
		while(child_op[i]){
			putchar(toupper(child_op[i]));
			i++;
		}
		printf("\n");
		shmdt(child_op);
	}
	shmctl(shmemid,IPC_RMID,NULL);
}

/*
Output:
Parent Process:

Enter a string: 
hello

Child Process:

Entered string: hello

String in upper case: HELLO
*/