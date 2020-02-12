#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum Status           /* Defines an enumeration type    */  
{  
    NewToReady,
    ReadyToRunning,
    RunningToBlocked,
    RunningToTerminated,
}Status;

// enum typeOfEvent           /* Defines an enumeration type    */  
// {  
//     ARRIVAL,
//     CPU_BURST,
//     IO_BURST,
//     TERMINATION,
// };

typedef struct Event
{
    enum Status s;
    int time;
    int tid;
    int pid;
}Event;

typedef struct Thread
{
    int pid, tid; //Process and Thread id
    struct Event *CPU; //CPU bursts
    struct Event *IO; //IO bursts
    //enum Status s;
    int arrivalTime; //Arrival time
    int numCPU, numIO; //Number of cpu events //Number of io events
    int waitTime, burstTime; //Burst time (IO + CPU) and wait time is all burst times before added up
    int totalIO, totalCPU; //IO and CPU sums
    int TAT, finishTime, rt; //rt = remaining time
}Thread;

typedef struct Process
{
    int pid; /* Process ID */
    struct Thread *t; /*Array of threads*/
    int numThreads;

}Process;

int isNumber(char* string);

Process* readInput(int* numProcesses, int* threadSwitch, int* processSwitch);

Thread* FCFS(Process *p, int numProcesses, int processSwitch, int threadSwitch, int* k);

Thread* RR(Process *p, int numProcesses, int processSwitch, int threadSwitch, int* k, int quantum);

void verboseFCFS(Thread *t2, int k, int processSwitch, int threadSwitch);

void verboseRR(Thread *t2, int k, int quantum, int threadSwitch, int processSwitch);

void printDetail(Thread* list, int size);

void sortAndPrint(Event *list, int j);

int main(int argc, char* argv[])
{
    Process *p;
    int quantum = 0;
    int numProcesses, threadSwitch, processSwitch, size = 0;
    Thread *fcfsOrder = NULL, *rr = NULL;

    p = readInput(&numProcesses, &threadSwitch, &processSwitch);

    if(argc > 5)
    {
        printf("Too little or too many arguments\n");
        exit(0);
    }
    else if(argc == 1)
    {
        fcfsOrder = FCFS(p, numProcesses, processSwitch, threadSwitch, &size);
    }
    else if(argc == 2)
    {
        if(strcmp(argv[1], "-v") == 0)
        {
            fcfsOrder = FCFS(p, numProcesses, processSwitch, threadSwitch, &size);
            verboseFCFS(fcfsOrder, size, processSwitch, threadSwitch);
        }
        else if(strcmp(argv[1], "-d") == 0)
        {
            fcfsOrder = FCFS(p, numProcesses, processSwitch, threadSwitch, &size);
            printDetail(fcfsOrder, size);
        }
        else
        {
            printf("Unknown format\n");
            exit(0);
        }
    }
    else if(argc == 3)
    {
        if((strcmp(argv[1], "-d") == 0 && strcmp(argv[2], "-v") == 0))
        {
            fcfsOrder = FCFS(p, numProcesses, processSwitch, threadSwitch, &size);
            printDetail(fcfsOrder, size);
            verboseFCFS(fcfsOrder, size, processSwitch, threadSwitch);
        }
        else if(strcmp(argv[1], "-r") == 0 && isNumber(argv[2]))
        {
            quantum = atoi(argv[2]);
            rr = RR(p, numProcesses, processSwitch, threadSwitch, &size, quantum);
            //printDetail(rr, size);
        }
        else
        {
            printf("Unknown format\n");
            exit(0);
        }
    }
    else if(argc == 4)
    {
        if(strcmp(argv[1], "-d") == 0 && strcmp(argv[2], "-r") == 0 && isNumber(argv[3]))
        {
            quantum = atoi(argv[3]);
            rr = RR(p, numProcesses, processSwitch, threadSwitch, &size, quantum);
            printDetail(rr, size);
        }
        else if(strcmp(argv[1], "-v") == 0 && strcmp(argv[2], "-r") == 0 && isNumber(argv[3]))
        {
            quantum = atoi(argv[3]);
            rr = RR(p, numProcesses, processSwitch, threadSwitch, &size, quantum);
            verboseRR(rr, size, quantum, threadSwitch, processSwitch);
        }
        else
        {
            printf("Unknown format\n");
            exit(0);
        }
    }
    else if(argc == 5)
    {
        if(strcmp(argv[1], "-d") == 0 && strcmp(argv[2], "-v") == 0 && strcmp(argv[3], "-r") == 0 && isNumber(argv[4]))
        {
            quantum = atoi(argv[4]);
            rr = RR(p, numProcesses, processSwitch, threadSwitch, &size, quantum);
            printDetail(rr, size);
            verboseRR(rr, size, quantum, threadSwitch, processSwitch);
        }
        else
        {
            printf("Unknown format\n");
            exit(0);
        }
    }
}

