//Implementing cp command 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

void main (int argc,char *argv[]){
	if(argc<3)
		printf("\n Insufficient arguments \n");
	else{
		if(argc==3){
		//Non-interactive
			int sourcefd=open(argv[1],O_RDWR);
			//Non-existent source file
			if(sourcefd==-1){
				printf("\n Source file does not exist \n");
			}
			else{
				//Reading source file
				char *tmpline=(char*)calloc(1000,sizeof(char));
				int readfd=read(sourcefd,tmpline,100);
					
				tmpline[readfd]='\0';
				
				//Creating destination file if non-existent
				int destfd=open(argv[2],O_CREAT|O_RDWR);
				if(destfd==-1)
					printf("\n Destination file could not be created \n");
				else{
					write(destfd,tmpline,strlen(tmpline));
					printf("\n Content copied successfully\n");
					close(destfd);
				}
				close(sourcefd);
			}
		}
		else{
			//Interactive
			int sourcefd=open(argv[2],O_RDWR);
			//Non-existent source file
			if(sourcefd==-1){
				printf("\n Source file does not exist \n");
			}
			else{
				//Reading source file
				char *tmpline=(char*)calloc(1000,sizeof(char));
				int readfd=read(sourcefd,tmpline,100);
					
				tmpline[readfd]='\0';
				
				//Creating destination file if non-existent
				int destfd=open(argv[3],O_CREAT|O_RDWR);
				if(destfd==-1)
					printf("\n Destination file could not be created \n");
				else{
					char opt;
					if(strcmp(argv[1],"-i")==0){
						printf("\n Copy contents? y/n ");
						scanf(" %c",&opt);
						if(opt=='Y'||opt=='y'){
							write(destfd,tmpline,strlen(tmpline));
							printf("\n Content copied successfully\n");
						}
						else{
							printf("\n Manual Abort. \n");
						}
					}
					else{
						write(destfd,tmpline,strlen(tmpline));
						printf("\n Content copied successfully\n");
					}
					close(destfd);
				}
				close(sourcefd);
			}
		}
	}
}

/*
Output:
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./mycp

 Insufficient arguments 
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./mycp -i source.txt 

 Source file does not exist 
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./mycp -i source.txt dest.txt

 Copy contents? y/n y

 Content copied successfully
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./mycp source.txt dest.txt

 Content copied successfully
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ cat source.txt
shiva
sharvan
shashuuuu
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ cat dest.txt
shiva
sharvan
shashuuuu

*/
