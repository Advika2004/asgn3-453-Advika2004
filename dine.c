#include "dine.h"
#include "print.h"


//! need to take care of the condition 
//! if if there is only one philosopher because that is hanging 


//function prototypes
void* philosopher(void* phil_id);
void create_forks();
void clean_forks();
void dawdle();
 
 int num_cycles = 1;

int main(int argc, char* argv[]) {

    print_the_top();

    //initialize the semaphore for printing
    sem_init(&printing_semaphore, 0, 1);

    //if the user provided the additional optional argument
    //make the string argument back into an int and assign it to the global
    if (argc > 1) {
        num_cycles = atoi(argv[1]); 
    }

    int i;

    //make all philosophers in the changing state upon start
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
    philosopher_state[i] = CHANGE;
    }

    //creates the forks for the meal
    create_forks();

    //create an ID for each philosopher
    //also create the philosophers

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        phil_id[i] = i;
        int status = pthread_create(&philosophers[i], 
                     NULL, philosopher, (void*)&phil_id[i]);

        if (status == -1){
            fprintf(stderr, "philosopher %i: %s\n", i, strerror(status));
            exit(-1);
        }
    }

    // wait for the threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    //cleanup semaphores when done
    clean_forks(); 

    print_the_bottom();

    return 0;
}

void* philosopher(void* phil_id) {

    //!why is phil_id cast to void* then cast back to int and deref?

    //the id is the id identifying each philosopher. ex: philosopher 1
    int id = *(int *)phil_id;
    
    //printf("philosopher %d wants to eat\n", id);

    //its left fork will be fork 1
    int left_fork = id;

    //its right fork will be 2
    //need modulo its a circle the last philosophers right becomes the first
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    //need to loop through the number of eat think wait cycles the user wants
    int i;
    for (i = 0; i < num_cycles; i++) {

        //have each philosopher pick up the fork
        //if the philosopher has an even id, pick up the right first then left
        //then switch for odd to avoid deadlock 
        if (id % 2 == 0) {
            //printf("philosopher %d tries to pick up right fork\n", id);
            sem_wait(&forks[right_fork]);
            //printf("philosopher %d picked up right fork\n", id);

            //printf("philosopher %d tries to pick up left fork\n", id);
            sem_wait(&forks[left_fork]);
            //printf("philosopher %d picked up left fork\n", id);
        }

        //odd does the opposite
        else {
            //printf("philosopher %d tries to pick up left fork\n", id);
            sem_wait(&forks[left_fork]);
            //printf("philosopher %d picked up left fork\n", id);

            //printf("philosopher %d tries to pick up right fork\n", id);
            sem_wait(&forks[right_fork]);
            //printf("philosopher %d picked up right fork\n", id);
        }

        //if philosopher has both forks, they will eat
        //EATING:
        philosopher_state[id] = 'E';
        //printf("CURRENT PHILOSOPHER WHO'S STATE IS BEING CHANGED: %d\n", id);
        //! is this supposed to be printed here? 
        print_the_middle();
        //printf("forks picked up for philosopher %d. NOW EATING\n", id);
        dawdle();

        //if philosopher has eaten, then they will think
        //to think, they have to put down both forks first
        //THINKING:
       //printf("philosopher %d puts down left fork\n", id);
        sem_post(&forks[left_fork]);
        //printf("philosopher %d puts down right fork\n", id);
        sem_post(&forks[right_fork]);
        philosopher_state[id] = 'T';
        //printf("CURRENT PHILOSOPHER WHO'S STATE IS BEING CHANGED: %d\n", id);
        //! is this supposed to be printed here?
        print_the_middle();
        //printf("forks returned for philosopher %d. NOW THINKING\n", id);
        dawdle();
    }

    return NULL;
}


//function will create as many semaphores as there are philosophers
void create_forks() {
    int i;
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        //not shared and free upon start
        sem_init(&forks[i], 0, 1);
    }
}

//at the end of the program takes care of deallocating the semaphores
//or "cleaning the forks" if you will haha clever right haha thanks
void clean_forks() {
    int i;
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }
}

void dawdle() {
/*
* sleep for a random amount of time between 0 and 999
* milliseconds. This routine is somewhat unreliable, since it
* doesn’t take into account the possiblity that the nanosleep
* could be interrupted for some legitimate reason.
*
* nanosleep() is part of the realtime library and must be linked
* with –lrt
*/
    struct timespec tv;
    int msec = (int)(((double)random() / RAND_MAX) * 1000);
    tv.tv_sec = 0;
    tv.tv_nsec = 1000000 * msec;
    if (-1 == nanosleep(&tv,NULL) ) {
        perror("nanosleep");
    }
}


