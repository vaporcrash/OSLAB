#include "LinkedList.h"

//Accept data 
void acceptData(int *noff,int *frames_reqd,char *reference_string){
	printf("\nEnter the number of free frames: ");scanf("%d",noff);
	
	do{
		printf("\nEnter the number of frames required: ");
		scanf("%d",frames_reqd);
		if(*frames_reqd>*noff)
			printf("\nProcess requires more frames than are free. Cannot allocate. \n");
	}while(*frames_reqd>*noff);

	printf("\nEnter the reference string: ");scanf(" %[^\n]",reference_string);
}

//Split the string of references into integers
int refStringSplit(char* reference_string,int *page_references){
	int ctr=0;
	for(int i=0;reference_string[i];i++){
		if(reference_string[i]>='0' && reference_string[i]<='9')
			page_references[ctr++]=reference_string[i]-'0';
	}
	return ctr;
}

//Display the table of frames with each page reference
void displayTable(char page_ref_table[10][100],int frames_reqd,int col_ctr){

	//Top line
	for(int j=0;j<col_ctr;j++)
		printf("______");
	printf("\n");

	for(int i=0;i<frames_reqd;i++){

		for(int j=0;j<col_ctr;j++){
			printf("|_%2c_|",page_ref_table[i][j]);
		}
		printf("\n");
	}
}

//FIFO Algorithm
/*
page_fault: The page fault produced by the algorithm satisfying the given string. 
page_references: Array of page references from the reference string.
page_ref_count: Number of references in the string.
page_ref_table: Table to keep track of all the changes in the frames.
row_ctr,col_ctr: Indices to traverse the page reference table.
insert_flag: Flag to indicate whether an updation has occured in the frames. 

*/
void FIFOAlgo(int frames_reqd,char *reference_string){
	int page_fault=0;

	//List of frames
	Frame *LL=(Frame*)malloc(sizeof(Frame));
	initialise(LL);

	int page_references[100];
	int page_ref_count=refStringSplit(reference_string,page_references);

	char page_ref_table[10][100];
	for(int i=0;i<10;i++)
		for(int j=0;j<100;j++)
			page_ref_table[i][j]=' ';

	int row_ctr=0;
	int col_ctr=0;

	printf("\n FIFO Page Replacement Algorithm \n");
	printf("\nThe reference string is: %-50s\n",reference_string);

	printf("\n%-15s -> %-20s %5s -> %-12s\n","Page reference","Memory"," ","Page fault");
	int ctr=0;
	int insert_flag=0;

	/*
	This loop fills the frame list for the first time, having number of nodes equal 
	to that of number of frames required by the process.
	*/
	for(int i=0;i<frames_reqd && ctr<page_ref_count;i++){
		//New Frame to be inserted
		Frame* newFrame=(Frame*)malloc(sizeof(Frame));
		initialise(newFrame);

		insert_flag=0;

		//If page reference already exists in the frames
		if(!checkLL(LL,page_references[ctr])){
			newFrame->page_no = page_references[ctr];
			insert(LL,newFrame);
			insert_flag=1;
			page_fault++;
		}
		printf("\n%15d -> ",page_references[ctr]);
		displayFrames(LL,frames_reqd);
		printf("-> %-12d\n",page_fault);
		
		//Updation of the page reference table
		if(insert_flag){
			page_ref_table[row_ctr][col_ctr]='0'+page_references[ctr];
			
			for(int i=col_ctr+1;i<100;i++)
				page_ref_table[row_ctr][i] = page_ref_table[row_ctr][col_ctr];

			if(row_ctr==frames_reqd-1)
				row_ctr=0;
			else
				row_ctr++;
			col_ctr++;
		}

		ctr++;
	}

	/*
	This loop makes all the succesive updation to the frame list depending on the 
	specified algorithm.
	*/
	for(;ctr<page_ref_count;ctr++){
		Frame* newFrame=(Frame*)malloc(sizeof(Frame));
		initialise(newFrame);

		insert_flag=0;

		if(!checkLL(LL,page_references[ctr])){
			//Delete the first element and insert a new element, ie, FIFO
			deleteFirst(LL);
			newFrame->page_no = page_references[ctr];
			insert(LL,newFrame);
			insert_flag=1;
			page_fault++;
		}

		printf("\n%15d -> ",page_references[ctr]);
		//Display the updations to the frame list
		displayFrames(LL,frames_reqd);
		printf("-> %-12d\n",page_fault);

		//Update the page reference table
		if(insert_flag){
			page_ref_table[row_ctr][col_ctr]='0'+page_references[ctr];
			
			for(int i=col_ctr+1;i<100;i++)
				page_ref_table[row_ctr][i] = page_ref_table[row_ctr][col_ctr];

			if(row_ctr==frames_reqd-1)
				row_ctr=0;
			else
				row_ctr++;
			col_ctr++;
		}

	}
	displayTable(page_ref_table,frames_reqd,col_ctr);
	printf("\nTotal number of Page faults:%d \n",page_fault);

}


