#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct{
	int front,rear;
	int data[100];
	int capacity,size;
}Queue;

void initialise(Queue *q);

int isFull(Queue *q);

int isEmpty(Queue *q);

void enqueue(Queue *q,int x);

int dequeue(Queue *q);

void display(Queue *q);

void initialise(Queue *q){
	q->front=q->rear=-1;
	q->capacity=100;
	q->size=0;
}

int isFull(Queue *q){
	if((q->rear==q->capacity-1&&q->front==0)||(q->rear==q->front-1))
		return 1;
	else
		return 0;
}
	
int isEmpty(Queue *q){
	if(q->front==-1)
		return 1;
	else
		return 0;
}	

void enqueue(Queue *q,int x){
	if(isFull(q))
		printf("Queue is full ");
	else{
		if(q->front==-1)
			q->front++;
			
		if(q->rear==q->capacity-1)
			q->rear=0;
		else
			q->rear++;
			
		q->size++;
		q->data[q->rear]=x;
	}
}

int dequeue(Queue *q){
	int x=0;
	if(isEmpty(q))
		return -1;
	else{
		x=q->data[q->front];
		q->size--;
		if(q->front==q->rear)
			q->front=q->rear=-1;
		else if(q->front==q->capacity-1)
			q->front=0;
		else
			q->front++;
	}
	return x;
}

void displayQueue(Queue *q){
	if(isEmpty(q))
		printf("Queue is empty");
	else{
			int i=q->front;
			while(i!=q->rear){
				printf("%d ",q->data[i]);
				
				if(i==q->capacity-1)
					i=0;
				else
					i++;
			}
			
			printf("%d ",q->data[i]);	
	}
}

//Check if a value is already in the queue
int checkQueue(Queue *q,int x){
	if(isEmpty(q))
		return 0;
	else{
		int i=q->front;
		while(i!=q->rear){
			if(x==q->data[i])
				return 1;

			if(i==q->capacity-1)
				i=0;
			else
				i++;
		}
		if(x==q->data[i])
			return 1;
	}
	return 0;
}