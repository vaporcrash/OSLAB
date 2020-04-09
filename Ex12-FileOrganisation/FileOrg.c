#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<fcntl.h>
#include<unistd.h>


struct Item{
	char *file_name;
	int start_address;
};

typedef struct Item File;

void initialiseFile(File *f){
	f->file_name=(char*)malloc(sizeof(File));
	f->start_address=-1;
}

struct base{
	char *root_name;
	int no_of_files;
	File file_list[5];
	int no_of_dir;
	struct base* dir_list[5];
	int start_address;
};

typedef struct base Root;

void initialiseRoot(Root *r){
	r->root_name=(char*)calloc(100,sizeof(char));
	r->no_of_files=0;
	for(int i=0;i<5;i++){
		initialiseFile(&r->file_list[i]);
	}
	r->no_of_dir=0;
	for(int i=0;i<5;i++){
		r->dir_list[i]=(Root*)malloc(sizeof(Root));
	}
	r->start_address=-1;
}

//Check if a file exists in a directory
int checkDirectory(char *dirname,char *file_name){
	
	DIR *dirstream=NULL;
	struct dirent *dirpointer=NULL;
	
	dirstream=opendir(dirname);
	if(dirstream==NULL){
		
		printf("\n Unable to open directory \n");
	}
	else{
		
		while(NULL != (dirpointer=readdir(dirstream))){
			
			if(strcmp(dirpointer->d_name,file_name)==0)
				return 1;
			
		}
		printf("\n");
		
		closedir(dirstream);
	}
	return 0;
}

//Single Level Directory
/*
Logic:
1. Create a directory to act as root. Set its start address as 0.
2. Create files inside this directory, use random number generation for file locations. 
*/
void SingleLevelDirectory(){
	srand(time(0));
	Root r;
	initialiseRoot(&r);

	char *dir_name=(char*)calloc(100,sizeof(char));
	strcat(dir_name,"SingleLevelDirectory");
	if(mkdir(dir_name,0777) ==-1){
		printf("\nError:Unable to find directory. \n");
	}
	else{
		strcpy(r.root_name,dir_name);
		r.start_address=0;
		int file_action;
		do{
			printf("\n Choose action: \n 1.Create File ");
			printf("\n 2.List Files \n 0.Back ");
			printf("\n Your choice: ");scanf("%d",&file_action);
			if(file_action==1){
				printf("\n Enter name of file: ");
				scanf(" %[^\n]",r.file_list[r.no_of_files].file_name);

				if(checkDirectory(dir_name,r.file_list[r.no_of_files].file_name)==0){
					char *path_name=(char*)calloc(100,sizeof(char));
					strcat(path_name,dir_name);
					strcat(path_name,"/");
					strcat(path_name,r.file_list[r.no_of_files].file_name);

					int file_fd = open(path_name,O_CREAT|O_RDWR,0664);
					if(file_fd==-1){
						printf("\nError: Unable to create file. \n");
						perror("open error");
					}
					else{
						int flag=0;
						int pos;
						do{
							flag=1;
							pos=rand()%49;
							pos++;
							for(int i=0;i<=r.no_of_files;i++){
								if(r.file_list[i].start_address==pos)
									flag=0;
							}
						}while(flag==0);

						r.file_list[r.no_of_files].start_address=pos;

						printf("\n File created! \n");
						close(file_fd);
						r.no_of_files++;
					}
				}
				else{
					printf("\nError: File name already exists. \n");
				}
			}
			else if(file_action==2){
				printf("\n%10s %10s\n","File name","Location");
				for(int i=0;i<r.no_of_files;i++){
					printf("\n%10s %10d\n",r.file_list[i].file_name,r.file_list[i].start_address);
				}
			}
			else if(file_action){
				printf("\n Invalid file action. \n");
			}
			else;
		}while(file_action);
	}
}

//Count the number of directories created in a given hierarchy.
int countDirectory(Root *r){
	int count=0;
	if(r!=NULL){
		count++;

		for(int i=0;i<r->no_of_dir;i++){
			count+=countDirectory(r->dir_list[i]);
		}
	}
	return count;
}