//Optimal Position
/*
Logic:
1.Maintain an array where each element represents the index of the next occurence of the 
corresponding element in the frame list.
2.Find the index of the maximum value in this array, which corresponds to the element that 
is not used in the near future.
3.Pass the page_no of the element in that index in the frame list.
*/

int OptimalPos(Frame *LL,int ctr,int page_references[100],int page_ref_count){
	//Optimal position array
	int opt_array[10];
	for(int i=0;i<10;i++)
		opt_array[i]=100;
	int arr_ctr=0;
	Frame *tmp=LL;
	tmp=tmp->next;

	//Compute occurences
	while(tmp){
		for(int i=ctr;i<page_ref_count;i++){
			if(page_references[i]==tmp->page_no){
				opt_array[arr_ctr]=i;
				break;
			}
		}
		tmp=tmp->next;
		arr_ctr++;
	}

	//Compute index of maximum value
	int maxid=0;
	for(int i=0;i<arr_ctr;i++){
		if(opt_array[i]>opt_array[maxid])
			maxid=i;
	}

	tmp=LL;
	tmp=tmp->next;
	while(maxid){
		tmp=tmp->next;maxid--;
	}
	return tmp->page_no;
}

//Optimal Algorithm
/*
page_fault: The page fault produced by the algorithm satisfying the given string. 
page_references: Array of page references from the reference string.
page_ref_count: Number of references in the string.
page_ref_table: Table to keep track of all the changes in the frames.
row_ctr,col_ctr: Indices to traverse the page reference table.
insert_flag: Flag to indicate whether an updation has occured in the frames. 

*/
void OptimalAlgo(int frames_reqd,char *reference_string){
	int page_fault=0;

	Frame *LL=(Frame*)malloc(sizeof(Frame));
	initialise(LL);

	int page_references[100];
	int page_ref_count=refStringSplit(reference_string,page_references);

	char page_ref_table[10][100];
	for(int i=0;i<10;i++)
		for(int j=0;j<100;j++)
			page_ref_table[i][j]=' ';

	int row_ctr=0;
	int col_ctr=0;

	printf("\n Optimal Page Replacement Algorithm \n");
	printf("\nThe reference string is: %-50s\n",reference_string);

	printf("\n%-15s -> %-20s %5s -> %-12s\n","Page reference","Memory"," ","Page fault");
	
	int ctr=0;
	int insert_flag=0;
	
	/*
	This loop fills the frame list for the first time, having number of nodes equal 
	to that of number of frames required by the process.
	*/
	for(int i=0;i<frames_reqd && ctr<page_ref_count;i++){
		//New frame to be inserted
		Frame* newFrame=(Frame*)malloc(sizeof(Frame));
		initialise(newFrame);

		insert_flag=0;

		//Check if element already exists in the frame list
		if(!checkLL(LL,page_references[ctr])){
			newFrame->page_no = page_references[ctr];
			insert(LL,newFrame);
			insert_flag=1;
			page_fault++;
		}
		printf("\n%15d -> ",page_references[ctr]);
		displayFrames(LL,frames_reqd);
		printf("-> %-12d\n",page_fault);
		
		//Update the page reference table
		if(insert_flag){
			page_ref_table[row_ctr][col_ctr]='0'+page_references[ctr];
			
			for(int i=col_ctr+1;i<100;i++)
				page_ref_table[row_ctr][i] = page_ref_table[row_ctr][col_ctr];

				row_ctr++;
			col_ctr++;
		}

		ctr++;
	}

	/*
	This loop makes all the succesive updation to the frame list depending on the 
	specified algorithm.
	*/
	for(;ctr<page_ref_count;ctr++){

		insert_flag=0;
		
		int page_no;
		if(!checkLL(LL,page_references[ctr])){
			page_no=OptimalPos(LL,ctr,page_references,page_ref_count);
			ReplacePos(LL,page_no, page_references[ctr]);
			insert_flag=1;
			page_fault++;
		}
		printf("\n%15d -> ",page_references[ctr]);
		displayFrames(LL,frames_reqd);
		printf("-> %-12d\n",page_fault);

		//Update page reference table
		if(insert_flag){
			for(int i=0;i<frames_reqd;i++){
				if(page_ref_table[i][col_ctr]=='0'+page_no){
					row_ctr=i;break;
					
				}
			}
			
			page_ref_table[row_ctr][col_ctr]='0'+page_references[ctr];
			
			for(int i=col_ctr+1;i<100;i++)
				page_ref_table[row_ctr][i] = page_ref_table[row_ctr][col_ctr];
			col_ctr++;
		}

	}
	displayTable(page_ref_table,frames_reqd,col_ctr);
	printf("\nTotal number of Page faults:%d \n",page_fault);

}