Process* readInput(int* numProcesses, int* threadSwitch, int* processSwitch)
{   
    char line[200];

    fgets(line, 200, stdin);

    *numProcesses = atoi(&line[0]);
    *threadSwitch = atoi(&line[2]);
    *processSwitch = atoi(&line[4]);
    
    Process *p = malloc(sizeof(Process) * *numProcesses);

    for(int i = 0; i<*numProcesses;i++) //Each process
    {
        fgets(line, 200, stdin);

        p[i].pid = atoi(&line[0]);
        p[i].numThreads = atoi(&line[2]);

        p[i].t = malloc(sizeof(Thread) * p[i].numThreads);

        for(int j = 0; j<p[i].numThreads;j++) //Each thread
        {
            fgets(line, 200, stdin);

            p[i].t[j].pid = p[i].pid;
            p[i].t[j].tid = atoi(&line[0]);
            p[i].t[j].arrivalTime = atoi(&line[2]);
            p[i].t[j].numCPU = atoi(&line[4]);
            p[i].t[j].numIO = p[i].t[j].numCPU -1;

            p[i].t[j].CPU = malloc(sizeof(Event) * p[i].t[j].numCPU);

            p[i].t[j].IO = malloc(sizeof(Event) * p[i].t[j].numIO);

            int k;
            for(k = 0; k<p[i].t[j].numCPU; k++)
            {
                if(k == (p[i].t[j].numCPU -1))//Last one
                {
                    fgets(line, 200, stdin);

                    p[i].t[j].CPU[k].time = atoi(&line[2]);

                    p[i].t[j].totalCPU += p[i].t[j].CPU[k].time;

                    p[i].t[j].burstTime += p[i].t[j].CPU[k].time;

                }
                else
                {
                    fgets(line, 200, stdin);

                    p[i].t[j].CPU[k].time = atoi(&line[2]);

                    p[i].t[j].IO[k].time = atoi(&line[4]);

                    p[i].t[j].totalCPU += p[i].t[j].CPU[k].time;

                    p[i].t[j].totalIO += p[i].t[j].IO[k].time;

                    p[i].t[j].burstTime += p[i].t[j].CPU[k].time;

                    p[i].t[j].burstTime += p[i].t[j].IO[k].time;

                }
            }
        }
    }

    return p;

}