//Two Level Directory
/*
Logic:
1. Create a directory to act as root. Set its start address as 0.
2. Create directories inside this directory, count the number of directories,
   set start address allocating 50 units of memory to each directory created. 
3. Create files inside any of the directories, use random number generation for file location, 
   adding it to directory's start address.
*/
void TwoLevelDirectory(){
	Root *r=(Root*)malloc(sizeof(Root));
	initialiseRoot(r);

	char *dir_name=(char*)calloc(100,sizeof(char));
	strcat(dir_name,"TwoLevelDirectory");
	if(mkdir(dir_name,0777) ==-1){
		printf("\nError:Unable to find directory. \n");
	}
	else{
		strcpy(r->root_name,dir_name);
		r->start_address=0;
		int action;
		do{
			printf("\n Choose action: \n 1.Create directory \n 2.Create File ");
			printf("\n 3.Search Files \n 4.Display files \n 0.Back ");
			printf("\n Your choice: ");scanf("%d",&action);
			if(action==1){
				initialiseRoot(r->dir_list[r->no_of_dir]);
				
				printf("\n Enter name of directory: ");
				scanf(" %[^\n]",r->dir_list[r->no_of_dir]->root_name);
				
				if(checkDirectory(dir_name,r->dir_list[r->no_of_dir]->root_name)==1){
					printf("\n Error:Directory already exists. \n");
				}	
				else{
					char *path_name=(char*)calloc(100,sizeof(char));
					strcat(path_name,dir_name);strcat(path_name,"/");
					strcat(path_name,r->dir_list[r->no_of_dir]->root_name);
					
					if(mkdir(path_name,0777) ==-1){
						printf("\nError:Unable to create directory. \n");
					}
					else{
						int count=countDirectory(r);
						r->dir_list[r->no_of_dir]->start_address=(count-1)*50;
						printf("\n Directory created! \n");
						r->no_of_dir++;
					}
					
				}
			}
			else if(action==2){
				char *file_name=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of file: ");
				scanf(" %[^\n]",file_name);

				int dest;
				printf("\n Choose destination of file: \n 1.Root ");
				for(int i=0;i<r->no_of_dir;i++){
					printf("\n %d.%s ",i+2,r->dir_list[i]->root_name);
				}
				printf("\n 0.back \n Your choice: ");scanf("%d",&dest);
				if(dest==1){
					if(checkDirectory(dir_name,file_name)==0){
						char *path_name=(char*)calloc(100,sizeof(char));
						strcat(path_name,dir_name);
						strcat(path_name,"/");
						strcat(path_name,file_name);

						int file_fd = open(path_name,O_CREAT|O_RDWR,0664);
						if(file_fd==-1){
							printf("\nError: Unable to create file. \n");
							perror("open error");
						}
						else{
							strcpy(r->file_list[r->no_of_files++].file_name,file_name);

							int flag=0;
							int pos;
							do{
								flag=1;
								pos=rand()%49;
								pos++;
								for(int i=0;i<=r->no_of_files;i++){
									if(r->file_list[i].start_address== r->start_address+pos)
										flag=0;
								}
							}while(flag==0);

							r->file_list[r->no_of_files].start_address=r->start_address+pos;

							printf("\n File created! \n");
							close(file_fd);
						}
					}
					else{
						printf("\nError: File name already exists. \n");
					}
				}
				else if(dest>1 && dest-1<=r->no_of_dir){
					char *path_name=(char*)calloc(100,sizeof(char));
					strcat(path_name,dir_name);
					strcat(path_name,"/");
					strcat(path_name,r->dir_list[dest-2]->root_name);
					if(checkDirectory(path_name,file_name)==0){
						strcat(path_name,"/");
						strcat(path_name,file_name);

						int file_fd = open(path_name,O_CREAT|O_RDWR,0664);
						if(file_fd==-1){
							printf("\nError: Unable to create file. \n");
							perror("open error");
						}
						else{
							strcpy(r->dir_list[dest-2]->file_list[r->dir_list[dest-2]->no_of_files].file_name,file_name);

							int flag=0;
							int pos;
							do{
								flag=1;
								pos=rand()%49;
								pos++;
								for(int i=0;i<=r->no_of_files;i++){
									if(r->dir_list[dest-2]->file_list[i].start_address== r->dir_list[dest-2]->start_address+pos)
										flag=0;
								}
							}while(flag==0);

							r->dir_list[dest-2]->file_list[r->dir_list[dest-2]->no_of_files].start_address=r->dir_list[dest-2]->start_address+pos;

							printf("\n File created! \n");
							close(file_fd);
						}
					}
					else{
						printf("\nError: File name already exists. \n");
					}
				}
				else if(dest){
					printf("\n Invalid destination. \n");
				}
				else;
			}
			else if(action==3){
				char *file_name=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of file: ");
				scanf(" %[^\n]",file_name);
				if(checkDirectory(dir_name,file_name)){
					printf("\nFound in %s\n",dir_name);
				}
				else{
					int i=0;
					char *path_name=(char*)calloc(100,sizeof(char));
					strcat(path_name,dir_name);strcat(path_name,"/");
					for(i=0;i<r->no_of_dir;i++){
						char *path_name=(char*)calloc(100,sizeof(char));
						strcat(path_name,dir_name);strcat(path_name,"/");
						strcat(path_name,r->dir_list[i]->root_name);
						if(checkDirectory(path_name,file_name)){
							printf("\nFound in %s\n",r->dir_list[i]->root_name);
							break;
						}
					}
					if(i>=r->no_of_dir){
						printf("\n File not found. \n");
					}
				}
			}
			else if(action==4){
				printf("\nRoot: \n");
				printf("\n%10s %10s\n","File name","Location");
				for(int i=0;i<r->no_of_dir;i++){
					printf("\n%10s %10d\n",r->dir_list[i]->root_name,r->dir_list[i]->start_address);
				}
				for(int i=0;i<r->no_of_files;i++){
					printf("\n%10s %10d\n",r->file_list[i].file_name,r->file_list[i].start_address);
				}
				for(int i=0;i<r->no_of_dir;i++){
					printf("\n%s: \n",r->dir_list[i]->root_name);
					printf("\n%10s %10s\n","File name","Location");
					
					for(int j=0;j<r->dir_list[i]->no_of_files;j++){
						printf("\n%10s %10d\n",r->dir_list[i]->file_list[j].file_name,r->dir_list[i]->file_list[j].start_address);
					}
				}
			}
			else if(action){
				printf("\n Invalid action. \n");
			}
			else;
		}while(action);
	}
}