//Least Recently Used Position
/*
Logic:
1.Maintain an array where each element represents the index of the previous occurence of the 
corresponding element in the frame list.
2.Find the index of the minimum value in this array, which corresponds to the element that 
is not used in the near future.
3.Pass the page_no of the element in that index in the frame list.
*/

int LRUPos(Frame *LL,int ctr,int page_references[100],int page_ref_count){
	//LRU Position array
	int lru_array[10];
	for(int i=0;i<10;i++)
		lru_array[i]=100;
	int arr_ctr=0;
	Frame *tmp=LL;
	tmp=tmp->next;

	//Compute LRU positions
	while(tmp){
		for(int i=0;i<ctr;i++){
			if(page_references[i]==tmp->page_no){
				lru_array[arr_ctr]=i;
			}
		}
		tmp=tmp->next;
		arr_ctr++;
	}

	//Compute index of minimum value
	int minid=0;
	for(int i=0;i<arr_ctr;i++){
		if(lru_array[i]<lru_array[minid])
			minid=i;
	}

	tmp=LL;
	tmp=tmp->next;
	while(minid){
		tmp=tmp->next;minid--;
	}
	return tmp->page_no;
}

//Least Recently Used Algorithm
/*
page_fault: The page fault produced by the algorithm satisfying the given string. 
page_references: Array of page references from the reference string.
page_ref_count: Number of references in the string.
page_ref_table: Table to keep track of all the changes in the frames.
row_ctr,col_ctr: Indices to traverse the page reference table.
insert_flag: Flag to indicate whether an updation has occured in the frames. 

*/

void LRUAlgo(int frames_reqd,char *reference_string){

	int page_fault=0;

	Frame *LL=(Frame*)malloc(sizeof(Frame));
	initialise(LL);

	int page_references[100];
	int page_ref_count=refStringSplit(reference_string,page_references);

	char page_ref_table[10][100];
	for(int i=0;i<10;i++)
		for(int j=0;j<100;j++)
			page_ref_table[i][j]=' ';

	int row_ctr=0;
	int col_ctr=0;

	printf("\n LRU Page Replacement Algorithm \n");
	printf("\nThe reference string is: %-50s\n",reference_string);

	printf("\n%-15s -> %-20s %5s -> %-12s\n","Page reference","Memory"," ","Page fault");
	
	int ctr=0;
	int insert_flag=0;
	
	/*
	This loop fills the frame list for the first time, having number of nodes equal 
	to that of number of frames required by the process.
	*/
	for(int i=0;i<frames_reqd && ctr<page_ref_count;i++){
		//New Frame to be inserted
		Frame* newFrame=(Frame*)malloc(sizeof(Frame));
		initialise(newFrame);

		insert_flag=0;

		//Check if element is already present in frame list
		if(!checkLL(LL,page_references[ctr])){
			newFrame->page_no = page_references[ctr];
			insert(LL,newFrame);
			insert_flag=1;
			page_fault++;
		}
		printf("\n%15d -> ",page_references[ctr]);
		displayFrames(LL,frames_reqd);
		printf("-> %-12d\n",page_fault);
		
		//Update the page reference table
		if(insert_flag){
			page_ref_table[row_ctr][col_ctr]='0'+page_references[ctr];
			
			for(int i=col_ctr+1;i<100;i++)
				page_ref_table[row_ctr][i] = page_ref_table[row_ctr][col_ctr];

				row_ctr++;
			col_ctr++;
		}

		ctr++;
	}

	/*
	This loop makes all the succesive updation to the frame list depending on the 
	specified algorithm.
	*/
	for(;ctr<page_ref_count;ctr++){

		insert_flag=0;
		
		int page_no;
		if(!checkLL(LL,page_references[ctr])){
			page_no=LRUPos(LL,ctr,page_references,page_ref_count);
			ReplacePos(LL,page_no, page_references[ctr]);
			insert_flag=1;
			page_fault++;
		}
		printf("\n%15d -> ",page_references[ctr]);
		displayFrames(LL,frames_reqd);
		printf("-> %-12d\n",page_fault);

		//Update the page reference table
		if(insert_flag){
			for(int i=0;i<frames_reqd;i++){
				if(page_ref_table[i][col_ctr]=='0'+page_no){
					row_ctr=i;break;
					
				}
			}
			
			page_ref_table[row_ctr][col_ctr]='0'+page_references[ctr];
			
			for(int i=col_ctr+1;i<100;i++)
				page_ref_table[row_ctr][i] = page_ref_table[row_ctr][col_ctr];
			col_ctr++;
		}

	}
	displayTable(page_ref_table,frames_reqd,col_ctr);
	printf("\nTotal number of Page faults:%d \n",page_fault);
}

