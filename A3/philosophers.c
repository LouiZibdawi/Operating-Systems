#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

int numPhils, numEat;
pthread_mutex_t food_lock;
pthread_t *philo;
pthread_mutex_t *spoon;

int isNum(char number[]);

void* philAction(int id);

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Incorrect amount of arguments\n");
        exit(1);
    }
    else if(!isNum(argv[1]) || atoi(argv[1]) < 3)
    {
        printf("The amount of philosphers must be larger than 2 and all digits\n");
        exit(1);
    }
    else if(!isNum(argv[2]) || atoi(argv[2]) <= 0 || atoi(argv[2]) > 1000)
    {
        printf("The number of times a philospher thinks must be between 1-1000 and all digits\n");
        exit(1);
    }

    numPhils = atoi(argv[1]);
    numEat = atoi(argv[2]);

    philo = malloc(numPhils * sizeof(pthread_t));
    spoon = malloc(numPhils * sizeof(pthread_mutex_t));

    pthread_mutex_init (&food_lock, NULL);

    for (int m = 0; m < numPhils; m++)
    {
        pthread_mutex_init (&spoon[m], NULL);
    }
    for (int m = 0; m < numPhils; m++)
    {
        pthread_create(&philo[m], NULL, (void*)philAction, (int*)m);
    }
    for (int m = 0; m < numPhils; m++)
    {
        pthread_join (philo[m], NULL);
    }
}

void* philAction(int id)
{
    //printf("Philospher %d\n", );
    int right = id;
    int left = id + 1;

    if(left == numPhils)
        left = 0;

    int food = numEat;

    while(food>0)
    {
        //sleep(rand()%4);
        printf ("Philosopher %d: thinking\n", id+1);
        pthread_mutex_lock (&food_lock);
        if (food > 0)
            food--;
        pthread_mutex_unlock (&food_lock);

        if(id == 1) //first philospher naps to avoid deadlock
            sleep(3);

        pthread_mutex_lock(&spoon[right]); //grab left
        pthread_mutex_lock(&spoon[left % numPhils]); //grab right

        printf ("Philosopher %d: eating\n", id+1);
        //usleep (5000 * (numEat - food + 1));
        sleep(3);
        pthread_mutex_unlock(&spoon[right]);
        pthread_mutex_unlock(&spoon[left % numPhils]);
    }

    printf ("Philosopher %d: thinking\n", id+1);
    return (NULL);
}

int isNum(char number[])
{
    int i = 0;

    //checking for negative numbers
    if (number[0] == '-')
        i = 1;
    for (; number[i] != 0; i++)
    {
        //if (number[i] > '9' || number[i] < '0')
        if (!isdigit(number[i]))
            return 0;
    }
    return 1;
}