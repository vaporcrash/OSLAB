//Implementing grep command 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int isSubstring(char* s1, char* s2) 
{ 
    int M = strlen(s1); 
    int N = strlen(s2); 
  
    //Loop to slide pat[] one by one 
    for (int i = 0; i <= N - M; i++) { 
        int j; 
  
        // For current index i, check for pattern match 
        for (j = 0; j < M; j++) 
            if (s2[i + j] != s1[j]) 
                break; 
  
        if (j == M) 
            return i; 
    } 
  
    return -1; 
} 

void main (int argc,char *argv[]){
	if(argc<2)
		printf("\n Insufficient arguments \n");
	else{
		
		int filefd;
		//printf("\n%d\n", argc);

		if(argc==3)
			filefd=open(argv[2],O_RDWR);
		else
			filefd=open(argv[3],O_RDWR);
		
		//Non-existent source file
		if(filefd==-1){
			printf("\n Source file does not exist \n");
		}
		else{
			//Reading source file
			char *tmpline=(char*)calloc(1000,sizeof(char));
			int readfd=read(filefd,tmpline,100);
				
			tmpline[readfd]='\0';

			char *lines[100];
			for(int i=0;i<100;i++){
				lines[i]=(char*)malloc(sizeof(100));
			}
			int lctr=0;
			char* token=strtok(tmpline,"\n");
			while(token!=NULL){
				strcpy(lines[lctr++],token);
				token=strtok(NULL,"\n");
			}
			
			if(argc==3){
				for(int i=0;i<lctr;i++){
					if(isSubstring(argv[1],lines[i])!=-1)
						printf("\n%s\n",lines[i]);
				}
			}
			else{
				if(strcmp(argv[1],"-c")==0){
					int ctr=0;
					for(int i=0;i<lctr;i++){
						if(isSubstring(argv[2],lines[i])!=-1)
							ctr++;
					}
					printf("\n%d\n",ctr);
				}
				else if(strcmp(argv[1],"-v")==0){
					for(int i=0;i<lctr;i++){
						if(isSubstring(argv[2],lines[i])==-1)
							printf("\n%s\n",lines[i]);
					}
				}
				else if(strcmp(argv[1],"-n")==0){
					int ctr=0;
					for(int i=0;i<lctr;i++){
						if(isSubstring(argv[2],lines[i])!=-1){
							ctr++;							
							printf("\n%d %s\n",ctr,lines[i]);
						}
					}
				}
			}

			close(filefd);
		}
	}
}

/*
Output:
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./a hi source.txt

shiva
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./a -c hi source.txt

1
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./a -v hi source.txt

sharvan

shashuuuu
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./a -n hi source.txt

1 shiva
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ cat source.txt
shiva
sharvan
shashuuuu
*/