//Implementing ls command 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>

void main (int argc,char *argv[]){
	char *dirname=(char*)calloc(1000,sizeof(char));
	
	printf("\nEnter path of directory: ");
	scanf(" %s",dirname);
	
	DIR *dirstream=NULL;
	struct dirent *dirpointer=NULL;
	
	if(dirname[0]=='.'){
		getcwd(dirname,1000);
	}
	dirstream=opendir(dirname);
	if(dirstream==NULL){
		printf("\n Unable to open directory \n");
	}
	else{
		while(NULL != (dirpointer=readdir(dirstream))){
			printf("\n %s",dirpointer->d_name);
		}
		printf("\n");
		
		closedir(dirstream);
	}
} 

/*
Output:
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OS/SystemCalls$ ./myls

Enter path of directory: .

 mycp.c
 2-ex2-cmds.pdf
 .
 dest.txt
 mycp
 myls
 myls.c
 source.txt
 ..
*/