//Least Frequently Used Position
/*
Logic:
1.Maintain an array where each element represents the frequency of the occurences of the 
corresponding element in the frame list.
2.Find the index of the minimum value in this array, which corresponds to the element that 
is not used in the near future.
3.Pass the page_no of the element in that index in the frame list.
*/

int LFUPos(Frame *LL,int ctr,int page_references[100],int page_ref_count){
	//LFU array
	int lfu_array[10];
	for(int i=0;i<10;i++)
		lfu_array[i]=0;
	int arr_ctr=0;
	Frame *tmp=LL;
	tmp=tmp->next;

	//Compute frequencies
	while(tmp){
		for(int i=ctr;i<page_ref_count;i++){
			if(page_references[i]==tmp->page_no){
				lfu_array[arr_ctr]++;
			}
		}
		tmp=tmp->next;
		arr_ctr++;
	}

	//Compute index of minimum element
	int minid=0;
	for(int i=0;i<arr_ctr;i++){
		if(lfu_array[i]<lfu_array[minid])
			minid=i;
	}

	tmp=LL;
	tmp=tmp->next;
	while(minid){
		tmp=tmp->next;minid--;
	}
	return tmp->page_no;
}

//Least Frequently Used Algorithm
/*
page_fault: The page fault produced by the algorithm satisfying the given string. 
page_references: Array of page references from the reference string.
page_ref_count: Number of references in the string.
page_ref_table: Table to keep track of all the changes in the frames.
row_ctr,col_ctr: Indices to traverse the page reference table.
insert_flag: Flag to indicate whether an updation has occured in the frames. 

*/

