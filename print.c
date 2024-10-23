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

void print_label(){
    //for the outer loop so that it can print enough |
    int i;

    //for the inner loop so it can print enough " "
    int j;

    int left_padding = COLLUMN_WIDTH / 2;
    int right_padding = (COLLUMN_WIDTH - 1) / 2;

    //for each philosopher, go through and print that many =
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

    //print the last line
    printf("|\n");
}

void print_forks(int phil_id) {

    int sem_value;
    int fork;
    int leftFork = phil_id;
    int rightFork = (phil_id + 1) % NUM_PHILOSOPHERS;

    //! change this to just be an int? idk why this is the way this is

    for (fork = 0; fork < NUM_PHILOSOPHERS; fork++) {
        sem_getvalue(&forks[fork], &sem_value);

        if (fork == leftFork && sem_value == 0) {
            // the left fork is occupied
            printf("%d", fork);
        } else if (fork == rightFork && sem_value == 0) {
            // the right fork is occupied
            printf("%d", fork);
        } else {
            // The fork is not held by this philosopher
            printf("-");
        }
    }

    // Add a space between the forks and the philosopher state
    printf(" ");
}








// void print_philosopher_state(int philo) {
//     /* Print state of the philosopher */
//     if (philInfo[philo].state == EATING) {
//         printf("Eat   ");
//     } else if (philInfo[philo].state == THINKING) {
//         printf("Think ");
//     } else if (philInfo[philo].state == CHANGING) {
//         printf("      "); // Empty space for changing state
//     }
// }

// void print_state() {
//     int philo, error;

//     /* Wait to take print semaphore */
//     error = sem_wait(&printSema);
//     if (error == -1) {
//         perror("sem_wait\n");
//         exit(EXIT_FAILURE);
//     }

//     /* Loop through each philosopher */
//     for (philo = 0; philo < NUM_PHILOSOPHERS; philo++) {
//         printf("| ");
        
//         /* Print fork status for the current philosopher */
//         print_forks(philo);

//         /* Print the state (Eat, Think, etc.) for the current philosopher */
//         print_philosopher_state(philo);

//         /* Format correction if there is an even number of philosophers */
//         if ((NUM_PHILOSOPHERS % EVEN_FACTOR) == EVEN) {
//             printf(" ");
//         }
//     }

//     /* Print the last column */
//     printf("|\n");

//     /* Release the printing semaphore */
//     error = sem_post(&printSema);
//     if (error == -1) {
//         perror("sem_post\n");
//         exit(EXIT_FAILURE);
//     }
// }

void print_whole_thing(){
    print_borders();
    print_label();
    print_borders();
    print_forks(*phil_id);
    print_borders();
}
