/**************************************************
Loui Zibdawi				CIS 3110

0924538						Assignment 1

Jan 30th
****************************************************/

********
Compile
********
Navigate to CIS3110-A1 folder

Type: make

********
Usage
********
./msh

***********
Description
***********

My algorithm starts by giving the user a prompt for input. It then looks if the input has any args and if it does,
than it looks for the & at the end of the args. If it finds it, it turns on a flag and then erases it. My program
then proceeds to analyze the code, looking for arrows first and then more specific commands like "add", "subtract"
or "arg". The last thing I do fork into a parent and child process and call execvp or wait where necceassary.

I then preceed to analyze the information and finally return to the shell prompt. At many points in my program I
use flags to signal whether an error is reached or not so the function gets passed to the next iteration of the
while loop. Using exit is how you end the cycle. 


*******************************
Known Limitations & Assumptions
*******************************

 Some known limitations of my assignment are:

 - Can't take floats or doubles (decimals) in my add or subtract
 - Assuming that the & is at the end of the string when used
 - Assuming that the add arg and subtract don't need to work with >
 - Assume exit is going to be the first argument

 ***************
 Custom Command
 ***************
Example: subtract 100 210 0xa
Details: subtract all numbers and output the result
Output: 100 - 210 - 0xa = -120