#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sched.c"
#include <string.h>

 float WTtimes[5];
 float TATtimes[5];
 int conTimes[5];
 char* file3;


int main(int argc, char *argv[]) {


char* inputFile = argv[1];  //Takes in the input file 
char* outputFile = argv[2]; //Output file to write to 
int timeInterval = atoi(argv[3]); //Stores time interval  

//for indexing 
int j = 0;   
int x = 0;  
int z = 0;

//Read in numbers and store into an array 
//first find number of elements reading in
FILE *file = fopen(inputFile, "r");
    //int integers[15];
    int i1=0;
    int num1;
    while(fscanf(file, "%d", &num1) >= 0) {
        
        i1++; //number of elements in array 
    }
    fclose(file);

//Use i1 to determine how many elements there are 
//Read in numbers and store into an array 
FILE *file2 = fopen(inputFile, "r");
    int integers[i1];
    int i=0;
    int num;
    while(fscanf(file, "%d", &num) >= 0) {
        integers[i] = num;
        i++;
    }
    fclose(file);

int n = i1/3; //Find number of elements in each burst,arrival,and priority array

FILE *file3 = fopen(outputFile, "w");  //Open file to write to 

struct Queue* readyQueue = createQueue(n); //Stores the wait list for process
struct Queue* processQueue = createQueue(n); //Holds information pertaining to each process
struct Queue* priorityQueue = createQueue(n);//Holds priority of each process
struct FinishQueue* FinishQueue;            //
processQueue->n = n;                        //Store number of elements in burst,arrival, and priority
processQueue->timeInterval = timeInterval;  //Pass the time interval to the process queue 
processQueue->currentTime = 0; //Set current time to 0 to start with

//Arrays to store information for algorithms
int arrival[n];
int priority[n];
int burst[n];


//Store file burst, arrival, and priority in different arrays for organization
for (int i = 0; i < i1; i=i+3)
    
    {
    burst[j] = integers[i];
    arrival[j] = integers[i+2];
    priority[j] = integers[i+1];
    
    fprintf(file3,"%d %d %d \n", burst[j],arrival[j],priority[j]); //print all the elements to be used 
    j=j+1;
    }

//Store information into process Queue to be used in algorithms
for (int i = 0; i<processQueue->n; i=i+1)
    
    {
    processQueue->burst[i] = burst[i];
    processQueue->arrival[i] = arrival[i];
    processQueue->priority[i] = priority[i];  
    }

//----------------------F C F S-----------------------------------
FCFS(readyQueue,processQueue,file3);

//----------------------S J F-------------------------------------
SJF(readyQueue,processQueue,file3);

//----------------------P R I O R I T Y---------------------------
PRIORITY(readyQueue,processQueue,priorityQueue,file3);

//----------------------S T F C---------------------------
STFC(readyQueue,processQueue,file3);

//----------------------R R---------------------------
RR(readyQueue,processQueue,file3);

fprintf(file3,"***** OVERALL SUMMARY *****\n");

//Create arrays to copy data into
float WTalg[5];
float TATalg[5];
int conalg[5];

//insert data into new arrays
for (int i = 0; i < 5; i++){
    WTalg[i] = WTtimes[i];
    TATalg[i] = TATtimes[i];
    conalg[i] = conTimes[i];
    }

int inner,outer,temp2 =0;
float temp=0;

//Sort wait times
for(outer=0;outer<5;outer=outer+1)
 {
 for(inner=outer+1;inner<5;inner++)
 {
 if(WTtimes[outer] > WTtimes[inner])
 {
 
 temp = WTtimes[outer];

 
 WTtimes[outer] = WTtimes[inner];
 
 
 WTtimes[inner] = temp;

}
}
}
fprintf(file3,"\n \n");
fprintf(file3,"Wait Time Comparison\n");
for(int i = 0; i < 5; i++){

    if(WTtimes[i] == WTalg[0])
    {
     fprintf(file3,"%d FCFS \t %f \n",i+1,WTtimes[i]);
    }
    if(WTtimes[i] == WTalg[1])
    {
     fprintf(file3,"%d RR \t %f \n",i+1,WTtimes[i]);
    }
    if(WTtimes[i] == WTalg[2])
    {
     fprintf(file3,"%d SJF \t %f \n",i+1,WTtimes[i]);
    }
    if(WTtimes[i] == WTalg[3])
    {
     fprintf(file3,"%d STFC \t %f \n",i+1,WTtimes[i]);
    }
     if(WTtimes[i] == WTalg[4])
    {
     fprintf(file3,"%d PRI \t %f \n",i+1,WTtimes[i]);
    }   
}
temp,inner,outer=0;
//Sort turnaround times
for(outer=0;outer<5;outer=outer+1)
 {
 for(inner=outer+1;inner<5;inner++)
 {
 if(TATtimes[outer] > TATtimes[inner])
 {
 
 temp = TATtimes[outer];

 
 TATtimes[outer] = TATtimes[inner];
 
 
 TATtimes[inner] = temp;

}
}
}
fprintf(file3,"\n \n");
fprintf(file3,"Turnaround Time Comparison\n");
for(int i = 0; i < 5; i++){

    if(TATtimes[i] == TATalg[0])
    {
     fprintf(file3,"%d FCFS \t %f \n",i+1,TATtimes[i]);
    }
    if(TATtimes[i] == TATalg[1])
    {
     fprintf(file3,"%d RR \t %f \n",i+1,TATtimes[i]);
    }
    if(TATtimes[i] == TATalg[2])
    {
     fprintf(file3,"%d SJF \t %f \n",i+1,TATtimes[i]);
    }
    if(TATtimes[i] == TATalg[3])
    {
     fprintf(file3,"%d STFC \t %f \n",i+1,TATtimes[i]);
    }
     if(TATtimes[i] == TATalg[4])
    {
     fprintf(file3,"%d PRI \t %f \n",i+1,TATtimes[i]);
    }   
}
temp,inner,outer=0;
int temp1=0;
//Sort Context Switches
for(outer=0;outer<5;outer=outer+1)
 {
 for(inner=outer+1;inner<5;inner++)
 {
 if(conTimes[outer] > conTimes[inner])
 {
 
 temp1 = conTimes[outer];

 
 conTimes[outer] = conTimes[inner];
 
 
 conTimes[inner] = temp1;

}
}
}
fprintf(file3,"\n \n");
fprintf(file3,"Context Switch Comparison\n");
int u,e,p,q,v = 0;
for(int i = 0; i < 5; i++)

{
  
    if(conTimes[i] == conalg[0]  && u<1 )
    {
     fprintf(file3,"%d FCFS \t %d \n",i+1,conTimes[i]);
     u++;
    }
   else if(conTimes[i] == conalg[1]  && e<1 )
    {
     fprintf(file3,"%d RR \t %d \n",i+1,conTimes[i]  );
     e++;    
    }
   else if(conTimes[i] == conalg[3] && q<1)
    {
     fprintf(file3,"%d STFC \t %d \n",i+1,conTimes[i] );
     q++;
    }
    else if(conTimes[i] == conalg[4] && v<1)
    {
     fprintf(file3,"%d PRI \t %d \n",i+1,conTimes[i]);
     v++;
    }   
   else if (conTimes[i] == conalg[2] && p<1)
    {
     fprintf(file3,"%d SJF \t %d \n",i+1,conTimes[i]);
     p++;
    }

}

fclose(file3); //Close the file



}




