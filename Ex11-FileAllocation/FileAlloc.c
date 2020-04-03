#include "LinkedList.h"

//Contiguous Allocation
/*
Logic:
For each file do the following:
1. Generate a random number between 1 to n.
2. Check for continuous number of needed file free blocks starting from that random block no.
3. If free then allot that file in those continuous blocks and update the directory structure.
4. If the block is not free, repeat from step 1.
5. If not enough continuous blocks are free then flag an error.
6. The Directory Structure should contain Filename, Starting Block, length (no. of blocks).
*/
void ContinguousAllocation(Block *MM,Directory *dir,int MM_block_count){
	srand(time(0));

	Block *tmp=MM;
	Block *start=tmp;
	int pos=-1;
	int block_count=0;
	int attempt_count=0;
	
	for(int i=0;i<dir->no_of_files;i++){
		
		attempt_count=0;
		do{
			
			if(attempt_count>=MM_block_count)
				break;
			attempt_count++;
			tmp=start->next;
			pos=rand()%MM_block_count;
			for(int j=0;j<pos;j++)
				tmp=tmp->next;
			block_count=countBlock(MM,tmp->block_id);
			
			
		}while(strcmp(tmp->f.file_name,"Free")!=0 || block_count < dir->file_list[i].no_of_blocks);
		
		if(attempt_count>=MM_block_count || pos<0 ){
			
			printf("\n Error: No enough memory for file %s. \n",dir->file_list[i].file_name);
			continue;
		}
		else{
			
			for(int j=0;j<dir->file_list[i].no_of_blocks;j++){
				
				strcpy(tmp->f.file_name,dir->file_list[i].file_name);
				tmp=tmp->next;
			}
			dir->file_list[i].start_block_id=pos;
			
		}
	}
	
	printf("\n Directory Structure: \n");
	printf("%-10s|%-12s|%-6s\n","File Name","Start Block","Length");
	for(int i=0;i<dir->no_of_files;i++){
		if( dir->file_list[i].start_block_id>=0)
			printf("%5s%5s|%6d%6s|%3d%3s\n",dir->file_list[i].file_name," ",dir->file_list[i].start_block_id," ",dir->file_list[i].no_of_blocks," ");
	}
	
}

//Linked Allocation
/*
Logic:

For each file do the following:
1. Generate a random number between 1 to n.
2. Check that block is free or not.
3. If free then allot it for file. Repeat step 1 to 3 for the needed number of blocks for the file and 
   create linked list in Main memory using the field Link to Next File block.
4. Update the Directory entry which contains Filename, Start block number, Ending Block Number.
5. Display the file blocks starting from start block number in Directory upto ending block number 
   by traversing the Main memory Linked list using the field Link to Next File block.
*/
void LinkedAllocation(Block *MM,Directory *dir,int MM_block_count){
	
	srand(time(0));

	Block *tmp=MM;
	Block *start=tmp;
	int pos=-1;
	int attempt_count=0;
	
	for(int i=0;i<dir->no_of_files;i++){
		
		for(int j=0;j<dir->file_list[i].no_of_blocks;j++){
			
			attempt_count=0;
			do{
				
				if(attempt_count>=MM_block_count)
					break;
				attempt_count++;
				tmp=start->next;
				pos=rand()%MM_block_count;
				for(int k=0;k<pos;k++)
					tmp=tmp->next;
				
			}while(strcmp(tmp->f.file_name,"Free")!=0);
			
			if(attempt_count>=MM_block_count || pos<0 ){
				
				printf("\n Error: No enough memory for file %s. \n",dir->file_list[i].file_name);
								
				if(j!=0){
					
					Block *del=MM;
					del=del->next;
					while(del){
						
						if(strcmp(del->f.file_name,dir->file_list[i].file_name)==0)
							strcpy(del->f.file_name,"Free");
						del=del->next;
					}
				}
				break;
			}
			else{
				
				strcpy(tmp->f.file_name,dir->file_list[i].file_name);
				tmp->next_file_block=NULL;
				if(j==0){
					dir->file_list[i].start_block_id=pos;
				}
				else{
					Block *ins=start->next;
					for(int k=0;k<dir->file_list[i].start_block_id;k++)
						ins=ins->next;
					while(ins->next_file_block)
						ins=ins->next_file_block;

					tmp->next_file_block=ins->next_file_block;
					ins->next_file_block=tmp;
				}
				if(j == dir->file_list[i].no_of_blocks-1){
					dir->file_list[i].end_block_id=pos;
				}
			}
		}
		
	}
	
	printf("\n Directory Structure: \n");
	printf("%-10s|%-12s|%-12s\n","File Name","Start Block","End Block");
	for(int i=0;i<dir->no_of_files;i++){
		if( dir->file_list[i].start_block_id>=0)
			printf("%5s%5s|%6d%6s|%6d%6s\n",dir->file_list[i].file_name," ",dir->file_list[i].start_block_id," ",dir->file_list[i].end_block_id," ");
	}
	
	for(int i=0;i<dir->no_of_files;i++){
		
		if(dir->file_list[i].start_block_id>=0){
			
			Block *tmp=MM;
			tmp=tmp->next;
			for(int j=0;j<dir->file_list[i].start_block_id;j++)
				tmp=tmp->next;
			printf("\nFile: %s\n",dir->file_list[i].file_name);
			while(tmp){
				printf("%d ",tmp->block_id);
				tmp=tmp->next_file_block;
			}
			printf("\n");
		}
	}
	
}