Thread* FCFS(Process *p, int numProcesses, int processSwitch, int threadSwitch, int* k)
{
    static Thread t2[50];
    double totaltime = 0.0, totalCPU = 0.0;
    double avgTAT = 0.0;

    for(int i = 0; i<numProcesses; i++) //Putting the threads in an array
    {
        for(int j = 0; j<p[i].numThreads;j++)
        {
            t2[*k] = p[i].t[j];
            *k = *k+1;
        }
    }

    for(int i = 0; i<*k; i++) //Sorting the array
    {
        for(int j = 0; j<*k-1; j++)
        {
            if(t2[j].arrivalTime>t2[j+1].arrivalTime)
            {
                Thread temp = t2[j+1];

                t2[j+1] = t2[j];

                t2[j] = temp;
            }
        }
    }
    t2[0].waitTime = 0;
    //Calculating wait time
    for(int i=1;i<*k;i++)
    {
        t2[i].waitTime=0;
        for(int j=0;j<i;j++)
        {
            t2[i].waitTime+=t2[j].burstTime;
        }
    }

    //Calculating total time
    int i;
    for(i = 0; i<*k;i++) //All threads
    {
        if(i<*k-1 && (t2[i].pid != t2[i+1].pid)) //If switching processes (ignoring the last one)
        {
            totaltime = totaltime + processSwitch;
        }
        else if(i<*k-1 && (t2[i].tid != t2[i+1].tid))
        {
            totaltime = totaltime + threadSwitch;
        }

        for(int j = 0; j<t2[i].numCPU; j++)
        {
            totaltime = totaltime + t2[i].CPU[j].time;
            totalCPU = totalCPU + t2[i].CPU[j].time;
        }

        for(int j = 0; j<t2[i].numIO; j++)
            totaltime = totaltime + t2[i].IO[j].time;

        avgTAT += t2[i].waitTime + t2[i].burstTime;
    }

    avgTAT = avgTAT/i;

    double CPUUtilization = ((totalCPU/totaltime) * 100);

    printf("\n------------------------------------------\n");
    printf("FCFS:\n");
    printf("Total time required is %.0lf time units\n", totaltime);
    printf("Average Turnaround time is %.2lf\n", avgTAT);
    printf("CPU Utilization is %.2lf%%\n", CPUUtilization);
    printf("------------------------------------------\n");

    return t2;
}

Thread* RR(Process *p, int numProcesses, int processSwitch, int threadSwitch, int* k, int quantum)
{
    static Thread t2[50];
    int count = 0, remain, flag = 0, time = 0;
    double avgTAT = 0.0; //remaining time

    for(int i = 0; i<numProcesses; i++) //Putting the threads in an array
    {
        for(int j = 0; j<p[i].numThreads;j++)
        {
            t2[*k] = p[i].t[j];
            *k = *k+1;
        }
    }

    for(int i = 0; i<*k; i++) //Sorting the array
    {
        for(int j = 0; j<*k-1; j++)
        {
            if(t2[j].arrivalTime>t2[j+1].arrivalTime)
            {
                Thread temp = t2[j+1];

                t2[j+1] = t2[j];

                t2[j] = temp;
            }
        }
    }

    for(int i = 0; i<*k;i++) //Setting remaining time for all threads
    {
        t2[i].rt = t2[i].burstTime;
    }

    remain = *k;
    int prevID = 0;
    while(remain != 0)
    {
        if(t2[count].rt<=quantum && t2[count].rt>0) //Time remaining is < quantum
        { 
            if(prevID != 0 && prevID != t2[count].pid) //Process Switch
            {   
                time += processSwitch;
            }
            else if(prevID !=0) //Thread switch
            {
                time += threadSwitch;
            }
            time += t2[count].rt; 
            t2[count].rt=0; 
            flag=1; 
            prevID = t2[count].pid;
        }
        else if(t2[count].rt>0) //Time remaining > quatum
        { 
            if(prevID != 0 && prevID != t2[count].pid) //Process Switch
            {   
                time += processSwitch;
            }
            else if(prevID !=0) //Thread switch
            {
                time += threadSwitch;
            }
            t2[count].rt -= quantum; 
            time += quantum; 
            prevID = t2[count].pid;
        } 

        if(t2[count].rt==0 && flag==1) //Process finished
        { 
            remain--; 
            t2[count].waitTime += time - t2[count].arrivalTime - t2[count].burstTime;
            t2[count].TAT += time - t2[count].arrivalTime; 
            t2[count].finishTime = time;
            flag=0; 
        } 
        if(count==*k-1) 
            count=0; 
        else if(t2[count+1].arrivalTime<=time) 
            count++; 
        else 
            count=0;
    }

    int totalCPU = 0;

    for(int i = 0; i<*k; i++) //All threads
    {
        avgTAT += t2[i].TAT;

        totalCPU += t2[i].totalCPU;

        // for(int j = 0; j<t2[i].numCPU; j++) //All cpu bursts
        //     totalCPU = totalCPU + t2[i].CPU[j].time;
    }

    avgTAT = avgTAT/ *k;

    printf("CPU: %d Time: %d \n", totalCPU, time);

    printf("\n------------------------------------------\n");
    printf("RR:\n");
    printf("\nTotal time required is %d time units\n", time);
    printf("Average Turnaround time is %.2lf\n", avgTAT);
    printf("CPU Utilization is %.2lf%%\n", ((double)totalCPU/time * 100));
    printf("------------------------------------------\n");

    return t2;
}

