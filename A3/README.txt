/**************************************************
Loui Zibdawi                CIS 3110

0924538                     Assignment 3

Mar 27th
****************************************************/

********
Compile
********
Navigate to CIS3110-A1 folder

Type: make

********
Usage
********
For dining Philosopher:

./philosophers 3 2

first number = amount of philosophers
second number = amount of times they eat

For holes:

./holes testfile1.txt

2nd argument is the test file

***********
Algorithms
***********

Dining Philosphers:

The Dining Philosopher problem is a classic computer science problem that addresses mutual exclusion. In my variation of
the algorithm, I allow the user to input the amount of philosophers eating as well as how many times they have to eat.
The Philosophers need to be holding 2 forks to be able to eat. They will take turns sharing the forks so that they all eat.
My algorithm avoid deadlock and starvation by causing the 1st philosopher to sleep for longer than the others.

Memory Management:

First fit:

This algorithm looks for the first hole in memory that can fit a process and it
inserts the process in that hole.

Best fit:

This algorithm finds all the holes in memory and then depicts the best fitting hole
by looking at the smallest hole that can fit the current process being trying to put
in memory.

Worst fit:

This algorithm finds all the holes in memory and then puts the process in the biggest
of those holes.

Next fit:

Similar to first fit, this algorithm looks for the hole closest to where the last insert
was peformmed and puts the process in there.

***********
ASSUMPTIONS
***********

As of now all my algorithms for the memory management give the same results, although when I
look through them load by load, they put processes in different locations. I believe this is a
result of having a small input file and when the input files become larger there will be variance
in the results.