//Recursive function to create a directory, moving through the directory structure through the variable path_name
int createDirectory(Root *r,char *name_dir,char *path_name){
	int dest;
	if(path_name==NULL){
		printf("\nCurrent path: %s \n",r->root_name);
	}
	else{
		printf("\nCurrent path: %s \n",path_name);
	}
	printf("\n Choose destination path of directory: \n 1.%s ",r->root_name);
	for(int i=0;i<r->no_of_dir;i++){
		printf("\n %d.%s ",i+2,r->dir_list[i]->root_name);
	}
	printf("\n 0.back \n Your choice: ");scanf("%d",&dest);
	
	if(dest==1){
		initialiseRoot(r->dir_list[r->no_of_dir]);

		strcat(path_name,r->root_name);

		if(checkDirectory(r->root_name,name_dir)==1){
			printf("\nError: Directory already exists in %s. \n",r->root_name);
			return 0;
		}
		else{
			strcat(path_name,"/");
			strcat(path_name,name_dir);
			if(mkdir(path_name,0777) ==-1){
				printf("\nError:Unable to create directory. \n");
				return 0;
			}
			else{
				strcpy(r->dir_list[r->no_of_dir]->root_name,name_dir);
				printf("\n Directory created! \n");
				r->no_of_dir++;
				return 1;
			}
		}

	}
	else if(dest>1 && dest-1<=r->no_of_dir){
		strcat(path_name,r->root_name);strcat(path_name,"/");
		return createDirectory(r->dir_list[dest-2],name_dir,path_name);
	}
	else if(dest){
		printf("\nInvalid destination path. \n");
	}
	else;
	return 0;
	
}