void printDetail(Thread* list, int size)
{
    for(int i = 0; i<size;i++) //All the threads in the ordered list
    {
        printf("\nThread %d of Process %d:\n\n", list[i].tid, list[i].pid);
        printf("Arrival time: %d \n", list[i].arrivalTime); //Arrival time
        printf("Service time: %d units\n", list[i].totalCPU); //Sum of cpu bursts
        printf("I/O Time: %d units\n", list[i].totalIO); //Sum of io bursts
        printf("Turnaround time: %d units\n", list[i].waitTime + list[i].burstTime); //Turnaround
        printf("Finish time: %d\n", list[i].arrivalTime + list[i].waitTime + list[i].burstTime); //Arrival + turnaround
    }
}

void verboseFCFS(Thread *t2, int k, int processSwitch, int threadSwitch)
{
    int totaltime = 0, j = 0;
    Event list[200];
    for(int i = 0; i<k;i++) //All threads
    {
        //Adding arrival
        list[j].time = t2[i].arrivalTime;
        list[j].s = NewToReady;
        list[j].tid = t2[i].tid;
        list[j].pid = t2[i].pid;
        j++;
        //Adding running
        list[j].time = totaltime;
        list[j].s = ReadyToRunning;
        list[j].tid = t2[i].tid;
        list[j].pid = t2[i].pid;
        j++;
        //printf("At time %d: Thread %d of Process %d moves from new to ready\n", totaltime, t2[i].tid, t2[i].pid);
        if(i<k-1 && (t2[i].pid != t2[i+1].pid)) //If switching processes (ignoring the last one)
        {
            totaltime = totaltime + processSwitch;
        }
        else if(i<k-1 && (t2[i].tid != t2[i+1].tid)) //If switching threads
        {
            totaltime = totaltime + threadSwitch;
        }

        for(int j = 0; j<t2[i].numCPU; j++)
        {
            totaltime = totaltime + t2[i].CPU[j].time;
        }

        for(int j = 0; j<t2[i].numIO; j++)
            totaltime = totaltime + t2[i].IO[j].time;
        //Adding terminated time
        list[j].time = totaltime;
        list[j].s = RunningToTerminated;
        list[j].tid = t2[i].tid;
        list[j].pid = t2[i].pid;
        j++;
    }

    sortAndPrint(list, j);
}

