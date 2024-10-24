#include "dine.h"

#define COLLUMN_WIDTH 8
char label_start = 'A';

void print_borders(){
    //for the outer loop so that it can print enough |
    int i;

    //for the inner loop so it can print enough =
    int j;

    int total = COLLUMN_WIDTH + NUM_PHILOSOPHERS;

    if (total % 2 == 0){
        total += 1;
    }

    //for each philosopher, go through and print that many =
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        printf("|");
        for (j = 0; j < total; j++) {
            printf("=");
        }
    }
    //print the last line
    printf("|\n");
}

void print_label() {  
    int i, j;

    int total = COLLUMN_WIDTH + NUM_PHILOSOPHERS;

    if (total % 2 == 0){
        total += 1;
    }

    int left_padding = ((total) / 2);
    int right_padding = ((total) - 1) / 2;

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        printf("|");
        for (j = 0; j < left_padding; j++) {
            printf(" ");
        }
        printf("%c", label_start);
        label_start += 1;
        for (j = 0; j < right_padding; j++) {
            printf(" ");
        }
    }
    printf("|\n");
}


void print_forks_and_status() {
    int philo, fork, fork_value;

    // Loop through each philosopher to print forks and status together
    for (philo = 0; philo < NUM_PHILOSOPHERS; philo++) {

        printf("| ");

        for (fork = 0; fork < NUM_PHILOSOPHERS; fork++){

            sem_getvalue(&forks[fork], &fork_value);

            //if the left fork is being used, print the left fork
            if ((who_got_da_fork[fork] == philo) && fork_value == 0) {
                printf("%d", fork);
            } 
            else {
                printf("-");
            }
            
        }
        

        //print space between forks and status
        printf(" ");

        int left_fork = philo;
        int right_fork = (philo + 1) % NUM_PHILOSOPHERS;

        //now move to printing the state
        if ((philosopher_state[philo] == 'E') 
            && (who_got_da_fork[left_fork] == philo) &&
            (who_got_da_fork[right_fork] == philo)) {
            printf("Eat   ");
        }
        else if ((philosopher_state[philo] == 'T') 
            && (who_got_da_fork[left_fork] == -1) &&
            (who_got_da_fork[right_fork] == -1)) {
            printf("Think ");
        }
        else {
            printf("      "); 
        }

        //extra space for even number of philosophers
        if ((NUM_PHILOSOPHERS % 2) == 0) {
            printf(" ");
        }
    }
    //end the line
    printf("|\n");
}


void print_the_middle() {
    int error = sem_wait(&printing_semaphore);
    if (error == -1) {
        perror("sem_wait error\n");
        exit(EXIT_FAILURE);
    }

    // Print the forks and state of each philosopher
    print_forks_and_status();

    error = sem_post(&printing_semaphore);
    if (error == -1) {
        perror("sem_post\n");
        exit(EXIT_FAILURE);
    }
}

void print_the_top() {
    print_borders();
    print_label();
    print_borders();
}

void print_the_bottom() {
    print_borders();
}
