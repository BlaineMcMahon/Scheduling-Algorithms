#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sched.h"
#include <limits.h>

 float WTtimes[5];
 float TATtimes[5];
 int conTimes[5];
 
/*--------------- C R E A T E  Q U E U E----------------------------*/
struct Queue* createQueue(int capacity) 
{ 
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue)); 
    queue->capacity = capacity; 
    queue->front = queue->size = 0;  
    queue->rear = capacity - 1;  // This is important, see the enqueue 
    queue->array = (int*) malloc(queue->capacity * sizeof(int)); 
    queue->burst = (int*) malloc(queue->capacity * sizeof(int));
    queue->priority = (int*) malloc(queue->capacity * sizeof(int));
    queue->arrival = (int*) malloc(queue->capacity * sizeof(int));
    queue->pids = (int*) malloc(queue->capacity * sizeof(int));
    return queue; 
} 

/*---------------I S  F U L L----------------------------*/
int isFull(struct Queue* queue) 
{  
    return (queue->size == queue->capacity); 
} 

/*---------------I S  E M P T Y----------------------------*/
int isEmpty(struct Queue* queue) 
{ 
    return (queue->size == 0); 
} 

/*---------------E N Q U E U E----------------------------*/
void enqueue(struct Queue* queue, int item) 
{ 
    if (isFull(queue)) 
        return; 
    queue->rear = (queue->rear + 1)%queue->capacity; 
    queue->array[queue->rear] = item; 
    queue->size = queue->size + 1; 
}

/*---------------D E N Q U E U E----------------------------*/
int dequeue(struct Queue* queue) 
{ 
    
    int item = queue->array[queue->front]; 
    queue->front = (queue->front + 1)%queue->capacity; 
    queue->size = queue->size - 1; 
    return item; 
} 

/*---------------F R O N T----------------------------*/
int front(struct Queue* queue) 
{ 
   // if (isEmpty(queue)) 
     //   ffprintf(file3,file3,"ReadyQueue is Empty \n"); 
    return queue->array[queue->front]; 
} 

/*---------------R E A R----------------------------*/
int rear(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return INT_MIN; 
    return queue->array[queue->rear]; 
} 

/*---------------P R I N T----------------------------*/
void print(struct Queue* queue,FILE* file3)
{
  if(isEmpty(queue)){fprintf(file3,"Ready Queue empty\n");}
else{
    fprintf(file3,"Ready Queue:");
    for(int i = 0; i < queue->size; i++)
    {
    fprintf(file3," %d ",queue->array[queue->front+i]);  
    }
fprintf(file3,"\n");}
}

void print2(struct Queue* queue,FILE* file3)
{
      if(isEmpty(queue)){fprintf(file3,"Ready Queue empty\n");}
else{
    fprintf(file3,"Ready Queue:");
    for(int i = 0; i < queue->size-1; i++)
    {
    fprintf(file3," %d ",queue->array[queue->front+i+1]);
    }
fprintf(file3,"\n"); }
}

void print3(struct Queue* queue,FILE* file3)
{
    fprintf(file3,"Ready Queue:");
    for(int i = 0; i < queue->size-1; i++)
    {
    fprintf(file3," %d ",queue->array[queue->front+i+1]);
    }
fprintf(file3,"\n");
}


/*---------------S T A R T  T I M E----------------------------*/
void startTime(struct Queue* processQueue)
{    
   //Consider first element as smallest
   int smallest = processQueue->arrival[0];

   for (int i = 0; i < processQueue->capacity; i++) {
      if (processQueue->arrival[i] < smallest) {
         smallest = processQueue->arrival[i];
      }
   }  
    processQueue->startTime=smallest;

}   

/*---------------C H E C K  T I M E----------------------------*/      
void checkTime(struct Queue* processQueue,struct Queue* readyQueue,int time)
{
  //function is used to check if any new processes have arrived 
  //and enqueue the burst and the process ID
  int time2 = time;
  
  for(int j=0; j<processQueue->n; j=j+1)
    {
    
        
        //Check if any processes match given time  
        if(processQueue->arrival[j] == time2)
        {
             readyQueue->arrival[j] = processQueue->arrival[j];
             enqueue(processQueue,processQueue->burst[j]); //Enqueue the arrival specific arrival time         
             enqueue(readyQueue,processQueue->pid);         //increment the process                  
             processQueue->pid = processQueue->pid +1;
        }
    
       
    } 
    
}