//Recursive function to create a file, moving through the directory structure using the variable path_name
int createFile(Root *r,char *file_name,char *path_name){
	int dest;
	if(path_name==NULL){
		printf("\nCurrent path: %s \n",r->root_name);
	}
	else{
		printf("\nCurrent path: %s \n",path_name);
	}
	printf("\n Choose destination path of file: \n 1.%s ",r->root_name);
	for(int i=0;i<r->no_of_dir;i++){
		printf("\n %d.%s ",i+2,r->dir_list[i]->root_name);
	}
	printf("\n 0.back \n Your choice: ");scanf("%d",&dest);
	
	if(dest==1){
		initialiseRoot(r->dir_list[r->no_of_dir]);

		strcat(path_name,r->root_name);

		if(checkDirectory(r->root_name,file_name)==1){
			printf("\nError: File already exists in %s. \n",r->root_name);
			return 0;
		}
		else{
			strcat(path_name,"/");
			strcat(path_name,file_name);
			int file_fd=open(path_name,O_CREAT|O_RDWR,0664);
			if(file_fd==-1){
				printf("\nError: Unable to create file. \n");
				perror("open error");
				return 0;
			}
			else{
				strcpy(r->file_list[r->no_of_files].file_name,file_name);
				printf("\n File created! \n");
				close(file_fd);
				return 1;
			}
		}
	}
	else if(dest>1 && dest-1<=r->no_of_dir){
		strcat(path_name,r->root_name);strcat(path_name,"/");
		return createFile(r->dir_list[dest-2],file_name,path_name);
	}
	else if(dest){
		printf("\nInvalid destination path. \n");
	}
	else;
	return 0;
}

//Search for a file in the directory structure, giving it's path  
int searchFile(Root *r,char *file_name,char *path_name){
	int flag=0;
	strcat(path_name,r->root_name);
	if(checkDirectory(path_name,file_name)==1){
		printf("\nFound in %s\n",r->root_name);
		flag=1;
	}
	else{
		strcat(path_name,"/");
		for(int i=0;i<r->no_of_dir;i++)
			flag+= searchFile(r->dir_list[i],file_name,path_name);
	}
	return (flag>0)?1:0;
}

//Tree Structure
/*
Logic:
1. Create a directory to act as root. Set its start address as 0.
2. Create directories inside this directory, count the number of directories,
   set start address allocating 50 units of memory to each directory created. 
3. Create files inside any of the directories, use random number generation for file location, 
   adding it to directory's start address.
*/
void TreeStructure(){
	Root *r=(Root*)malloc(sizeof(Root));
	initialiseRoot(r);

	char *dir_name=(char*)calloc(100,sizeof(char));
	strcat(dir_name,"TreeStructure");
	if(mkdir(dir_name,0777) ==-1){
		printf("\nError:Unable to find directory. \n");
	}
	else{
		strcpy(r->root_name,dir_name);
		int action;
		do{
			printf("\n Choose action: \n 1.Create directory \n 2.Create File ");
			printf("\n 3.Search Files \n 0.Back ");
			printf("\n Your choice: ");scanf("%d",&action);
			if(action==1){
				char *name_dir=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of directory: ");
				scanf(" %[^\n]",name_dir);
				
				char *path_name=(char*)calloc(100,sizeof(char));

				createDirectory(r,name_dir,path_name);
			}
			else if(action==2){
				char *file_name=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of file: ");
				scanf(" %[^\n]",file_name);

				char *path_name=(char*)calloc(100,sizeof(char));

				createFile(r,file_name,path_name);
			}
			else if(action==3){
				char *file_name=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of file: ");
				scanf(" %[^\n]",file_name);
				
				char *path_name=(char*)calloc(100,sizeof(char));

				if(!searchFile(r,file_name,path_name))
					printf("\nFile not found. ");
			}
			else if(action){
				printf("\n Invalid action. \n");
			}
			else;
		}while(action);
	}
}

//Recursive function to set the destination path for a link to be created,
//moving through the directories using link_path_name
int chooseLinkDest(Root *r,char* link_name,char* link_path_name){
	
	int dest;
	if(link_path_name==NULL){
		printf("\nCurrent path: %s \n",r->root_name);
	}
	else{
		printf("\nCurrent path: %s \n",link_path_name);
	}

	printf("\n Choose destination path of file: \n 1.%s ",r->root_name);
	for(int i=0;i<r->no_of_dir;i++){
		printf("\n %d.%s ",i+2,r->dir_list[i]->root_name);
	}
	printf("\n 0.back \n Your choice: ");scanf("%d",&dest);
	
	if(dest==1){

		strcat(link_path_name,r->root_name);

		if(checkDirectory(r->root_name,link_name)==1){
			printf("\nError: File already exists in %s. \n",r->root_name);
			return 0;
		}
		else{
			return 1;
		}
	}
	else if(dest>1 && dest-1<=r->no_of_dir){
		
		strcat(link_path_name,r->root_name);strcat(link_path_name,"/");
		return chooseLinkDest(r->dir_list[dest-2],link_name,link_path_name);
	}
	else if(dest){
		printf("\nInvalid destination path. \n");
	}
	else;
	
	return 0;	
}

