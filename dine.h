#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <errno.h>

//make the number of philosophers a variable
#ifndef NUM_PHILOSOPHERS
#define NUM_PHILOSOPHERS 5
#endif

//my macros
#define EAT 'E'
#define THINK 'T'
#define CHANGE 'C'


//create an array to store philosophers and forks
sem_t forks[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];
int phil_id[NUM_PHILOSOPHERS];
char philosopher_state[NUM_PHILOSOPHERS];
int who_got_da_fork[NUM_PHILOSOPHERS];

//binary semaphore for printing
sem_t printing_semaphore;

//function prototypes
void* philosopher(void* phil_id);
void create_forks();
void clean_forks();
void dawdle();
