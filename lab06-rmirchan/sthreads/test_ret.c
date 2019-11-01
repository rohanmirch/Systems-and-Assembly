/* A program for testing the thread runtimes of multiple
 * threads from the sthreads API.
 */

#include <stdio.h>
#include "sthread.h"
#include <stdint.h>


/*! This thread-function loops n times and prints a corresponding
    message n times for a thread of size n (arg = n). */
static void thread_loop(void *arg) {
    int i;
    // Loop arg times and print a message each time.
    for (i = 0; i < (intptr_t)(arg); i++) {
        printf("Thread of life span %d run  #%d\n", (int)(intptr_t)(arg), i+1);
        sthread_yield();
    }
}

/* Creates and runs 4 threads, each with a different life span. */
int main(int argc, char **argv) {
    sthread_create(thread_loop, (void*)1);
    sthread_create(thread_loop, (void*)2);
    sthread_create(thread_loop, (void*)3);
    sthread_create(thread_loop, (void*)4);
    sthread_start();
    return 0;
}