void LFUAlgo(int frames_reqd,char *reference_string){

	int page_fault=0;

	Frame *LL=(Frame*)malloc(sizeof(Frame));
	initialise(LL);

	int page_references[100];
	int page_ref_count=refStringSplit(reference_string,page_references);

	char page_ref_table[10][100];
	for(int i=0;i<10;i++)
		for(int j=0;j<100;j++)
			page_ref_table[i][j]=' ';

	int row_ctr=0;
	int col_ctr=0;

	printf("\n LFU Page Replacement Algorithm \n");
	printf("\nThe reference string is: %-50s\n",reference_string);

	printf("\n%-15s -> %-20s %5s -> %-12s\n","Page reference","Memory"," ","Page fault");
	
	int ctr=0;
	int insert_flag=0;
	
	/*
	This loop fills the frame list for the first time, having number of nodes equal 
	to that of number of frames required by the process.
	*/
	for(int i=0;i<frames_reqd && ctr<page_ref_count;i++){
		//New Frame to be inserted
		Frame* newFrame=(Frame*)malloc(sizeof(Frame));
		initialise(newFrame);

		insert_flag=0;

		//Check if element is already in frame list
		if(!checkLL(LL,page_references[ctr])){
			newFrame->page_no = page_references[ctr];
			insert(LL,newFrame);
			insert_flag=1;
			page_fault++;
		}
		printf("\n%15d -> ",page_references[ctr]);
		displayFrames(LL,frames_reqd);
		printf("-> %-12d\n",page_fault);
		
		//Update the page reference table
		if(insert_flag){
			page_ref_table[row_ctr][col_ctr]='0'+page_references[ctr];
			
			for(int i=col_ctr+1;i<100;i++)
				page_ref_table[row_ctr][i] = page_ref_table[row_ctr][col_ctr];

				row_ctr++;
			col_ctr++;
		}

		ctr++;
	}

	/*
	This loop makes all the succesive updation to the frame list depending on the 
	specified algorithm.
	*/
	for(;ctr<page_ref_count;ctr++){

		insert_flag=0;
		
		int page_no;
		if(!checkLL(LL,page_references[ctr])){
			page_no=LFUPos(LL,ctr,page_references,page_ref_count);
			ReplacePos(LL,page_no, page_references[ctr]);
			insert_flag=1;
			page_fault++;
		}
		printf("\n%15d -> ",page_references[ctr]);
		displayFrames(LL,frames_reqd);
		printf("-> %-12d\n",page_fault);

		//Update the page reference table
		if(insert_flag){
			for(int i=0;i<frames_reqd;i++){
				if(page_ref_table[i][col_ctr]=='0'+page_no){
					row_ctr=i;break;
					
				}
			}
			
			page_ref_table[row_ctr][col_ctr]='0'+page_references[ctr];
			
			for(int i=col_ctr+1;i<100;i++)
				page_ref_table[row_ctr][i] = page_ref_table[row_ctr][col_ctr];
			col_ctr++;
		}

	}
	displayTable(page_ref_table,frames_reqd,col_ctr);
	printf("\nTotal number of Page faults:%d \n",page_fault);
}

void main(){
	//Number of free frames
	int noff;
	//Frames required by process
	int frames_reqd;
	//Page reference string
	char *reference_string=(char*)malloc(sizeof(100));

	int option;
	do{
		printf("\n Choose operation: \n 1.Read data \n 2.FIFO algorithm ");
		printf("\n 3.Optimal algorithm \n 4.LRU algorithm \n 5.LFU algorithm ");
		printf("\n 0.Exit \n Your choice: ");scanf("%d",&option);

		if(option==1){
			acceptData(&noff,&frames_reqd,reference_string);
		}
		else if(option==2){
			FIFOAlgo(frames_reqd,reference_string);
		}
		else if(option==3){
			OptimalAlgo(frames_reqd,reference_string);
		}
		else if(option==4){
			LRUAlgo(frames_reqd,reference_string);
		}
		else if(option==5){
			LFUAlgo(frames_reqd,reference_string);
		}
		else if(option){
			printf("\n Invalid option. \n");
		}
		else;
	}while(option);

}

