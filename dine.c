#include "dine.h"
#include "print.h"


//global variable
int num_cycles = 1;


//function will create as many semaphores as there are philosophers
void create_forks() {
    int i;
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        int result1 = sem_init(&forks[i], 0, 1);
        if (result1 != 0) {
            errno = result1;
            perror("failed to initialize forks");
        }
    }
}

//at the end of the program takes care of deallocating the semaphores
void clean_forks() {
    int i;
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        int result2 = sem_destroy(&forks[i]);
        if (result2 != 0) {
            errno = result2;
            perror("failed to destroy forks");
        }
    }
}

//given dawdle function
void dawdle() {
    struct timespec tv;
    int msec = (int)(((double)random() / RAND_MAX) * 1000);
    tv.tv_sec = 0;
    tv.tv_nsec = 1000000 * msec;
    if (-1 == nanosleep(&tv,NULL) ) {
        perror("nanosleep");
    }
}

//picks them up 
void pickup_forks(int id, int left_fork, int right_fork) {

    //start off with all philosophers changing
    philosopher_state[id] = 'C';  
    print_the_middle();  

    //if it is even do right first then left
    //update which philosopher is holding the forks
    if (id % 2 == 0) {

        int result3 = sem_wait(&forks[right_fork]);
        if (result3 != 0) {
            errno = result3;
            perror("failed to pick up right fork");
        }

        who_got_da_fork[right_fork] = id;

        int result4 = sem_wait(&forks[left_fork]);
        if (result4 != 0) {
            errno = result4;
            perror("failed to pick up left fork");
        }

        who_got_da_fork[left_fork] = id;

    } 
    
    //else do the opposite 
    //update which philosopher is holding the forks
    else {

        int result5 = sem_wait(&forks[left_fork]);
        if (result5 != 0) {
            errno = result5;
            perror("failed to pick up left fork");
        }

        who_got_da_fork[left_fork] = id;


        int result6 = sem_wait(&forks[right_fork]);
        if (result6 != 0) {
            errno = result6;
            perror("failed to pick up right fork");
        }

        who_got_da_fork[right_fork] = id;

    }

    //after picking both up, goes into EAT state
    philosopher_state[id] = 'E';  
    print_the_middle();
}

//puts em down
void putdown_forks(int id, int left_fork, int right_fork) {

    //make it in the CHANGE state before it goes to THINK
    philosopher_state[id] = 'C'; 
    print_the_middle();


    //if it is even do right then left
    //update who got da fork by saying no one has that fork anymore
    if (id % 2 == 0) {
        who_got_da_fork[right_fork] = -1;
        int result7 = sem_post(&forks[right_fork]);
        if (result7 != 0) {
            errno = result7;
            perror("failed to put down right fork");
        }

        who_got_da_fork[right_fork] = -1;
        int result8 = sem_post(&forks[left_fork]);
        if (result8 != 0) {
            errno = result8;
            perror("failed to put down left fork");
        }

    } 

    //else do opposite 
    else {
        who_got_da_fork[right_fork] = -1;
        int result9 = sem_post(&forks[left_fork]);
        if (result9 != 0) {
            errno = result9;
            perror("failed to put down left fork");
        }

        who_got_da_fork[right_fork] = -1;
        int result10 = sem_post(&forks[right_fork]);
        if (result10 != 0) {
            errno = result10;
            perror("failed to put down right fork");
        }
    }

    //after both have been put down, goes to THINKING
    philosopher_state[id] = 'T'; 
    print_the_middle();
}

//eats and calls random time
void eat() {
    dawdle();
}

//thinks and calls random time
void think() {
    dawdle();
}

//calls all of that in right order so each philosopher can eat wait and think
void* philosopher(void* phil_id) {
    int id = *(int *)phil_id;
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;
    int i;

    //for as many number of cycles were told
    for (i = 0; i < num_cycles; i++) {

        //pick up the forks
        pickup_forks(id, left_fork, right_fork);

        //then eat
        eat();

        //put down the forks
        putdown_forks(id, left_fork, right_fork);

        //then think
        think();

    }
    
    return NULL;
}


int main(int argc, char* argv[]) {

    //print top of the table
    print_the_top();

    //initialize the semaphore for printing
    int result11 = sem_init(&printing_semaphore, 0, 1);
    if (result11 != 0) {
        errno = result11;
        perror("failed to initialize printing fork");
    }

    //if the user provided the additional optional argument
    //make the string argument back into an int and assign it to the global
    if (argc > 1) {
        num_cycles = atoi(argv[1]); 
    }

    int i;

    if (NUM_PHILOSOPHERS < 2){
        fprintf(stderr, "not enough forks to continue /n");
        exit(-1);
    }

    //creates the forks for the meal
    create_forks();

    //create an ID for each philosopher
    //also create the philosophers
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        phil_id[i] = i;
        int status = pthread_create(&philosophers[i], 
                     NULL, philosopher, (void*)&phil_id[i]);

        if (status != 0) {
            errno = status;
            perror("error creating philosopher thread");
            exit(EXIT_FAILURE);
        }
    }

    //wait for the threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        int result12 = pthread_join(philosophers[i], NULL);
        if (result12 != 0) {
            errno = result12;
            perror("failed to join philosopher thread");
        }
    }

    //cleanup semaphores when done
    clean_forks(); 

    print_the_bottom();

    return 0;
}