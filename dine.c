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

//function will create as many semaphores as there are philosophers
void create_forks() {
    int i;
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }
}

//at the end of the program takes care of deallocating the semaphores
void clean_forks() {
    int i;
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
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
    philosopher_state[id] = 'C';  
    print_the_middle();  

    if (id % 2 == 0) {

        //!printing here
        //printf("philosopher %d tries to pick up right fork\n", id);

        sem_wait(&forks[right_fork]);
        who_got_da_fork[right_fork] = id;

        //!printing here
        //printf("philosopher %d picked up right fork\n", id);

        //!printing here
        //printf("philosopher %d tries to pick up left fork\n", id);

        sem_wait(&forks[left_fork]);
        who_got_da_fork[left_fork] = id;

        //!printing here
        //printf("philosopher %d picked up left fork\n", id);

    } else {
        //!printing
        //printf("philosopher %d tries to pick up left fork\n", id);

        sem_wait(&forks[left_fork]);
        who_got_da_fork[left_fork] = id;

        //!printing here
        //printf("philosopher %d picked up left fork\n", id);

        //!printing
        //printf("philosopher %d tries to pick up right fork\n", id);

        sem_wait(&forks[right_fork]);
        who_got_da_fork[right_fork] = id;



        
        //!printing here
        //printf("philosopher %d picked up right fork\n", id);

    }

    philosopher_state[id] = 'E';  // State to Eating after picking up forks
    print_the_middle();  // Print after picking up forks
}

void putdown_forks(int id, int left_fork, int right_fork) {
    philosopher_state[id] = 'C';  // State is Changing before putting down
    print_the_middle();  // Print the changing state


    if (id % 2 == 0) {
        who_got_da_fork[right_fork] = -1;
        sem_post(&forks[right_fork]);

        //!printing
        //printf("philosopher %d puts down right fork\n", id);

        who_got_da_fork[right_fork] = -1;
        sem_post(&forks[left_fork]);

        //!printing
        //printf("philosopher %d puts down left fork\n", id);

    } else {
        who_got_da_fork[right_fork] = -1;
        sem_post(&forks[left_fork]);

        //!printing
        //printf("philosopher %d puts down left fork\n", id);

        who_got_da_fork[right_fork] = -1;
        sem_post(&forks[right_fork]);

        //!printing
        //printf("philosopher %d puts down right fork\n", id);
    }

    philosopher_state[id] = 'T';  // State to Thinking after putting down forks
    print_the_middle();  // Print after putting down forks

}

//eats
void eat() {

    //!printing here
    //printf("forks picked up for philosopher. NOW EATING\n");

    dawdle();
}

//thinks
void think() {

    //!printing here
    //printf("forks put down by philosopher. NOW THINKING\n");

    dawdle();
}

//calls all of that in right order
void* philosopher(void* phil_id) {
    int id = *(int *)phil_id;

    //!printing here
    //printf("philosopher %d wants to eat\n", id);

    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;
    int i;

    for (i = 0; i < num_cycles; i++) {

        // Pickup forks
        pickup_forks(id, left_fork, right_fork);

        // Eat
        eat();

        // Put down forks
        putdown_forks(id, left_fork, right_fork);

        // Think
        think();

    }
    
    return NULL;
}


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






// void* philosopher(void* phil_id) {

//     int id = *(int *)phil_id;
    
//     int left_fork = id;

//     int right_fork = (id + 1) % NUM_PHILOSOPHERS;

//     int i;
//     for (i = 0; i < num_cycles; i++) {

//     philosopher_state[id] = 'C';

//         if (id % 2 == 0) {

//             sem_wait(&forks[right_fork]);
            
//             sem_wait(&forks[left_fork]);
        
//         }

//         else {
//             sem_wait(&forks[left_fork]);

//             sem_wait(&forks[right_fork]);
//         }

//         //EATING:
//         philosopher_state[id] = 'E';

//         print_the_middle();

//         dawdle();


//         //THINKING:

//         sem_post(&forks[left_fork]);
//         sem_post(&forks[right_fork]);
//         philosopher_state[id] = 'T';

//         print_the_middle();

//         dawdle();
//     }

//     return NULL;
// }

// void* philosopher(void* phil_id) {


//     //!why is phil_id cast to void* then cast back to int and deref?

//     //the id is the id identifying each philosopher. ex: philosopher 1
//     int id = *(int *)phil_id;
    
//     //printf("philosopher %d wants to eat\n", id);

//     //its left fork will be fork 1
//     int left_fork = id;

//     //its right fork will be 2
//     //need modulo its a circle the last philosophers right becomes the first
//     int right_fork = (id + 1) % NUM_PHILOSOPHERS;

//     //need to loop through the number of eat think wait cycles the user want
//     int i;
//     for (i = 0; i < num_cycles; i++) {

//         //upon start the philospher is in the changing state
//     philosopher_state[id] = 'C';

//         //have each philosopher pick up the fork
//         //if the philosopher has an even id, pick up the right first then 
//         //then switch for odd to avoid deadlock 
//         if (id % 2 == 0) {
//             //printf("philosopher %d tries to pick up right fork\n", id);
//             sem_wait(&forks[right_fork]);
//             //printf("philosopher %d picked up right fork\n", id);

//             //printf("philosopher %d tries to pick up left fork\n", id);
//             sem_wait(&forks[left_fork]);
//             //printf("philosopher %d picked up left fork\n", id);
//         }

//         //odd does the opposite
//         else {
//             //printf("philosopher %d tries to pick up left fork\n", id);
//             sem_wait(&forks[left_fork]);
//             //printf("philosopher %d picked up left fork\n", id);

//             //printf("philosopher %d tries to pick up right fork\n", id);
//             sem_wait(&forks[right_fork]);
//             //printf("philosopher %d picked up right fork\n", id);
//         }

//         //if philosopher has both forks, they will eat
//         //EATING:
//         philosopher_state[id] = 'E';
//         //printf("CURRENT PHILOSOPHER WHO'S STATE IS BEING CHANGED: %d\n", i
//         //! is this supposed to be printed here? 
//         print_the_middle();
//         //printf("forks picked up for philosopher %d. NOW EATING\n", id);
//         dawdle();

//         //if philosopher has eaten, then they will think
//         //to think, they have to put down both forks first
//         //THINKING:
//        //printf("philosopher %d puts down left fork\n", id);
//         sem_post(&forks[left_fork]);
//         //printf("philosopher %d puts down right fork\n", id);
//         sem_post(&forks[right_fork]);
//         philosopher_state[id] = 'T';
//         //printf("CURRENT PHILOSOPHER WHO'S STATE IS BEING CHANGED: %d\n", i
//         //! is this supposed to be printed here?
//         print_the_middle();
//         //printf("forks returned for philosopher %d. NOW THINKING\n", id);
//         dawdle();
//     }

//     return NULL;
// }