/*---------------C H E C K  T I M E----------------------------*/      
void checkTime2(struct Queue* processQueue,struct Queue* readyQueue,struct Queue * priorityQueue,int time)
{
  //function is used to check if any new processes have arrived 
  //and enqueue the burst and the process ID
  //and store the priority
  int time2 = time;
  
  for(int j=0; j<processQueue->n; j=j+1)
    {
    
        
        //Check if any processes match given time  
        if(processQueue->arrival[j] == time2)
        {
             readyQueue->arrival[j] = processQueue->arrival[j];
             enqueue(priorityQueue,processQueue->priority[j]);
             enqueue(processQueue,processQueue->burst[j]); //Enqueue the arrival specific arrival time         
             enqueue(readyQueue,processQueue->pid);         //increment the process                  
             processQueue->pid = processQueue->pid +1;
             bubbleSort2(processQueue,readyQueue,priorityQueue);      
          }
    
       
    } 
    
}

void wt(struct Queue* readyQueue,struct Queue* processQueue,FILE* file3)
{
//Function is used to find the wait time and turn around time and context switches
//for each algorithm
    int n = processQueue->n;
    int contextSwitch[5];
    int startTime = 0;
    int wt[n];
    int service_time[n];
    int tat[n];   
    float waitTime;
    float turnaroundTime;    

    float avg_wt;
    float avg_tat;    
    
    service_time[0] = 0;
    wt[0] = 0;
        
    // calculating waiting time 
    for (int i = 1; i < n ; i++) 
    { 
        // Add burst time of previous processes 
        service_time[i] = service_time[i-1] + readyQueue->burst[i-1]; 
  
        // Find waiting time for current process = 
        // sum - at[i] 
        wt[i] = service_time[i] - readyQueue->arrival[i]; 
     
        // If waiting time for a process is in negative 
        // that means it is already in the ready queue 
        // before CPU becomes idle so its waiting time is 0 
        if (wt[i] < 0) 
            wt[i] = 0; 
    
        waitTime = wt[i] + waitTime;
    } 
   
     // Calculating turnaround time by adding bt[i] + wt[i] 
    for (int i = 0; i < n ; i++) {
        tat[i] = readyQueue->burst[i] + wt[i]; 
        turnaroundTime = turnaroundTime + tat[i];
} 
   
    avg_wt = waitTime/processQueue->n;
    avg_tat = turnaroundTime/processQueue->n;
   
   WTtimes[0] = avg_wt;
   TATtimes[0] = avg_tat;
 
    for (int i = 0; i <processQueue->n; i = i + 1)
        {
        
        fprintf(file3,"%d \t %d \t %d \n",readyQueue->pids[i],wt[i],tat[i]);   
        }
   fprintf(file3,"AVG: %f %f \n",avg_wt,avg_tat);
   fprintf(file3,"Process Sequence: ");   

   for (int i = 0; i < n; i = i +1)
    {   
    fprintf(file3,"%d",readyQueue->pids[i]);  
    }
    fprintf(file3,"\n");
    fprintf(file3,"Context Switch: %d \n",processQueue->contextSwitch[0]);
    fprintf(file3,"\n");
}
 
void wt2(struct Queue* readyQueue,struct Queue* processQueue,int k,FILE* file3)
{
    int n = processQueue->n;
    int startTime = 0;
    int wt[n];
    int service_time[n];
    int tat[n];   
    float waitTime;
    float turnaroundTime;    

    float avg_wt;
    float avg_tat;    
    
    service_time[0] = 0;
    wt[0] = 0;
        
    // calculating waiting time 
    for (int i = 1; i < n; i++) 
    { 
        // Add burst time of previous processes 
        service_time[i] = service_time[i-1] + readyQueue->burst[i-1]; 
  
        // Find waiting time for current process = 
        // sum - at[i] 
        wt[i] = service_time[i] - readyQueue->arrival[i+1]; 
     
        // If waiting time for a process is in negative 
        // that means it is already in the ready queue 
        // before CPU becomes idle so its waiting time is 0 
        if (wt[i] < 0) 
            wt[i] = 0; 
    
        waitTime = wt[i] + waitTime;
    } 
   
     // Calculating turnaround time by adding bt[i] + wt[i] 
    for (int i = 0; i < n ; i++) {
        tat[i] = readyQueue->burst[i] + wt[i]; 
        turnaroundTime = turnaroundTime + tat[i];
} 

    avg_wt = waitTime/n;
    avg_tat = turnaroundTime/n;
    
   WTtimes[k] = avg_wt;
   TATtimes[k] = avg_tat;
         

    for (int i = 0; i < n; i = i + 1)
        {
        
        fprintf(file3,"%d \t %d \t %d \n",readyQueue->pids[i],wt[i],tat[i]);   
        }
   fprintf(file3,"AVG: %f %f \n",avg_wt,avg_tat);
   fprintf(file3,"Process Sequence: ");   

   for (int i = 0; i < n; i = i +1)
    {   
    fprintf(file3,"%d",readyQueue->pids[i]);  
    }
    fprintf(file3,"\n");
    fprintf(file3,"Context Switch: %d \n",processQueue->contextSwitch[k]);
    fprintf(file3,"\n");
    conTimes[k] = processQueue->contextSwitch[k];
}
 