/*
Output:

 Choose operation: 
 1.Read data 
 2.FIFO algorithm 
 3.Optimal algorithm 
 4.LRU algorithm 
 5.LFU algorithm 
 0.Exit 
 Your choice: 1

Enter the number of free frames: 10

Enter the number of frames required: 3

Enter the reference string: 7 0 1 2 0 3 0 4 2 3 0 3
                         
 Choose operation: 
 1.Read data 
 2.FIFO algorithm 
 3.Optimal algorithm 
 4.LRU algorithm 
 5.LFU algorithm 
 0.Exit 
 Your choice: 2

 FIFO Page Replacement Algorithm 

The reference string is: 7 0 1 2 0 3 0 4 2 3 0 3                           

Page reference  -> Memory                     -> Page fault  

              7 -> 7    -    -                -> 1           

              0 -> 7    0    -                -> 2           

              1 -> 7    0    1                -> 3           

              2 -> 0    1    2                -> 4           

              0 -> 0    1    2                -> 4           

              3 -> 1    2    3                -> 5           

              0 -> 2    3    0                -> 6           

              4 -> 3    0    4                -> 7           

              2 -> 0    4    2                -> 8           

              3 -> 4    2    3                -> 9           

              0 -> 2    3    0                -> 10          

              3 -> 2    3    0                -> 10          
____________________________________________________________
|_ 7_||_ 7_||_ 7_||_ 2_||_ 2_||_ 2_||_ 4_||_ 4_||_ 4_||_ 0_|
|_  _||_ 0_||_ 0_||_ 0_||_ 3_||_ 3_||_ 3_||_ 2_||_ 2_||_ 2_|
|_  _||_  _||_ 1_||_ 1_||_ 1_||_ 0_||_ 0_||_ 0_||_ 3_||_ 3_|

Total number of Page faults:10 

 Choose operation: 
 1.Read data 
 2.FIFO algorithm 
 3.Optimal algorithm 
 4.LRU algorithm 
 5.LFU algorithm 
 0.Exit 
 Your choice: 3

 Optimal Page Replacement Algorithm 

The reference string is: 7 0 1 2 0 3 0 4 2 3 0 3                           

Page reference  -> Memory                     -> Page fault  

              7 -> 7    -    -                -> 1           

              0 -> 7    0    -                -> 2           

              1 -> 7    0    1                -> 3           

              2 -> 2    0    1                -> 4           

              0 -> 2    0    1                -> 4           

              3 -> 2    0    3                -> 5           

              0 -> 2    0    3                -> 5           

              4 -> 2    4    3                -> 6           

              2 -> 2    4    3                -> 6           

              3 -> 2    4    3                -> 6           

              0 -> 0    4    3                -> 7           

              3 -> 0    4    3                -> 7           
__________________________________________
|_ 7_||_ 7_||_ 7_||_ 2_||_ 2_||_ 2_||_ 0_|
|_  _||_ 0_||_ 0_||_ 0_||_ 0_||_ 4_||_ 4_|
|_  _||_  _||_ 1_||_ 1_||_ 3_||_ 3_||_ 3_|

Total number of Page faults:7 

 Choose operation: 
 1.Read data 
 2.FIFO algorithm 
 3.Optimal algorithm 
 4.LRU algorithm 
 5.LFU algorithm 
 0.Exit 
 Your choice: 4

 LRU Page Replacement Algorithm 

The reference string is: 7 0 1 2 0 3 0 4 2 3 0 3                           

Page reference  -> Memory                     -> Page fault  

              7 -> 7    -    -                -> 1           

              0 -> 7    0    -                -> 2           

              1 -> 7    0    1                -> 3           

              2 -> 2    0    1                -> 4           

              0 -> 2    0    1                -> 4           

              3 -> 2    0    3                -> 5           

              0 -> 2    0    3                -> 5           

              4 -> 4    0    3                -> 6           

              2 -> 4    0    2                -> 7           

              3 -> 4    3    2                -> 8           

              0 -> 0    3    2                -> 9           

              3 -> 0    3    2                -> 9           
______________________________________________________
|_ 7_||_ 7_||_ 7_||_ 2_||_ 2_||_ 4_||_ 4_||_ 4_||_ 0_|
|_  _||_ 0_||_ 0_||_ 0_||_ 0_||_ 0_||_ 0_||_ 3_||_ 3_|
|_  _||_  _||_ 1_||_ 1_||_ 3_||_ 3_||_ 2_||_ 2_||_ 2_|

Total number of Page faults:9 

 Choose operation: 
 1.Read data 
 2.FIFO algorithm 
 3.Optimal algorithm 
 4.LRU algorithm 
 5.LFU algorithm 
 0.Exit 
 Your choice: 5

 LFU Page Replacement Algorithm 

The reference string is: 7 0 1 2 0 3 0 4 2 3 0 3                           

Page reference  -> Memory                     -> Page fault  

              7 -> 7    -    -                -> 1           

              0 -> 7    0    -                -> 2           

              1 -> 7    0    1                -> 3           

              2 -> 2    0    1                -> 4           

              0 -> 2    0    1                -> 4           

              3 -> 2    0    3                -> 5           

              0 -> 2    0    3                -> 5           

              4 -> 4    0    3                -> 6           

              2 -> 2    0    3                -> 7           

              3 -> 2    0    3                -> 7           

              0 -> 2    0    3                -> 7           

              3 -> 2    0    3                -> 7           
__________________________________________
|_ 7_||_ 7_||_ 7_||_ 2_||_ 2_||_ 4_||_ 2_|
|_  _||_ 0_||_ 0_||_ 0_||_ 0_||_ 0_||_ 0_|
|_  _||_  _||_ 1_||_ 1_||_ 3_||_ 3_||_ 3_|

Total number of Page faults:7 

 Choose operation: 
 1.Read data 
 2.FIFO algorithm 
 3.Optimal algorithm 
 4.LRU algorithm 
 5.LFU algorithm 
 0.Exit 
 Your choice: 0
*/