//DAG
/*
Logic:
1. Create a directory to act as root. Set its start address as 0.
2. Create directories inside this directory, count the number of directories,
   set start address allocating 50 units of memory to each directory created. 
3. Create files inside any of the directories, use random number generation for file location, 
   adding it to directory's start address.
*/
void DAG(){
	Root *r=(Root*)malloc(sizeof(Root));
	initialiseRoot(r);

	char *dir_name=(char*)calloc(100,sizeof(char));
	strcat(dir_name,"DAG");
	if(mkdir(dir_name,0777) ==-1){
		printf("\nError:Unable to find directory. \n");
	}
	else{
		strcpy(r->root_name,dir_name);
		int action;
		do{
			printf("\n Choose action: \n 1.Create directory \n 2.Create File ");
			printf("\n 3.Search Files \n 4.Create Link \n 0.Back ");
			printf("\n Your choice: ");scanf("%d",&action);
			if(action==1){
				char *name_dir=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of directory: ");
				scanf(" %[^\n]",name_dir);
				
				char *path_name=(char*)calloc(100,sizeof(char));

				createDirectory(r,name_dir,path_name);
			}
			else if(action==2){
				char *file_name=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of file: ");
				scanf(" %[^\n]",file_name);

				char *path_name=(char*)calloc(100,sizeof(char));

				createFile(r,file_name,path_name);
			}
			else if(action==3){
				char *file_name=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of file: ");
				scanf(" %[^\n]",file_name);
				
				char *path_name=(char*)calloc(100,sizeof(char));

				if(!searchFile(r,file_name,path_name))
					printf("\nFile not found. ");
			}
			else if(action==4){
				char *file_name=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of file to be linked: ");
				scanf(" %[^\n]",file_name);

				char *link_name=(char*)calloc(100,sizeof(char));
				printf("\n Enter name of link: ");
				scanf(" %[^\n]",link_name);

				char *file_path_name=(char*)calloc(100,sizeof(char));
				char *link_path_name=(char*)calloc(100,sizeof(char));

				strcat(file_path_name,"./");
				strcat(link_path_name,"./");

				
				if(chooseLinkDest(r,link_name,link_path_name)){
					if(searchFile(r,file_name,file_path_name)){
						
						strcat(file_path_name,"/");
						strcat(file_path_name,file_name);
						strcat(link_path_name,"/");
						strcat(link_path_name,link_name);
						
						if(symlink(file_path_name,link_path_name)!=0){
							perror("link() error");
	      					unlink(file_path_name);
						}
						else{
							printf("\nLink created! \n");
						}

					}
					else{
						printf("\nError: File not found. \n");
					}	
				}
			}
			else if(action){
				printf("\n Invalid action. \n");
			}
			else;
		}while(action);
	}
}

void main(){

	int option;
	do{
		printf("\n Choose option: \n 1.Single Level Directory \n 2.Two Level Directory ");
		printf("\n 3.Tree Structure \n 4.DAG \n 0.Exit \n Your choice: ");scanf("%d",&option);

		if(option==1){
			SingleLevelDirectory();
		}
		else if(option==2){
			TwoLevelDirectory();
		}
		else if(option==3){
			TreeStructure();
		}
		else if(option==4){
			DAG();
		}
		else if(option){
			printf("\n Invalid option. \n");
		}
		else;
	}while(option);
}