void wt3(struct Queue* readyQueue,struct Queue* processQueue,int k,FILE* file3)
{
    int n = processQueue->n;
    int startTime = 0;
    int wt[n];
    int service_time[n];
    int tat[n];   
    float waitTime;
    float turnaroundTime;    

    float avg_wt;
    float avg_tat;    
    
    service_time[0] = 0;
    wt[0] = 0;
        
    // calculating waiting time 
    for (int i = 1; i < n; i++) 
    { 
        // Add burst time of previous processes 
        service_time[i] = service_time[i-1] + readyQueue->burst[i-1]; 
  
        // Find waiting time for current process = 
        // sum - at[i] 
        wt[i] = service_time[i] - readyQueue->arrival[i+1]; 
     
        // If waiting time for a process is in negative 
        // that means it is already in the ready queue 
        // before CPU becomes idle so its waiting time is 0 
        if (wt[i] < 0) 
            wt[i] = 0; 
    
        waitTime = wt[i] + waitTime;
    } 
   
     // Calculating turnaround time by adding bt[i] + wt[i] 
    for (int i = 0; i < n ; i++) {
        tat[i] = readyQueue->burst[i] + wt[i]; 
        turnaroundTime = turnaroundTime + tat[i];
} 

    avg_wt = waitTime/n;
    avg_tat = turnaroundTime/n;
    
   WTtimes[k] = avg_wt;
   TATtimes[k] = avg_tat;
         

    for (int i = 0; i < n; i = i + 1)
        {
        
        fprintf(file3,"%d \t %d \t %d \n",readyQueue->pids[i],wt[i],tat[i]);   
        }
   fprintf(file3,"AVG: %f %f \n",avg_wt,avg_tat);
   fprintf(file3,"Process Sequence: ");   

   for (int i = 0; i < n; i = i +1)
    {   
    //fprintf(file3,"%d",readyQueue->pids[i]);  
    }
    //fprintf(file3,"\n");
    //fprintf(file3,"Context Switch: %d \n",processQueue->contextSwitch[k]);
    //fprintf(file3,"\n");
    //conTimes[k] = processQueue->contextSwitch[k];
}

/*---------------B U B B L E  S O R T----------------------------*/      
void bubbleSort(struct Queue* readyQueue, struct Queue* processQueue)
{

//Bubble sort used to sort elements in each queue from smallest to largest based on burst time
int inner,temp,outer,temp2 =0;


for(outer=0;outer<processQueue->size-1;outer=outer+1)
 {
 for(inner=outer+1;inner<processQueue->size;inner++)
 {
 if(processQueue->array[processQueue->front+outer] > processQueue->array[processQueue->front+inner])
 {
 
 temp = processQueue->array[processQueue->front + outer];
 temp2 = readyQueue->array[readyQueue->front + outer];
 
 processQueue->array[processQueue->front+outer] = processQueue->array[processQueue->front+inner];
 readyQueue->array[readyQueue->front+outer] = readyQueue->array[readyQueue->front+inner]; 
 
 processQueue->array[processQueue->front+inner] = temp;
 readyQueue->array[readyQueue->front+inner] = temp2;
 
     
}
 }
 } 
}
/*---------------B U B B L E  S O R T----------------------------*/      
void bubbleSort2(struct Queue* readyQueue, struct Queue* processQueue,struct Queue* priorityQueue)
{
//This bubble sort will sort the queues based on the priority
int inner,temp,outer,temp2,temp3=0;


for(outer=0;outer<processQueue->size-1;outer=outer+1)
 {
 for(inner=outer+1;inner<processQueue->size;inner++)
 {
 if(priorityQueue->array[priorityQueue->front+outer] > priorityQueue->array[priorityQueue->front+inner])
 {
 temp3 = priorityQueue->array[priorityQueue->front + outer];
 temp = processQueue->array[processQueue->front + outer];
 temp2 = readyQueue->array[readyQueue->front + outer];
 priorityQueue->array[priorityQueue->front+outer] = priorityQueue->array[priorityQueue->front+inner];
 processQueue->array[processQueue->front+outer] = processQueue->array[processQueue->front+inner];
 readyQueue->array[readyQueue->front+outer] = readyQueue->array[readyQueue->front+inner]; 
 processQueue->array[processQueue->front+inner] = temp;
 readyQueue->array[readyQueue->front+inner] = temp2;
 priorityQueue->array[priorityQueue->front+inner] = temp3;
     
}
 }
 } 
}

