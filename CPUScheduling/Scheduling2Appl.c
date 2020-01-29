#include "Scheduling2Impl.h"

int main(){
	printf("\n\t\tCPU SCHEDULING ALGORITHMS\n");
	Process p[100];
	int number_of_processes;
int algo_option;
	do{
		printf("\nChoose your scheduling algorithm ");
		printf("\n1. Round Robin\n2. Priority\n0. Exit\n Your Choice: ");
		scanf("%d",&algo_option);
	
		//RoundRobin Scheduling
		if(algo_option==1){
			printf("\nEnter the number_of_processes:");scanf("%d",&number_of_processes);
			printf("\nEnter the details of the processes:");
	
			int i;
			for(i=0;i<number_of_processes;i++){
				initialise(&p[i]);
				acceptProcess(&p[i]);
			}
			
			Process RRp[100];
			for(i=0;i<number_of_processes;i++){
				initialise(&RRp[i]);
				RRp[i]=p[i];
			}
			printf("\n Round Robin Scheduling Output:\n ");
			RoundRobin(RRp,number_of_processes);
	
		}
		//Priority Scheduling
		else if(algo_option==2){
			printf("\nEnter the number_of_processes:");scanf("%d",&number_of_processes);
			printf("\nEnter the details of the processes:");
	
			int i;
			for(i=0;i<number_of_processes;i++){
				initialise(&p[i]);
				acceptProcess(&p[i]);
			}
		
			char preemp_option;
			printf("\n Use Pre-emption? y/n ");scanf(" %c",&preemp_option);
			//Non preemptive Priority Scheduling
			if(preemp_option=='n'||preemp_option=='N'){
			
				Process NPrip[100];
				for(i=0;i<number_of_processes;i++){
					initialise(&NPrip[i]);
					NPrip[i]=p[i];
				}
			
				printf("\n Non-preemptive SJF Scheduling Output:\n ");
				NonPriority(NPrip,number_of_processes);
			}
			//Preemptive Priority Scheduling
			else if(preemp_option=='y'||preemp_option=='Y'){
				Process PPrip[100];
				for(i=0;i<number_of_processes;i++){
					initialise(&PPrip[i]);
					PPrip[i]=p[i];
				}
			
				printf("\n Preemptive Priority Scheduling Output:\n ");
				Priority(p,number_of_processes);
			}
			else{
				printf("\n Invalid choice\n");
			}
		}
		else if(algo_option!=0){
			printf("\n Invalid option\n");
		}
		else;
	}while(algo_option);
}