/*
Output:

 Choose option: 
 1.Single Level Directory 
 2.Two Level Directory 
 3.Tree Structure 
 4.DAG 
 0.Exit 
 Your choice: 1

 Choose action: 
 1.Create File 
 2.List Files 
 0.Back 
 Your choice: 1

 Enter name of file: sample


 File created! 

 Choose action: 
 1.Create File 
 2.List Files 
 0.Back 
 Your choice: 1

 Enter name of file: sample

Error: File name already exists. 

 Choose action: 
 1.Create File 
 2.List Files 
 0.Back 
 Your choice: 1      

 Enter name of file: sample1


 File created! 

 Choose action: 
 1.Create File 
 2.List Files 
 0.Back 
 Your choice: 2

 File name   Location

    sample         33

   sample1         26

 Choose action: 
 1.Create File 
 2.List Files 
 0.Back 
 Your choice: 0

 Choose option: 
 1.Single Level Directory 
 2.Two Level Directory 
 3.Tree Structure 
 4.DAG 
 0.Exit 
 Your choice: 2

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Display files 
 0.Back 
 Your choice: 1

 Enter name of directory: D1


 Directory created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Display files 
 0.Back 
 Your choice: 2

 Enter name of file: sample

 Choose destination of file: 
 1.Root 
 2.D1 
 0.back 
 Your choice: 1


 File created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Display files 
 0.Back 
 Your choice: 2

 Enter name of file: sample1

 Choose destination of file: 
 1.Root 
 2.D1 
 0.back 
 Your choice: 2


 File created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Display files 
 0.Back 
 Your choice: 3

 Enter name of file: sample

Found in TwoLevelDirectory

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Display files 
 0.Back 
 Your choice: 3

 Enter name of file: sample1


Found in D1

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Display files 
 0.Back 
 Your choice: 4

Root: 

 File name   Location

        D1         50

    sample         17

D1: 

 File name   Location

   sample1         73


 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Display files 
 0.Back 
 Your choice: 0

 Choose option: 
 1.Single Level Directory 
 2.Two Level Directory 
 3.Tree Structure 
 4.DAG 
 0.Exit 
 Your choice: 3

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 0.Back 
 Your choice: 1

 Enter name of directory: D1

Current path:  

 Choose destination path of directory: 
 1.TreeStructure 
 0.back 
 Your choice: 1


 Directory created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 0.Back 
 Your choice: 1

 Enter name of directory: D2

Current path:  

 Choose destination path of directory: 
 1.TreeStructure 
 2.D1 
 0.back 
 Your choice: 2

Current path: TreeStructure/ 

 Choose destination path of directory: 
 1.D1 
 0.back 
 Your choice: 1


 Directory created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 0.Back 
 Your choice: 2

 Enter name of file: sample

Current path:  

 Choose destination path of file: 
 1.TreeStructure 
 2.D1 
 0.back 
 Your choice: 1


 File created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 0.Back 
 Your choice: 2

 Enter name of file: sample1

Current path:  

 Choose destination path of file: 
 1.TreeStructure 
 2.D1 
 0.back 
 Your choice: 2

Current path: TreeStructure/ 

 Choose destination path of file: 
 1.D1 
 2.D2 
 0.back 
 Your choice: 1


 File created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 0.Back 
 Your choice: 2

 Enter name of file: sample2

Current path:  

 Choose destination path of file: 
 1.TreeStructure 
 2.D1 
 0.back 
 Your choice: 2

Current path: TreeStructure/ 

 Choose destination path of file: 
 1.D1 
 2.D2 
 0.back 
 Your choice: 2

Current path: TreeStructure/D1/ 

 Choose destination path of file: 
 1.D2 
 0.back 
 Your choice: 1


 File created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 0.Back 
 Your choice: 3

 Enter name of file: sample

Found in TreeStructure

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 0.Back 
 Your choice: 3

 Enter name of file: sample2



Found in D2

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 0.Back 
 Your choice: 3

 Enter name of file: sample1


Found in D1

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 0.Back 
 Your choice: 0

 Choose option: 
 1.Single Level Directory 
 2.Two Level Directory 
 3.Tree Structure 
 4.DAG 
 0.Exit 
 Your choice: 4

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Create Link 
 0.Back 
 Your choice: 1

 Enter name of directory: D1

Current path:  

 Choose destination path of directory: 
 1.DAG 
 0.back 
 Your choice: 1


 Directory created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Create Link 
 0.Back 
 Your choice: 2

 Enter name of file: sample

Current path:  

 Choose destination path of file: 
 1.DAG 
 2.D1 
 0.back 
 Your choice: 2

Current path: DAG/ 

 Choose destination path of file: 
 1.D1 
 0.back 
 Your choice: 1


 File created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Create Link 
 0.Back 
 Your choice: 3

 Enter name of file: sample


Found in D1

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Create Link 
 0.Back 
 Your choice: 4

 Enter name of file to be linked: sample

 Enter name of link: link

Current path: ./ 

 Choose destination path of file: 
 1.DAG 
 2.D1 
 0.back 
 Your choice: 1



Found in D1

Link created! 

 Choose action: 
 1.Create directory 
 2.Create File 
 3.Search Files 
 4.Create Link 
 0.Back 
 Your choice: 0

 Choose option: 
 1.Single Level Directory 
 2.Two Level Directory 
 3.Tree Structure 
 4.DAG 
 0.Exit 
 Your choice: 0
*/