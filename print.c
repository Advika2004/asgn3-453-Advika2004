#include "dine.h"
#include "print.h"

//what the labels should start at
char label_start = 'A';

//will print the top and bottom and middle border
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
    int i; 
    int j;

    int total = COLLUMN_WIDTH + NUM_PHILOSOPHERS;

    //make sure if its even that it lines up properly
    if (total % 2 == 0){
        total += 1;
    }

    //make sure the letter will be centered
    int left_padding = ((total) / 2);
    int right_padding = ((total) - 1) / 2;

    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        printf("|");

        for (j = 0; j < left_padding; j++) {
            printf(" ");
        }

        printf("%c", label_start);

        //go to the next ascii 
        label_start += 1;

        for (j = 0; j < right_padding; j++) {
            printf(" ");
        }
    }

    printf("|\n");
}


void print_forks_and_status() {

    int phil;
    int fork;
    int fork_value;

    //for every  philosopher
    for (phil = 0; phil < NUM_PHILOSOPHERS; phil++) {

        printf("| ");

        //then go through every fork for each philosopher
        for (fork = 0; fork < NUM_PHILOSOPHERS; fork++){

            //check if the fork is being used or not
            int result1 = sem_getvalue(&forks[fork], &fork_value);
            if (result1 != 0) {
                errno = result1;
                perror("failed to get fork value");
            }

            //if the left fork is being used, print the left fork
            if ((who_got_da_fork[fork] == phil) && fork_value == 0) {
                printf("%d", fork);
            } 
            else {
                printf("-");
            }
            
        }
        
        //print space between forks and status
        printf(" ");

        //define the left and the right
        int left_fork = phil;
        int right_fork = (phil + 1) % NUM_PHILOSOPHERS;

        //now move to printing the state
        if ((philosopher_state[phil] == 'E') 
            && (who_got_da_fork[left_fork] == phil) &&
            (who_got_da_fork[right_fork] == phil)) {
            printf("Eat   ");
        }
        else if ((philosopher_state[phil] == 'T') 
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

//prints the juicy meat
void print_the_middle() {
    int result2 = sem_wait(&printing_semaphore);
    if (result2 != 0) {
        perror("failed to wait for printing semaphore\n");
        exit(EXIT_FAILURE);
    }

    //print the forks and state of each philosopher
    print_forks_and_status();

    result2 = sem_post(&printing_semaphore);
    if (result2 != 0) {
        perror("failt to post printing semaphore\n");
        exit(EXIT_FAILURE);
    }
}

//print the top of the chart
void print_the_top() {
    print_borders();
    print_label();
    print_borders();
}

//print the bottom of the chart
void print_the_bottom() {
    print_borders();
}