/*--------------------- L E F T  R O T A T E------------------------------*/
void LeftRotatebyOne(struct Queue* processQueue, struct Queue* readyQueue)
{
//This function will rotate all elements in the queues to the left by one

int temp = front(processQueue);
int temp2 = front(readyQueue);

    for(int i = 0; i < processQueue->size-1; i++)
        {
        processQueue->array[processQueue->front+i] = processQueue->array[processQueue->front+i+1];
        readyQueue->array[readyQueue->front+i] = readyQueue->array[readyQueue->front+i+1];      
        }
        processQueue->array[readyQueue->rear]=temp; 
        readyQueue->array[readyQueue->rear]=temp2; 

}



/*-------------------- A L G O R I T H M  F U N C T I O N S---------------*/
void FCFS(struct Queue* readyQueue, struct Queue* processQueue, FILE *file3)
{      

   
    processQueue->currentTime = 0;
    processQueue->pid = 0;    
    startTime(processQueue);
    
    fprintf(file3,"Time interval for this process is: %d \n start time is: %d \n",processQueue->timeInterval,processQueue->startTime);
    
    int time = processQueue->currentTime; //Store current time
    int i=0; //indexing variables
    int j=0;
    int x=1;
    int z=0;
    int previousTime=processQueue->startTime; //Find start time
    //Arrival [0,0,2,4,6]
    //Burst [10,3,7,1,5]
    //Priority [1,4,2,2,3]

    
    checkTime(processQueue,readyQueue,time); //Check if any values have arrived
  
   
         
        //Perform burst for processes in the queue                      
        for (int i = front(processQueue);  i >= 0;   i=i-1)
        {       

              


            //If no more burst print results
              if(isEmpty(processQueue))
                {
                conTimes[0] = processQueue->contextSwitch[0];
           fprintf(file3,"**********************************************\nFCFS Summary (WT = wait time,  TT = turnaround time): \nPID \t WT \t TT\n");  
                  wt(readyQueue,processQueue,file3);
                  return;            
                }

             
            //If burst is about finish dequeue and start next burst
             if(i == 0)
            {
               {
                z = front(readyQueue);
                readyQueue->pids[x] = front(readyQueue);
                readyQueue->burst[x-1] = front(processQueue);
                readyQueue->finishTime[x] = time;   

                dequeue(readyQueue);               
                dequeue(processQueue);

                processQueue->contextSwitch[0] += 1;
                i = front(processQueue);    
                x = x + 1;               
                }            
            }
         
      
             

            //Case to handle for start time of first element 
            if(time == processQueue->startTime)
            {           
                fprintf(file3,"t = %d \n CPU: Loading Process %d (reamaining burst: %d) \n",time,front(readyQueue),i);        
                z = front(readyQueue);                
                processQueue->pids[0] = z;            
                dequeue(readyQueue); //Once element is loaded remove from ready Queue                                                 
                print(readyQueue,file3);                
                previousTime=time;                
                time = time + 1;
                i = i - 1;                           
            }
           
            //Check time to ensure ready queue is in order 
            checkTime(processQueue,readyQueue,time);
           

            //Check if time interval is equal to the current time 
            if((time) == (previousTime+processQueue->timeInterval))
            {
                //check if process has finished 
                if(i == front(processQueue))

                    {
                 fprintf(file3,"t = %d \n CPU: Finished Process %d; Loading process %d (reamaining burst: %d) \n",time,z-1,z,i);                      
                        print(readyQueue,file3);                        
                        previousTime = time;
                        time = time + 1;

                    }  
                //Process is still running
                else 
                    { 
                        
                        fprintf(file3,"t = %d \n CPU: Running Process %d (reamaining burst: %d) \n",time,z,i);                      
                        print(readyQueue,file3);                        
                        previousTime = time;
                        time = time + 1;
                    }
                          

            }
          //increment time 
            else 
            {

                time = time + 1;
            }          
            
                     
            }
    
  
}

