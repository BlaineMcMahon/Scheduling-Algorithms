//Blaine McMahon
//01559746

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

//Queue Data Structure
struct Queue{

    int* array;               //Queue to keep track of processes 
    int capacity;             //Capacity of the Queue
    int front;                //Front points to the front element of the Queue
    int rear;                 //Rear points to the last element in the Queue
    int count;                //Holds the current size of the Queue
    int size;                 //Size of Array
    int* burst;               //Array to keep track of burst
    int* priority;            //Array to keep track of priority
    int* arrival;             //Array to keep track of arrival times
    int* pids;                //Array to keep track of pid numbers       
    int pid;                  //Keeps track of processes    
    int currentTime;          //Current time variable 
    int timeInterval;         //Time interval
    int startTime;            //Store start time for first process
    int totalTime;            //Store end time of last process
    int contextSwitch[5];     //Keep track of context switches
    int n;                    //Holds size of total elements
    int finishTime[5];
 };

//global variables
extern float WTtimes[5];     //Global for wait times
extern float TATtimes[5];   //Global for turnaround times
extern int conTimes[5];    //Global for context switch




//---------------------P R O T O T Y P E S-------------------------

/*-------------------- Q U E U E  F U N C T I O N S---------------*/
struct Queue* createQueue(int capacity);        //Create queue 
int isFull (struct Queue* queue);               //Check if queue is full
int isEmpty(struct Queue* queue);               //Check if queue is empty
void enqueue(struct Queue* queue, int item);    //Add to the queue
int dequeue(struct Queue* queue);               //Remove from queue
int front(struct Queue* queue);                 //Get front element of queue
int rear(struct Queue* queue);                  //Get back element of queue
void print(struct Queue* queue,FILE* file3);    //Print elements in queue
void print2(struct Queue* queue,FILE* file3);   //Variation to print
/*-------------------- F U N C T I O N S---------------*/
void checkTime(struct Queue* processQueue,struct Queue* readyQueue,int time);                             //Checks if any processes have arrived
void checkTime2(struct Queue* processQueue,struct Queue* readyQueue,struct Queue* priorityQueue,int time);//Variation to check  
void startTime(struct Queue* queue);                                                                      //Find start time 
void wt(struct Queue* queue,struct Queue* ready,FILE* file3);                                             //Calculate wt and tat
void wt2(struct Queue* queue,struct Queue* ready,int k, FILE* file3);                                     //Variation to wt
void wt3(struct Queue* readyQueue,struct Queue* processQueue,int k,FILE* file3);                          //Variation to wt2
void bubbleSort(struct Queue* processQueue, struct Queue* readyQueue);                                    //Sort queues from smallest to largest 
void bubbleSort2(struct Queue* processQueue, struct Queue* readyQueue,struct Queue* burstQueue);          //Variation to bubbleSort
void LeftRotatebyOne(struct Queue* processQueue, struct Queue* readyQueue);                               //Rotate all elements to left
/*-------------------- A L G O R I T H M S---------------*/
void FCFS(struct Queue* ready, struct Queue* process,FILE* file3);
void SJF(struct Queue* ready, struct Queue* process, FILE* file3);
void PRIORITY(struct Queue* ready, struct Queue* process,struct Queue* priorityQueue, FILE* file3);
void STFC(struct Queue* ready, struct Queue* process,FILE* file3);
void RR(struct Queue* ready, struct Queue* process, FILE* file3);