//Indexed Allocation
/*
Logic:
For each file do the following:
1. Generate a random number between 1 to n as a block for index block.
2. Check if it is free else repeat index block selection.
3. Generate needed number of free blocks in random order for the file and store those block numbers 
   in index block as array in File block table array.
4. Display the Directory structure which contains the filename and index blocknumber. Display the 
   File Details by showing the File Block Table in the index block.
*/
void IndexedAllocation(Block *MM,Directory *dir,int MM_block_count){

	srand(time(0));

	Block *tmp=MM;
	Block *start=tmp;
	int pos=-1;
	int attempt_count=0;
	
	for(int i=0;i<dir->no_of_files;i++){
		int file_block_ctr=0;
		for(int j=0;j<dir->file_list[i].no_of_blocks+1;j++){
			
			attempt_count=0;
			do{
				
				if(attempt_count>=MM_block_count)
					break;
				attempt_count++;
				tmp=start->next;
				pos=rand()%MM_block_count;
				for(int k=0;k<pos;k++)
					tmp=tmp->next;
				
			}while(strcmp(tmp->f.file_name,"Free")!=0);
			
			if(attempt_count>=MM_block_count || pos<0 ){
				
				printf("\n Error: No enough memory for file %s. \n",dir->file_list[i].file_name);
								
				if(j!=0){
					
					Block *del=MM;
					del=del->next;
					while(del){
						
						if(strcmp(del->f.file_name,dir->file_list[i].file_name)==0)
							strcpy(del->f.file_name,"Free");
						del=del->next;
					}
				}
				dir->file_list[i].start_block_id=-1;
				break;
			}
			else{
				
				strcpy(tmp->f.file_name,dir->file_list[i].file_name);
				
				if(j==0){
					dir->file_list[i].start_block_id=pos;
				}
				else{
					Block *ins=start->next;
					for(int k=0;k<dir->file_list[i].start_block_id;k++)
						ins=ins->next;
					ins->f.file_block_table[file_block_ctr++]=pos;
				}
			}
		}
		
	}
	
	printf("\n Directory Structure: \n");
	printf("%-10s|%-12s\n","File Name","Index Block");
	for(int i=0;i<dir->no_of_files;i++){
		if( dir->file_list[i].start_block_id>=0)
			printf("%5s%5s|%6d%6s\n",dir->file_list[i].file_name," ",dir->file_list[i].start_block_id," ");
	}
	
	for(int i=0;i<dir->no_of_files;i++){
		
		if(dir->file_list[i].start_block_id>=0){
			
			Block *tmp=MM;
			tmp=tmp->next;
			for(int j=0;j<dir->file_list[i].start_block_id;j++)
				tmp=tmp->next;
			printf("\nFile: %s\n",dir->file_list[i].file_name);
			for(int j=0;j<dir->file_list[i].no_of_blocks;j++)
				printf("%d ",tmp->f.file_block_table[j]);
			printf("\n");
		}
	}
}