//-------------------------------- S J F ------------------------------------
void SJF(struct Queue* readyQueue, struct Queue* processQueue,FILE* file3)
{      
    processQueue->startTime = 0;    
    processQueue->currentTime = 0;
    processQueue->pid = 0;    
    startTime(processQueue);
    int k = 2;

    fprintf(file3,"Time interval for this process is: %d \n start time is: %d \n",processQueue->timeInterval,processQueue->startTime);
    
    int burst[processQueue->n];
    int pids[processQueue->n];
    int time = 0;
    int i=0;
    int j=0;
    int x=0;
    int z=0;
    int previousTime=processQueue->startTime;
    //Arrival [0,0,2,4,6]
    //Burst [10,3,7,1,5]
    //Priority [1,4,2,2,3]
    readyQueue->front = 0;
    checkTime(processQueue,readyQueue,time);        //check if any processes have arrived
    bubbleSort(readyQueue,processQueue);            //Sort the queue
    fprintf(file3,"ready Queue %d \n",readyQueue->array[readyQueue->front]);
    
        //Perform burst for processes in the queue                      
        for (int i = front(processQueue);  i >= 0;   i=i-1)
        {   
                  
        //Check if all processes complete 
             if(isEmpty(processQueue))
                {
  fprintf(file3,"**********************************************\nSJF Summary (WT = wait time,  TT = turnaround time): \nPID \t WT \t TT\n");           
                        
                  wt2(readyQueue,processQueue,k,file3);
                  return;            
                }

               


            //If burst is at 0
             if(i == 0)
            {
               {           
                z = front(readyQueue);
                readyQueue->pids[x] = front(readyQueue);
                readyQueue->burst[x] = front(processQueue);
                readyQueue->finishTime[x] = time;   


                dequeue(readyQueue);               
                dequeue(processQueue);
                bubbleSort(readyQueue,processQueue); 
                          
                processQueue->contextSwitch[k] += 1;        
                i = front(processQueue);                  
                x = x + 1;               
                }            
            }
         
            
             

            //Case to handle for start time of first element 
            if(time == processQueue->startTime)
            {           
                fprintf(file3,"t = %d \n CPU: Loading Process %d (reamaining burst: %d) \n",time,front(readyQueue),i);        
                z = front(readyQueue);                                                                      
                print2(readyQueue,file3);                
                previousTime=time;                
                time = time + 1;
                i = i - 1;                           
            }
            checkTime(processQueue,readyQueue,time);
        
           

            //Check if time interval is equal to the current time 
          if((time) == (previousTime+processQueue->timeInterval))
            {
                //check if process has finished 
                if(i == front(processQueue))

                    {
                        z = front(readyQueue);
    fprintf(file3,"t = %d \n CPU: Finished Process %d; Loading process %d (reamaining burst: %d)\n",time,front(readyQueue),readyQueue->array[readyQueue->front+1],i);                      
                        print2(readyQueue,file3);                        
                        previousTime = time;
                        time = time + 1;

                    }  

                else 
                    { 
                        z = front(readyQueue);
                        fprintf(file3,"t = %d \n CPU: Running Process %d (reamaining burst: %d) \n",time,z,i);                      
                        print2(readyQueue,file3);                        
                        previousTime = time;
                        time = time + 1;
                    }
                          

            }
          
            else 
            {

                time = time + 1;
            }          
            
                     
            }
    
  
} 

