#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define hole "H"

struct Box{
	int start_byte;
	int end_byte;
	int size;
	char* status;
	struct Box* next;
};

typedef struct Box Node;

void initialise(Node* n){
	
	n->start_byte=-1;
	n->end_byte=-1;
	n->size=0;
	n->status=(char*)malloc(10);
	strcpy(n->status,hole);
	n->next=NULL;
}

//Accept details of partitions
void readNode(Node *n){
	printf("Enter the start and end address of partition: ");
	scanf("%d",&n->start_byte);scanf("%d",&n->end_byte);
	
	n->size=n->end_byte-n->start_byte;
}

//Copy one partition to another
void copyNode(Node *OG,Node *copy){
	copy->start_byte=OG->start_byte;
	copy->end_byte=OG->end_byte;
	copy->size=OG->size;
	strcpy(copy->status,OG->status);
	copy->next=NULL;
}

//Count number of partitions
int partitionCount(Node* LL){
	Node *tmp=LL;
	int ctr=0;
	tmp=tmp->next;
	while(tmp){
		ctr++;
		tmp=tmp->next;
	}
	return ctr;
}

//Insert into Linked List
void insert(Node *LL, Node *n){	
	
	Node *stmp=LL;
	while(stmp->next && n->start_byte >= stmp->next->start_byte){
		stmp=stmp->next;
	}
	n->next=stmp->next;
	stmp->next=n;
}

//Delete Node from Linked List
void delete(Node *LL,char *status){
	Node *tmp=LL;
	Node *prev=LL;
	tmp=tmp->next;
	while(strcmp(status,tmp->status)!=0){
		prev=tmp;
		tmp=tmp->next;
	}
	prev->next=tmp->next;
	free(tmp);
}

//Display memory
void displayMemory(Node *LL){
	
	int no_of_partitions=partitionCount(LL);

	Node *tmp=LL;
	tmp=tmp->next;

	if(tmp==NULL){
		printf("\nNULL\n");
	}
	else{
		//Display top line
		for(int i=0;i<no_of_partitions;i++){
			printf(" ______________  ");
		}
		
		//Display order of processes
		printf("\n");
		while(tmp){
			printf("|_____%4s_____| ",tmp->status);
			tmp=tmp->next;
		}
		printf("\n");

		//Display time line
		tmp=LL;
		tmp=tmp->next;
		while(tmp){
			printf("%-8d%8d ",tmp->start_byte,tmp->end_byte);
			tmp=tmp->next;
		}
		printf("\n\n");
	}
}