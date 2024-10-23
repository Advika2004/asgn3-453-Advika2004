#include "dine.h"

#define COLLUMN_WIDTH 13
char label_start = 'A';

void print_borders(){
    //for the outer loop so that it can print enough |
    int i;

    //for the inner loop so it can print enough =
    int j;

    int total = COLLUMN_WIDTH;

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
    label_start = 'A';  // Reset label start to 'A' each time
    int i, j;
    int left_padding = (COLLUMN_WIDTH / 2);
    int right_padding = (COLLUMN_WIDTH - 1) / 2;

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
    int philo, fork, sem_value;

    // Loop through each philosopher to print forks and status together
    for (philo = 0; philo < NUM_PHILOSOPHERS; philo++) {

        printf("| ");

        // Loop through forks for each philosopher
        for (fork = 0; fork < NUM_PHILOSOPHERS; fork++) {
            int leftFork = philo;
            int rightFork = (philo + 1) % NUM_PHILOSOPHERS;

            sem_getvalue(&forks[fork], &sem_value);

            if (fork == leftFork && sem_value == 0) {
                printf("%d", fork);  // Left fork held
            } else if (fork == rightFork && sem_value == 0) {
                printf("%d", fork);  // Right fork held
            } else {
                printf("-");
            }
        }

        // Print space between forks and philosopher status
        printf(" ");


        // Print the philosopher's current state
        if (philosopher_state[philo] == 'E') {
            printf("Eat   ");
        } else if (philosopher_state[philo] == 'T') {
            printf("Think ");
        } else {
            printf("      ");  // No state
        }

        // Extra space for even number of philosophers
        if ((NUM_PHILOSOPHERS % 2) == 0) {
            printf(" ");
        }
    }
    // End the line for the final column
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

// void print_borders(){
//     //for the outer loop so that it can print enough |
//     int i;

//     //for the inner loop so it can print enough =
//     int j;

//     int total = COLLUMN_WIDTH;

//     //for each philosopher, go through and print that many =
//     for (i = 0; i < NUM_PHILOSOPHERS; i++) {
//         printf("|");
//         for (j = 0; j < total; j++) {
//             printf("=");
//         }
//     }
//     //print the last line
//     printf("|\n");
// }

// void print_label(){
//     //for the outer loop so that it can print enough |
//     int i;

//     //for the inner loop so it can print enough " "
//     int j;

//     int left_padding = COLLUMN_WIDTH / 2;
//     int right_padding = (COLLUMN_WIDTH - 1) / 2;

//     //for each philosopher, go through and print that many =
//     for (i = 0; i < NUM_PHILOSOPHERS; i++) {
//         printf("|");
//         for (j = 0; j < left_padding; j++) {
//             printf(" ");
//         }
//         printf("%c", label_start);
//         label_start += 1;
        
//         for (j = 0; j < right_padding; j++) {
//             printf(" ");
//         }
//     }

//     //print the last line
//     printf("|\n");
// }

// void print_forks_and_status() {
//     int philo, fork, sem_value;

//     // Loop through each philosopher to print forks and status together
//     for (philo = 0; philo < NUM_PHILOSOPHERS; philo++) {
//         printf("| ");

//         // Loop through forks for each philosopher
//         for (fork = 0; fork < NUM_PHILOSOPHERS; fork++) {
//             int leftFork = philo;
//             int rightFork = (philo + 1) % NUM_PHILOSOPHERS;

//             sem_getvalue(&forks[fork], &sem_value);

//             if (fork == leftFork && sem_value == 0) {
//                 printf("%d", fork);  // Left fork held
//             } else if (fork == rightFork && sem_value == 0) {
//                 printf("%d", fork);  // Right fork held
//             } else {
//                 printf("-");
//             }
//         }

//         // Print space between forks and philosopher status
//         printf(" ");

//         // Print the philosopher's current state
//         if (philosopher_state[philo] == 'E') {
//             printf("Eat   ");
//         } else if (philosopher_state[philo] == 'T') {
//             printf("Think ");
//         } else {
//             printf("      ");  // No state
//         }

//         // Extra space for even number of philosophers
//         if ((NUM_PHILOSOPHERS % 2) == 0) {
//             printf(" ");
//         }
//     }

//     // End the line for the final column
//     printf("|\n");
// }


// void print_whole_thing() {

//     int error = sem_wait(&printing_semaphore);
//     if (error == -1) {
//         perror("sem_wait error\n");
//         exit(EXIT_FAILURE);
//     }

//     // Print the top border
//     print_borders();

//     // Print the label for each philosopher (A, B, C, ...)
//     print_label();

//     // Print the middle border
//     print_borders();

//     print_forks_and_status();

//     // Print the bottom border
//     print_borders();

//     // Signal to release the semaphore after printing
//     error = sem_post(&printing_semaphore);
//     if (error == -1) {
//         perror("sem_post\n");
//         exit(EXIT_FAILURE);
//     }
// }

