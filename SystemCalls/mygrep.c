//Implementing cp command 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

void main (int argc,char *argv[]){
	if(argc<2)
		printf("\n Insufficient arguments \n");
	else{
		int filefd=open(argv[2],O_RDWR);
		//Non-existent source file
		if(filefd==-1){
			printf("\n Source file does not exist \n");
		}
		else{
			//Reading source file
			char *tmpline=(char*)calloc(1000,sizeof(char));
			int readfd=read(filefd,tmpline,100);
				
			tmpline[readfd]='\0';
			int flag=0;
			for(int i=0;i<=(strlen(tmpline)-strlen(argv[1]));i++){
				flag=0;
				int j,k;
				for(j=0,k=i;j<strlen(argv[1])&&flag==0;j++,k++){
					if(tmpline[k]!=argv[1][j])
						flag=1;
				}
				if(j==strlen(argv[1])&&flag==0)
					printf("\n%s",tmpline);
			}
			close(filefd);
		}
	}
}

/*
Output:
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ cat source.txt
shiva
sharvan
shashuuuu

shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ gcc mygrep.c -o mygrep

shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./mygrep hi source.txt

shiva
sharvan
shashuuuu

shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./mygrep has source.txt
shiva
sharvan
shashuuuu

shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./mygrep hau source.txt
*/
			
