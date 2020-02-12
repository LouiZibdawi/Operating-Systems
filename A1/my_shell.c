#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>

extern char **getln();

int isHex(char **args, int i);

int isNumber(char **args, int i);

int main() {
	int i, status, numArrows, flag = 0, valid = 0, sum, numArrows2, result, background;
	char **args; 
	char fileName[50], inputFile[50];
	
	while(1) 
	{
		background = 0;
		sum = 0;
		result = 0;
		valid = 1;
		numArrows = 0;
		numArrows2 = 0;
		flag = 0;

		printf("/>");
		args = getln();
		for(i = 0; args[i] != NULL; i++)
		{

		}

		if(i < 1 || flag == 1)
			continue;

		if(strcmp(args[i-1], "&") == 0)
		{
			background = 1;
			args[i-1] = NULL;
		}

		// Setting i to num of args
		for(i = 0; args[i] != NULL; i++)
		{
			//printf("%d\n", i);
			if(strcmp(args[i], ">") == 0) //Found file direction
			{
				numArrows++;
				if(args[i+1] == NULL)
				{
					printf("Error: Filename not present\n");
					flag = 1;
					break;
				}
				else if(args[i+2] != NULL)
				{
					printf("Error: Can't print to two or more files\n");
					flag = 1;
					break;
				}
				else if(strcmp(args[i+1], ">")==0)
				{
					printf("Error: File name can not be '>' \n");
					flag = 1;
					break;
				}
				else
				{
					strcpy(fileName, args[i+1]);
					args[i] = NULL;
					args[i+1] = NULL;
					break;
				}
			}
	
			if(strcmp(args[i], "<") == 0) // input file direction
			{
				numArrows2++;
				if(args[i+1] == NULL)
				{
					printf("Error: Filename not present\n");
					flag = 1;
					break;
				}
				else if(args[i+2] != NULL)
				{
					printf("Error: Can't print to two or more files\n");
					flag = 1;
					break;
				}
				else if(strcmp(args[i+1], "<")==0)
				{
					printf("Error: File name can not be '<' \n");
					flag = 1;
					break;
				}
				else
				{
					strcpy(inputFile, args[i+1]);
					args[i] = NULL;
					args[i+1] = NULL;
					break;
				}
			}
		}
		
		if(i < 1 || flag == 1)
			continue;
		
		if(strcmp(args[0], "exit") == 0)
			exit(0);
		else if(strcmp(args[0], "add") == 0)
		{
			if(args[2] == NULL)
			{
				printf("Error: Not enough arguments\n");
				continue;
			}

			for(i = 1; args[i] != NULL; i++)
			{
				valid = isHex(args, i);

				if(valid == 0)
					valid = isNumber(args, i);
				else
					sum = sum + strtol(args[i], NULL, 0);
				
				if(valid == 0)
				{
					printf("Error: All arguments must be decimal or hex number\n");
					flag = 1;
					break;
				}
				else
					sum = sum + strtol(args[i], NULL, 0);
				
			}

			if(flag == 1)
				continue;

			printf("%s", args[1]);

			for(int j = 2; j < i; j++)
			{
				printf(" + %s", args[j]);
			}

			printf(" = %d\n", sum);
		}
		else if(strcmp(args[0], "arg") == 0)
		{
			printf("argc = %d,", i-1);

			printf(" args =");

			for(int j = 1; j<i-1; j++)
			{
				printf(" %s,", args[j]);
			}
			printf(" %s\n", args[i-1]);
		}
		else if(strcmp(args[0], "subtract") == 0)
		{
			if(args[2] == NULL)
			{
				printf("Error: Not enough arguments\n");
				continue;
			}

			for(i = 1; args[i] != NULL; i++)
			{
				valid = isHex(args, i);

				if(valid == 0)
					valid = isNumber(args, i);
				else
				{
					if(i == 1)
						result = strtol(args[i], NULL, 0);
					else
						result = result - strtol(args[i], NULL, 0);
				}
				
				if(valid == 0)
				{
					printf("Error: All arguments must be decimal or hex number\n");
					flag = 1;
					break;
				}
				else
				{
					if(i == 1)
						result = strtol(args[i], NULL, 0);
					else
						result = result - strtol(args[i], NULL, 0);
				}
				
			}

			if(flag == 1)
				continue;

			printf("%s", args[1]);

			for(int j = 2; j < i; j++)
			{
				printf(" - %s", args[j]);
			}

			printf(" = %d\n", result);
		}
		//printf("2\n");
		//Fork process
		pid_t processID;

		processID = fork();

		if (processID < 0) //Error
		{
			printf("Error Found\n");
			exit(1);
		}
		else if (processID == 0) //Child
		{
			if(numArrows == 0 && numArrows2 != 1)
			{
				if(execvp(args[0], args) < 0)
					printf("Error: Unrecognized command\n");
			}
			else if(numArrows == 1)
			{
				FILE *fp;

				fp = freopen(fileName, "w+", stdout);

				if(execvp(args[0], args) < 0);
					printf("Error: Unrecognized command\n");

				fclose(fp);
			}

			if(numArrows2 == 0 && numArrows != 0)
			{
				if(execvp(args[0], args) < 0)
					printf("Error: Unrecognized command\n");
			}
			else if(numArrows2 == 1)
			{
				FILE *fp;

				fp = freopen(inputFile, "r", stdin);
	
				if(execvp(args[0], args) < 0)
					printf("Error: Unrecognized command\n");

				fclose(fp);
			}

		}
		else //Parent 
		{
			if(background == 0)
				while(wait(&status) != processID);
			//wait(&status);


			//else

		}

	}
}

int isHex(char **args, int i)
{
	int isHex = 0;

	if(args[i][0] == '0' && args[i][1] == 'x')
	{
		for(int j = 2; j<strlen(args[i]); j++)
		{
			if(isxdigit(args[i][j]) != 0)
				isHex = 1;
			else
			{
				isHex = 0;
				break;
			}
		}
	}
	if(isHex == 0)
		return 0;
	else
		return 1;
}

int isNumber(char **args, int i)
{
	int isDec = 0;

	if(args[i][0] == '-' || isdigit(args[i][0]) != 0)
		isDec = 1;

	for(int j = 1; j<strlen(args[i]); j++)
	{
		if(isdigit(args[i][j]) != 0)
			isDec = 1;
		else
		{
			isDec = 0;
			break;
		}
	}

	if(isDec == 0)
		return 0;
	else
		return 1;
}