//-------------------------------- P R I O R I T Y  ------------------------------------
void PRIORITY(struct Queue* readyQueue, struct Queue* processQueue,struct Queue* priorityQueue,FILE *file3)
{      
    readyQueue->array[readyQueue->front] = 0;   
    processQueue->startTime = 0;    
    processQueue->currentTime = 0;
    processQueue->pid = 0;    
    startTime(processQueue);
    dequeue(readyQueue);
    readyQueue->pids[0]=0;
    fprintf(file3,"Time interval for this process is: %d \n start time is: %d \n",processQueue->timeInterval,processQueue->startTime);
    
    int k = 4;
    int burst[processQueue->n];
    int pids[processQueue->n];
    int time = 0;
    int i=0;
    int j=0;
    int x=0;
    int z=0;
    int previousTime=processQueue->startTime;
    //Arrival [0,0,2,4,6]
    //Burst [10,3,7,1,5]
    //Priority [1,4,2,2,3]
    readyQueue->front = 0;
//Check if process has arrived
    checkTime2(processQueue,readyQueue,priorityQueue,time);
    bubbleSort2(readyQueue,processQueue,priorityQueue); //order queue based on priority 

    

        //Perform burst for processes in the queue                      
        for (int i = front(processQueue);  i >= 0;   i=i-1)
        {   
        

        

            //When queue is empty 
               if(isEmpty(processQueue))
                {
    fprintf(file3,"**********************************************\nPRIORITY Summary (WT = wait time,  TT = turnaround time): \nPID \t WT \t TT\n");  
                    
                  wt2(readyQueue,processQueue,k,file3);
                  return;            
                }

               


            //When process is finishing 
             if(i == 0)
            {
               {           

                
                z = front(readyQueue);
                
                readyQueue->pids[x] = front(readyQueue);
                readyQueue->burst[x] = front(processQueue);
                


                dequeue(priorityQueue);                
                dequeue(readyQueue);               
                dequeue(processQueue);
                          
                processQueue->contextSwitch[k] += 1;
                bubbleSort2(readyQueue,processQueue,priorityQueue); //sort the queue again
                i = front(processQueue);                  
                x = x + 1;     

                       
                }            
            }
         
        
             

            //Case to handle for start time of first element 
            if(time == processQueue->startTime)
            {           
                fprintf(file3,"t = %d \n CPU: Loading Process %d (reamaining burst: %d) \n",time,front(readyQueue),i);        
                z = front(readyQueue);                                                                      
                print2(readyQueue,file3);                
                previousTime=time;                
                time = time + 1;
                i = i - 1;                           
            }

            
            checkTime2(processQueue,readyQueue,priorityQueue,time);
            
           

            //Check if time interval is equal to the current time 
          if((time) == (previousTime+processQueue->timeInterval))
            {
               // bubbleSort2(processQueue,readyQueue,priorityQueue);
                //check if process has finished 
                if(i == front(processQueue))

                    {
                        z = front(readyQueue);
    fprintf(file3,"t = %d \n CPU: Finished Process %d; Loading process %d (reamaining burst: %d)\n",time,front(readyQueue),readyQueue->array[readyQueue->front+1],i);                      
                        print2(readyQueue,file3);                        
                        previousTime = time;
                        time = time + 1;
                        
                    }  

                      
                else 
                    { 
                        z=front(readyQueue);
                        fprintf(file3,"t = %d \n CPU: Running Process %d (reamaining burst: %d) \n",time,z,i);                      
                        print2(readyQueue,file3);                 
                        previousTime = time;
                        time = time + 1;
                    }
                          

            }
          
            else 
            {

                time = time + 1;
                processQueue->totalTime = time; //Store total time for STFC and RR
            }          
            
                     
            }
    
  
} 
    
