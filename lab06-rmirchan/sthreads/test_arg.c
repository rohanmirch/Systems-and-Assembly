/* A program for testing the function inputs for
 * the sthreads API. Creates two threads and checks if they both
 * recieve the correct inputs.
 */

#include <stdio.h>
#include "sthread.h"
#include <stdint.h>


/*! This thread-function prints a success message if it recieves 1
    as its argument. Thread runs for 100 iterations. */
static void thread_func(void *arg) {
    int i = 1;
    while(i < 100) {
          i++;
          if ((intptr_t)(arg) == 1) {
              printf("Thread 1 successfully received argument.\n");
              sthread_yield();
          } else {
              fprintf(stderr, "Argument not recieved.\n");
          }
     }
}

/*! This thread-function prints a success message if it recieves 2
    as its argument. Thread runs for 100 iterations.*/
static void thread_func_two(void *arg) {
    int i = 0;
    while(i < 100) {
          i++;
          if ((intptr_t)(arg) == 2) {
              printf("Thread 2 successfully received argument.\n");
              sthread_yield();
          } else {
              fprintf(stderr, "Argument not recieved.\n");
          }
     }
}

/* Creates two threads that run for 100 iterations each and print
 * out success messages if they recieve the correct arguments.
 */
int main(int argc, char **argv) {
    sthread_create(thread_func, (void*)1);
    sthread_create(thread_func_two, (void*)(2));
    sthread_start();
    return 0;
}