void main(){
	int MMsize;
	int block_size;
	int MM_block_count;
	
	Directory dir;
	initialiseDir(&dir);

	printf("\nEnter size of Main Memory: ");scanf("%d",&MMsize);
	printf("\nEnter size of block: ");scanf("%d",&block_size);

	MM_block_count = MMsize/block_size;

	Block *MM=(Block*)malloc(sizeof(Block));
	initialiseBlock(MM);

	for(int i=0;i<MM_block_count;i++){
		Block *tmp=(Block*)malloc(sizeof(Block));
		initialiseBlock(tmp);
		tmp->block_id=i;
		insert(MM,tmp);
	}

	printf("\nEnter the number of files: ");scanf("%d",&dir.no_of_files);
	for(int i=0;i<dir.no_of_files;i++){
		acceptFile(&dir.file_list[i]);
		if(dir.file_list[i].size % block_size)
			dir.file_list[i].no_of_blocks = (dir.file_list[i].size / block_size)+1;
		else
			dir.file_list[i].no_of_blocks = (dir.file_list[i].size / block_size);
	}

	for(int i=0;i<dir.no_of_files;i++){
		displayFile(&dir.file_list[i]);
	}

	int option;
	do{
		printf("\n Select Allocation Algorithm: ");
		printf("\n 1.Contiguous Allocation ");
		printf("\n 2.Linked Allocation \n 3.Indexed Allocation ");
		printf("\n 0.Exit \n Your choice: ");scanf("%d",&option);

		if(option==1){
			ContinguousAllocation(MM,&dir,MM_block_count);
			
		}
		else if(option==2){
			LinkedAllocation(MM,&dir,MM_block_count);
		}
		else if(option==3){
			IndexedAllocation(MM,&dir,MM_block_count);
		}
		else if(option){
			printf("\n Invalid option. \n");
		}
		else;
	}while(option);
}


/*
Output:

Enter size of Main Memory: 500

Enter size of block: 10

Enter the number of files: 3

Enter file name: file1

Enter file size: 203

Enter file name: file2

Enter file size: 154

Enter file name: file3

Enter file size: 50

 Select Allocation Algorithm: 
 1.Contiguous Allocation 
 2.Linked Allocation 
 3.Indexed Allocation 
 0.Exit 
 Your choice: 1

 Directory Structure: 
File Name |Start Block |Length
file1     |    10      | 21   
file2     |    34      | 16   
file3     |     5      |  5  

 Select Allocation Algorithm: 
 1.Contiguous Allocation 
 2.Linked Allocation 
 3.Indexed Allocation 
 0.Exit 
 Your choice: 2

 Directory Structure: 
File Name |Start Block |End Block   
file1     |    45      |    39      
file2     |     6      |    23      
file3     |    30      |    37      

File: file1
45 3 25 22 41 38 49 8 36 42 46 35 5 18 31 14 19 21 12 11 39 

File: file2
6 2 24 15 9 0 13 16 20 44 40 29 17 32 33 23 

File: file3
30 27 48 34 37 

 Select Allocation Algorithm: 
 1.Contiguous Allocation 
 2.Linked Allocation 
 3.Indexed Allocation 
 0.Exit 
 Your choice: 3

 Directory Structure: 
File Name |Index Block 
file1     |     9      
file2     |    18      
file3     |    41      

File: file1
33 46 17 7 39 28 24 25 1 13 0 26 10 21 15 23 19 40 11 44 45 

File: file2
22 12 38 43 47 6 5 8 34 32 42 37 14 48 16 35 

File: file3
30 36 31 4 3 

 Select Allocation Algorithm: 
 1.Contiguous Allocation 
 2.Linked Allocation 
 3.Indexed Allocation 
 0.Exit 
 Your choice: 0

*/