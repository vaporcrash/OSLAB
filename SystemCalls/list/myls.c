//Implementing ls command 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<dirent.h>

void Recurse_ls(char *dirname){
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
		struct stat dirstat;
		while(NULL != (dirpointer=readdir(dirstream))){
			printf("%s\t",dirpointer->d_name);
		}
		printf("\n\n");
		dirpointer=NULL;
		while(NULL != (dirpointer=readdir(dirstream))){
			int dirfd=stat(dirpointer->d_name,&dirstat);
			if(dirfd<0)
				printf("\nUnable to locate file\n");
			else{
				int check_dir=S_ISDIR(dirstat.st_mode);
				if(check_dir){
					Recurse_ls(dirpointer->d_name);
				}
			}
		}
	}
}
void main (int argc,char *argv[]){
	
	char *dirname=(char*)calloc(1000,sizeof(char));
	
	printf("\nEnter path of directory: ");
	scanf(" %s",dirname);
	if(argc==1){	
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
	else if(strcmp(argv[1],"-l")==0){
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
			struct stat dirstat;
			while(NULL != (dirpointer=readdir(dirstream))){
		
				int dirfd=stat(dirpointer->d_name,&dirstat);
				if(dirfd<0)
					printf("\nUnable to locate directory\n");
				else{
					printf((S_ISDIR(dirstat.st_mode))?"d":"-");
					printf( (dirstat.st_mode & S_IRUSR) ? "r" : "-");
				    printf( (dirstat.st_mode & S_IWUSR) ? "w" : "-");
				    printf( (dirstat.st_mode & S_IXUSR) ? "x" : "-");
				    printf( (dirstat.st_mode & S_IRGRP) ? "r" : "-");
				    printf( (dirstat.st_mode & S_IWGRP) ? "w" : "-");
				    printf( (dirstat.st_mode & S_IXOTH) ? "x" : "-");
				    printf( (dirstat.st_mode & S_IXGRP) ? "x" : "-");
				    printf( (dirstat.st_mode & S_IROTH) ? "r" : "-");
				    printf( (dirstat.st_mode & S_IWOTH) ? "w" : "-");
					printf(" %ld ",dirstat.st_nlink);
					printf(" %6ld ",dirstat.st_size);
					printf(" %8ld ",dirstat.st_ino);
					printf(" %15s",dirpointer->d_name);
				}
				printf("\n");
			}
		}
	}
	else if(strcmp(argv[1],"-R")==0){
		Recurse_ls(dirname);
	}
} 

/*
Output:
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./myls

Enter path of directory: .

 mycp.c
 2-ex2-cmds.pdf
 mygrep.c
 .
 dest.txt
 CPUScheduling
 SYSTEM_CALLS_MANUAL.pdf
 a
 grep.c
 mycp
 myls
 ls
 mygrep
 myls.c
 source.txt
 ..
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./myls -l

Enter path of directory: .
-rw-rw--r- 1    2625   2363958           mycp.c
-rw-rw--r- 1   44937   3670881   2-ex2-cmds.pdf
-rw-r---r- 1    1358   2363973         mygrep.c
drwxr-xxr- 3    4096   2363810                .
-r--rw-xr- 1      24   2360766         dest.txt
drwxr-xxr- 2    4096   2368128    CPUScheduling
-rw-rw--r- 1   19896   3670880  SYSTEM_CALLS_MANUAL.pdf
-rwxrwxxr- 1   12920   2360763                a
-rw-r---r- 1    2506   2360758           grep.c
-rwxrwxxr- 1   12840   2360764             mycp
-rwxrwxxr- 1   12992   2360767             myls
-rwxrwxxr- 1   12992   2368119               ls
-rwxrwxxr- 1    8560   2363966           mygrep
-rw-r---r- 1    2864   2363972           myls.c
-rw-rw--r- 1      24   2363959       source.txt
drwxr-xxr- 6    4096   2363963               ..
shivanirudh@shiva-ideapad:~/Desktop/Semester4/OSLAB/SystemCalls$ ./myls -R

Enter path of directory: .
mycp.c	2-ex2-cmds.pdf	mygrep.c	.	dest.txt	CPUScheduling	SYSTEM_CALLS_MANUAL.pdf	a	grep.c	mycp	myls	ls	mygrep	myls.c	source.txt	..	
*/
