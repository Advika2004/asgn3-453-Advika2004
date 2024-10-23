Advika Deodhar

______________________________________________________________________________________
Pseudocode To Begin: 


Overall Task:
- Philosophers have to eat, think, and get full / hungry
- Need to implement 3 different states for each philosopher: eating, thinking, transitioning
- Each Philosopher needs 2 forks to eat, so each thread needs two resources to run
- Each fork, or resource is represented as a semaphore 
- Need to make sure that no philosophers next to each other are eating at the same time
- Need to also make sure that each philosopher picks up their fork to their left and right
- Need to alternate which philosopher picks up which fork first so that there are no deadlocks
- Use a binary semaphore for printing and updating states? 
- Every philosopher starts out hungry and wanting to eat

Steps:
- Need to write the main function
- Need to write the philosophers function
- Function to initialize semaphores 
- Function to destroy semaphores
- Function to print stuff

Dine File:

- Create Forks:
    - for as many philosophers there are, create one fork for each of them
    - num philosophers = num forks because it is a circle even though each person uses two forks

- Clean Forks:
    - at the end need to destroy the semaphores just as I created them

- Main: 
    - if the user provided the extra argument then update the cycles
    - create all the forks needed
    - create all the philosophers needed and check if the creation failed
        - the philosophers will automatically run the philosopher function when created
    - wait for all the threads
    - clean up the forks

- Philosopher:
    - this is where the philosopher actually picks up the forks, waits, thinks, and eats. 

Printing File: 

- print_border:
    -  prints out the top middle and bottom borders

- print_label: 
    - print the letters for each label 

- print_status:
    - prints if it is EAT, THINK or nun

- print_forks:
    - will print the forks held by each philosopher

- print_table:
    - first will create the semaphore lock so that only one thread prints at a time
    - will print_border for the top
    - will print_label for the label
    - will print_border for the middle border
    - will print_status for each philosopher by looping through
    - will print_forks for each philosopher by looping through
    - will print_border to end the table
    - post to the semaphore



ERROR CHECKS: 
 - Check liteally all pthreads
 - Check all semaphore creation, and all actions for that in general 
