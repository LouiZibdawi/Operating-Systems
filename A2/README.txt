/**************************************************
Loui Zibdawi                CIS 3110

0924538                     Assignment 2

Feb 27th
****************************************************/

********
Compile
********
Navigate to CIS3110-A1 folder

Type: make

********
Usage
********
./simcpu < inputfile

Optional flags: [-d] [-v] [-r 10(num)] *exclude square brackets*

***********
Algorithms
***********

First Come First Serve:

The First-Come-First-Served algorithm is often considered the simplest scheduling algorithm. Processes are dispatched 
according to their arrival time on the ready queue. Being a nonpreemptive discipline, once a process has a CPU, it 
runs to completion. The FCFS scheduling is fair in the formal sense or human sense of fairness but it is unfair 
in the sense that long jobs make short jobs wait and unimportant jobs make important jobs wait.

Round Robin:

The Round Robin algorithm is an approach to scheduling where each thread has a set time they are allowed to run at a
time, called the quantum. After the quantum expires, the ready queue is searched and if there are other threads ready,
then they are run. This avoids the problem that arises with FCFS which is that some processes which arrive later will
have to wait until everything before them is done (which can be very long at times).

***********
Description
***********

i. Does your simulator include switch overhead for the first ready state to
running state transition? Explain.

    - Yes. I decided on including a switch overhead on my first ready to running switch
    because I see the overhead as the cost to stop a process as well as start up a
    new one. Therefore since there still is going to be a charge to start up the first 
    process, I thought it was appropriate to add a overhead charge to the first transition
    between ready and running state.

ii. Does your simulator include switch overhead if a thread moves from ready
state to running state and the CPU is idle? Explain.

    - Yes. Similar to the first answer, I believe that there should be a charge to start
    up a process, whether or not a process is being ended and switched to it right before.
    By that logic, I decided it is appropriate to add a overhead charge to a thread even
    when the cpu is idle.

iii. Does your simulator include switch overhead if a thread moves from
running state to blocked state and the ready queue is empty? Explain.

    - Yes. I included an overhead cost in this specific case because in order to stop a
    process I believe it should take some amount of time. Therefore I found it more suitable
    to include the process/thread switch cost, rather than making it free.

iv. Does your simulation include switch overhead if a thread is interrupted
(due to a time slice) and either the ready queue is empty or the thread has
the highest priority? Explain. 
    
    - Yes. Much like the previous answers, no matter what the situation is, if there is a need
    to stop a thread, I included an overhead charge.