//-------------------------------- R R ------------------------------------
void RR(struct Queue* readyQueue, struct Queue* processQueue,FILE *file3)
{
  
    readyQueue->array[readyQueue->front] = 0;   
    processQueue->startTime = 0;    
    processQueue->currentTime = 0;
    processQueue->pid = 0;    
    startTime(processQueue);
    int timeQuantum = 0;
    readyQueue->pids[0]=0;
    fprintf(file3,"Time interval for this process is: %d \n start time is: %d \n",processQueue->timeInterval,processQueue->startTime);
    int f = 0;
    int k = 0;
    int burst[5];
    int pids[5];
    int time = 0;
    int i=0;
    int quantum = 0;
    int t = 5;
    int j=0;
    int x=0;
    int n = processQueue->n;
    int z=0;
    int m=0;
    int proSeq[40];
    int temp;
    int previousTime=processQueue->startTime;
    int temp2=0;
    int old = 0;    
    int comTime[n];
    int h = 0;
    //Arrival [0,0,2,4,6]
    //Burst [10,3,7,1,5]
    //Priority [1,4,2,2,3]
    readyQueue->front = 0;
    k++;
    checkTime(processQueue,readyQueue,i);
    readyQueue->priority[f] = front(readyQueue);
    f++;
    fprintf(file3,"t = 0\nCPU: Loading process %d (CPU burst = %d)\n",front(readyQueue),front(processQueue));
    i = 0;
    quantum = 1;
    while(i < processQueue->totalTime) {  //Run till total time complete 
     i++;
     processQueue->array[processQueue->front]--; //decrement the burst
     checkTime(processQueue,readyQueue,i);      //Check if any process arrived
    
       
    
    //enter snapshot
    if(i % t == 0)
{    
//If process is getting preempted
    if(i % 2 == 0 && readyQueue->size > 1 && front(processQueue)!=0){
        fprintf(file3,"t = %d\n CPU: Preempting process %d; Loading Process %d (remaining CPU burst = %d)\n",i,front(readyQueue),readyQueue->array[readyQueue->front+1],processQueue->array[processQueue->front+1]);
     print2(readyQueue,file3);
        }
//if process has completed
    else if (front(processQueue)==0){
        fprintf(file3,"t = %d\nCPU Finishing Process %d; Loading Process %d (CPU burst = %d)\n",i,front(readyQueue),readyQueue->array[readyQueue->front+1],processQueue->array[processQueue->front+1]);  
         print2(readyQueue,file3);  
        }
//If processes are all finished
    else if (front(processQueue)==0 && readyQueue->size <= 1){
        fprintf(file3,"t = %d\nCPU Finishing Process %d",front(readyQueue),front(processQueue));
         print2(readyQueue,file3);
            }
//else the process is running 
   else{
      fprintf(file3,"t = %d\nCPU Running Process %d (CPU burst = %d)\n",i,front(readyQueue),front(processQueue));
     print2(readyQueue,file3);
    }
 }



    if(front(processQueue)==0){
 comTime[h] = i;
        h++;
 readyQueue->pids[x] = front(readyQueue);
 readyQueue->burst[x] = processQueue->burst[front(readyQueue)];  
 readyQueue->arrival[x] = processQueue->arrival[front(readyQueue)];
    x++;
}



    //if quantum has reached limit
     if(quantum == 2)
        {
        proSeq[m]=front(readyQueue);
        m++;
        LeftRotatebyOne(processQueue,readyQueue); 
        quantum = 0;    
        }
    //if burst is 0
     if(front(processQueue)==0)
        {
        if(quantum != 0){
         proSeq[m]=front(readyQueue);
         m++;}                      
        k++;
        z++;
        dequeue(readyQueue);               
        dequeue(processQueue);                                  
        }
    quantum++;
}


processQueue->contextSwitch[5] = m;

//find wait times and turnaround times
int WT[n];
int TAT[n];
float avg_wt=0;
float avg_tat=0;
for(int i = 0; i< n; i++)
{
WT[i] = comTime[i] - (readyQueue->burst[i] + readyQueue->arrival[i]);
TAT[i] = comTime[i] - readyQueue->arrival[i];
avg_wt = avg_wt + WT[i];
avg_tat = avg_wt + TAT[i];
}

avg_wt = avg_wt / n;
avg_tat = avg_tat / n;
WTtimes[1] = avg_wt;
TATtimes[1] = avg_tat;
   fprintf(file3,"**********************************************\nRR Summary (WT = wait time,  TT = turnaround time): \nPID \t WT \t TT\n");
  for (int i = 0; i < n; i = i + 1)
        {
        
        fprintf(file3,"%d \t %d \t %d \n",readyQueue->pids[i],WT[i],TAT[i]);   
        }
   fprintf(file3,"AVG: %f %f \n",avg_wt,avg_tat);
   fprintf(file3,"Process Sequence: ");   
    for(int i = 0; i<m; i++)
{
fprintf(file3,"%d",proSeq[i]);
}
fprintf(file3," Context Switches: %d ",processQueue->contextSwitch[5]);
fprintf(file3,"\n");
conTimes[1] = m;
}





