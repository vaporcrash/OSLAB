#include <pthread.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>

double avg;
int max;
int min;

void *average(void *val){
	int sum=0;
	int *ival=(int*)val;
	
	for(int i=1;i<ival[0];i++){
		sum+=ival[i];
	}

	avg=sum/(ival[0]-1);
	pthread_exit(0);
}

void *maximum(void *val){
	int *ival=(int*)val;
	max=ival[1];
	for(int i=1;i<ival[0];i++)
		if(max<ival[i])
			max=ival[i];

	pthread_exit(0);
}

void *minimum(void *val){
	int *ival=(int*)val;
	min=ival[1];
	for(int i=1;i<ival[0];i++)
		if(min>ival[i])
			min=ival[i];

	pthread_exit(0);
}

int main(int argc,char *argv[]){
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;

	if (argc < 2){
		fprintf(stderr,"usage: a.out <integer value>\n");
		return -1;
	}

	int *val = calloc(100,sizeof(int));
	val[0]=argc;
	for(int i=1;i<argc;i++){
		int x=atoi(argv[i]);
		val[i]=x;
	}
	
	pthread_create(&thread1,NULL,average,val);
	pthread_create(&thread2,NULL,maximum,val);
	pthread_create(&thread3,NULL,minimum,val);

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);

	printf("\nAverage is: %.2lf\n",avg);
	printf("\nMaximum value is: %d\n",max);
	printf("\nMinimum value is: %d\n",min);

}

/*
Output:

./a 45 35 24 61 85 94 12 10 

Average is: 45.00

Maximum value is: 94

Minimum value is: 10
*/