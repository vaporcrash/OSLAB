#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

struct Item{
	char *file_name;
	int size;
	int no_of_blocks;
	int file_block_table[20];
	int start_block_id;
	int end_block_id;
};

typedef struct Item File;

struct Box{
	int block_id;
	struct Box* next;
	struct Box* next_file_block;
	File f;
};

typedef struct Box Block;

void initialiseFile(File *f){
	f->file_name=(char*)malloc(sizeof(20));
	strcpy(f->file_name,"Free");
	f->no_of_blocks=0;
	for(int i=0;i<20;i++)
		f->file_block_table[i]=-1;
	f->start_block_id=-1;
	f->end_block_id=-1;
}

void initialiseBlock(Block* b){
	
	b->block_id=-1;
	b->next=NULL;
	b->next_file_block=NULL;
	initialiseFile(&b->f);
}

void acceptFile(File *f){
	printf("\nEnter file name: ");scanf(" %[^\n]",f->file_name);
	printf("\nEnter file size: ");scanf("%d",&f->size);
}

void displayFile(File *f){
	printf("\n%s %d %d\n",f->file_name,f->size,f->no_of_blocks);
}

//Insert into Linked List
void insert(Block *LL, Block *n){	
	
	Block *stmp=LL;
	while(stmp->next){
		stmp=stmp->next;
	}
	n->next=stmp->next;
	stmp->next=n;
}

//Count number of contiguous free blocks from a given block
int countBlock(Block *LL,int block_id){
	Block *tmp=LL;
	tmp=tmp->next;
	while(tmp){
		if(tmp->block_id==block_id)
			break;
		else
			tmp=tmp->next;
	}

	int block_count=1;
	while(tmp){
		if(strcmp(tmp->f.file_name,"Free")!=0)
			break;
		else{
			block_count++;
			tmp=tmp->next;
		}
	}
	return block_count;
}

void Display(Block *LL){
	Block *tmp=LL;
	tmp=tmp->next;
	while(tmp){
		printf("%d %s\n",tmp->block_id,tmp->f.file_name);
		tmp=tmp->next;
	}
}

struct Dir{
	int no_of_files;
	File file_list[10];
};

typedef struct Dir Directory;

void initialiseDir(Directory *d){
	d->no_of_files=0;
	for(int i=0;i<10;i++)
		initialiseFile(&d->file_list[i]);
}