//-------------------------------- S T F C ------------------------------------
void STFC(struct Queue* readyQueue, struct Queue* processQueue, FILE *file3)
{
    readyQueue->array[readyQueue->front] = 0;   
    processQueue->startTime = 0;    
    processQueue->currentTime = 0;
    processQueue->pid = 0;    
    startTime(processQueue);
    int temp = 0;
    readyQueue->pids[0]=0;
    fprintf(file3,"Time interval for this process is: %d \n start time is: %d \n",processQueue->timeInterval,processQueue->startTime);
    int t;
    int proSeq[20];
    t = processQueue->timeInterval ;
    int k = 0;
    int burst[processQueue->n];
    int pids[processQueue->n];
    int time = 0;
    int i=0;
    int j=0;
    int x=0;
    int z=4;
    int f=0;
    int r=0;
    int p=0;
    int previousTime=processQueue->startTime;
    int temp2=0;
    int old = 0;    
    //Arrival [0,0,2,4,6]
    //Burst [10,3,7,1,5]
    //Priority [1,4,2,2,3]
    readyQueue->front = 0;
    k++;
    checkTime(processQueue,readyQueue,i);
    bubbleSort(readyQueue,processQueue); //sort queue for shortest time
    readyQueue->priority[f] = front(readyQueue);
    f++;
    fprintf(file3,"t = 0\nCPU: Loading process %d (CPU burst = %d)\n",front(readyQueue),front(processQueue));
    i = 0;

while(i < processQueue->totalTime) //Run for total time
{
    
    i++;
    processQueue->array[processQueue->front]--;
    checkTime(processQueue,readyQueue,i);
   
  //  fprintf(file3,"%d \n \n",front(readyQueue));
  //  fprintf(file3,"old %d \n \n",old);

    if(old != front(readyQueue)){ //if process has changed 
       readyQueue->priority[f] = old;
       f++;
    }


    if(i % t == 0)  {

        if(front(processQueue) > rear(processQueue) ){
        fprintf(file3,"t = %d\nCPU: Preempting process %d (remaining CPU burst %d \n",i,front(readyQueue),front(processQueue));
        fprintf(file3,"Loading process %d (CPU burst = %d) \n",rear(readyQueue),rear(processQueue));
    
        proSeq[r] = front(readyQueue);
        r++;
        //Swap the tail and the second then rearrange the rest of the queue
        temp2 = processQueue->array[processQueue->front+1];
        temp = readyQueue->array[readyQueue->front + 1];
        readyQueue->array[readyQueue->front + 1] = rear(readyQueue);
        readyQueue->array[readyQueue->rear] = temp;
        processQueue->array[processQueue->front + 1] = rear(processQueue);
        processQueue->array[processQueue->rear] = temp2;
        bubbleSort(readyQueue,processQueue);
      
        print2(readyQueue,file3);
        //readyQueue->priority[z] = front(readyQueue);
        //z++;
  
    }
//if burst is finishing
    else if(front(processQueue) == 0) {
            
            fprintf(file3,"t = %d \nCPU Finishing process %d \n",i,front(readyQueue));
            print2(readyQueue,file3);
        }
       
//if process is running
     else {
            if(readyQueue->size <=1)
            {
            fprintf(file3,"t = %d\n CPU Running Process %d: (CPU burst %d) \n",i,p,front(processQueue));
            print2(readyQueue,file3);
            }
        else{
           
            fprintf(file3,"t = %d\nCPU Finishing Process %d; Loading Process %d (CPU burst %d) \n",i,front(readyQueue),readyQueue->array[readyQueue->front+1],processQueue->array[processQueue->front+1]);
            print2(readyQueue,file3);
            bubbleSort(readyQueue,processQueue);
}
         }
    }
        old = front(readyQueue);

     if(front(processQueue)==0)
        {
        proSeq[r] = front(readyQueue);
        r++;
        readyQueue->pids[x] = front(readyQueue);
        readyQueue->burst[x] = processQueue->burst[front(readyQueue)];                           
        k++;
        readyQueue->priority[z] = front(readyQueue);
        z++;
        dequeue(readyQueue);               
        dequeue(processQueue); 
        p = front(readyQueue); 
        bubbleSort(readyQueue,processQueue);
                                        
        x = x + 1;     
        }

}

   fprintf(file3,"**********************************************\nSTFC Summary (WT = wait time,  TT = turnaround time): \nPID \t WT \t TT\n");
 z=3;
 wt3(readyQueue,processQueue,z,file3);
 for(int i = 0; i<r; i++)
{
fprintf(file3,"%d",proSeq[i]);
}
processQueue->contextSwitch[3] = r;
fprintf(file3," Context Switches %d \n",processQueue->contextSwitch[3]);
conTimes[3] =r;
}