void verboseRR(Thread *t2, int k, int quantum, int threadSwitch, int processSwitch)
{
    int remain = k, j = 0, flag = 0;
    int prevID = 0, count = 0, time = 0;
    Event list[50000];

    for(int i = 0; i<k;i++) //Setting remaining time for all threads
    {
        t2[i].rt = t2[i].burstTime;
    }
    for(int i = 0; i<k;i++) //Adding arrival times
    {
        //Adding arrival
        list[j].time = t2[i].arrivalTime;
        list[j].s = NewToReady;
        list[j].tid = t2[i].tid;
        list[j].pid = t2[i].pid;
        j++;
    }

    while(remain != 0)
    {
        //printf("Remain: %d\n", t2[count].rt);
        if(t2[count].rt<=quantum && t2[count].rt>0) //Time remaining is < quantum
        { 
            if(t2[count].rt == t2[count].burstTime) //Just started
            {
                list[j].time = time;
                list[j].s = ReadyToRunning;
                list[j].tid = t2[count].tid;
                list[j].pid = t2[count].pid;
                j++;
            }

            if(prevID != 0 && prevID != t2[count].pid) //Process Switch
            {   
                time += processSwitch;
            }
            else if(prevID !=0) //Thread switch
            {
                time += threadSwitch;
            }
            time += t2[count].rt; 
            t2[count].rt=0; 
            flag=1; 
            prevID = t2[count].pid;

        }
        else if(t2[count].rt>0) //Time remaining > quatum
        { 
            if(t2[count].rt == t2[count].burstTime) //Just started
            {
                list[j].time = time;
                list[j].s = ReadyToRunning;
                list[j].tid = t2[count].tid;
                list[j].pid = t2[count].pid;
                j++;
            }

            if(prevID != 0 && prevID != t2[count].pid) //Process Switch
            {   
                time += processSwitch;
            }
            else if(prevID !=0) //Thread switch
            {
                time += threadSwitch;
            }
            t2[count].rt -= quantum; 
            time += quantum; 
            prevID = t2[count].pid;

            list[j].time = time;
            list[j].s = RunningToBlocked;
            list[j].tid = t2[count].tid;
            list[j].pid = t2[count].pid;
            j++;
        } 

        if(t2[count].rt==0 && flag==1) //Process finished
        { 
            list[j].time = time;
            list[j].s = RunningToTerminated;
            list[j].tid = t2[count].tid;
            list[j].pid = t2[count].pid;
            j++;

            remain--; 
            t2[count].waitTime += time - t2[count].arrivalTime - t2[count].burstTime;
            t2[count].TAT += time - t2[count].arrivalTime; 
            t2[count].finishTime = time;
            flag=0; 
        } 

        if(count==k-1) 
            count=0; 
        else if(t2[count+1].arrivalTime<=time) 
            count++; 
        else 
            count=0;
    }
    sortAndPrint(list, j);
}

void sortAndPrint(Event *list, int j)
{
    //Sorting event list
    for(int i = 0; i<j; i++) //Sorting the array
    {
        for(int m = 0; m<j-1; m++)
        {
            if(list[m].time>list[m+1].time)
            {
                Event temp = list[m+1];

                list[m+1] = list[m];

                list[m] = temp;
            }
        }
    }
    //Print all events
    for(int i = 0; i<j;i++)
    {
        if(list[i].s == NewToReady)
        {
            printf("\nAt time %d: Thread %d of Process %d moves from NEW to READY\n", list[i].time, list[i].tid, list[i].pid);
        }
        else if(list[i].s == ReadyToRunning)
        {
            printf("\nAt time %d: Thread %d of Process %d moves from READY to RUNNING\n", list[i].time, list[i].tid, list[i].pid);
        }
        else if(list[i].s == RunningToTerminated)
        {
            printf("\nAt time %d: Thread %d of Process %d moves from RUNNING to TERMINATED\n", list[i].time, list[i].tid, list[i].pid);
        }
        else if(list[i].s == RunningToBlocked)
        {
            printf("\nAt time %d: Thread %d of Process %d moves from RUNNING to BLOCKED\n", list[i].time, list[i].tid, list[i].pid);
        }
    }
}

int isNumber(char* string)
{
    for (int i = 0; string[i] != '\0'; i++)
    {
        //if (number[i] > '9' || number[i] < '0')
        if (!isdigit(string[i]))
            return 0;
        
        if(string[i] == 0)
            return 0;
    }
    return 1;
}