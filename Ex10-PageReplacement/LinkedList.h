#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define hole "H"

struct Box{
	int page_no;
	struct Box* next;
};

typedef struct Box Frame;

void initialise(Frame* f){
	
	f->page_no=-1;
	f->next=NULL;
}

//Copy one partition to another
void copyFrame(Frame *copy,Frame *OG){
	copy->page_no=OG->page_no;
	copy->next=NULL;
}

//Count number of partitions
int partitionCount(Frame* LL){
	Frame *tmp=LL;
	int ctr=0;
	tmp=tmp->next;
	while(tmp){
		ctr++;
		tmp=tmp->next;
	}
	return ctr;
}

//Insert into Linked List
void insert(Frame *LL, Frame *n){	
	
	Frame *stmp=LL;
	while(stmp->next){
		stmp=stmp->next;
	}
	n->next=stmp->next;
	stmp->next=n;
}

//Check if an element is present in the list
int checkLL(Frame *LL,int x){
	Frame *tmp=LL;
	tmp=tmp->next;
	while(tmp){
		if(x==tmp->page_no)
			return 1;
		tmp=tmp->next;
	}
	return 0;
}

//Delete first element of linked list
void deleteFirst(Frame *LL){
	Frame *tmp=LL;
	Frame *prev=tmp;
	tmp=tmp->next;
	if(tmp->next){
		prev->next=tmp->next;
		tmp->next=NULL;
		free(tmp);
	}
	else{
		printf("\n Empty\n");
	}
}


//Display Frames
void displayFrames(Frame *LL,int frames_reqd){
	
	int no_of_partitions=partitionCount(LL);

	int spacing = 27/frames_reqd;

	Frame *tmp=LL;
	tmp=tmp->next;

	for(int i=0;i<frames_reqd;i++){
		if(tmp==NULL){
			printf("%-5c",'-');
			continue;
		}
		else{
			printf("%-5d",tmp->page_no);
			tmp=tmp->next;
		}
	}
	for(int i=0;i<27-(5*frames_reqd);i++)
		printf(" ");
}

//Replace a page with another
void ReplacePos(Frame *LL,int old_page_no,int new_page_no){
	Frame *tmp=LL;

	tmp=tmp->next;

	while(tmp){
		if(tmp->page_no!=old_page_no){
			tmp=tmp->next;
		}
		else
			break;
	}
	tmp->page_no=new_page_no;
}