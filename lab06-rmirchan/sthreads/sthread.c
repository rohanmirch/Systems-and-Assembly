/*
 * The simple thread scheduler.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sthread.h"
#include "queue.h"


/*! The default stack size of threads, 1MiB of stack space. */
#define DEFAULT_STACKSIZE       (1 << 20)


/************************************************************************
 * Interface to the assembly.
 */


/*! This function must be called to start thread processing. */
void __sthread_start(void);


/*!
 * The is the entry point into the scheduler, to be called
 * whenever a thread action is required.
 */
void __sthread_switch(void);

/*!
 * Initialize the context for a new thread.
 *
 * The stackp pointer should point to the *end* of the area allocated for the
 * thread's stack.  (Don't forget that x86 stacks grow downward in memory.)
 *
 * The function f is initially called with the argument given.  When f
 * returns, __sthread_finish() is automatically called by the threading
 * library to ensure that the thread is cleaned up and deallocated properly.
 */
ThreadContext *__sthread_initialize_context(void *stackp, ThreadFunction f,
                                            void *arg);

/************************************************************************
 * Internal helper functions.
 */

/*
 * The initial thread context is initialized such that this function
 * will be called automatically when a thread's function returns.
 */
void __sthread_finish(void);

/*
 * This function deallocates the memory associated with a thread
 * when it terminates.
 */
void __sthread_delete(Thread *threadp);


/************************************************************************
 * Global variables and scheduler-level queue operations.
 */

/*!
 * The thread that is currently running.
 *
 * Invariant: during normal operation, there is exactly one thread in
 * the ThreadRunning state, and this variable points to that thread.
 *
 * Note that at start-up of the threading library, this will be NULL.
 */
static Thread *current;

/*!
 * The queue of ready threads.  Invariant:  All threads in the ready queue
 * are in the state ThreadReady.
 */
static Queue ready_queue;

/*!
 * The queue of blocked threads.  Invariant:  All threads in the blocked
 * queue are in the state ThreadBlocked.
 */
static Queue blocked_queue;


/*!
 * Add a thread to the appropriate scheduling queue, based on its state.
 */
static void enqueue_thread(Thread *threadp) {
    assert(threadp != NULL);

    switch(threadp->state) {
    case ThreadReady:
        queue_append(&ready_queue, threadp);
        break;
    case ThreadBlocked:
        queue_append(&blocked_queue, threadp);
        break;
    default:
        fprintf(stderr, "Thread state has been corrupted: %d\n",
                threadp->state);
        exit(1);
    }
}

/************************************************************************
 * Scheduler.
 */

/*
 * The scheduler is called with the context of the current thread,
 * or NULL when the scheduler is first started.
 *
 * The general operation of this function is:
 *   1.  Save the context argument into the current thread.
 *   2.  Either queue up or deallocate the current thread,
 *       based on its state.
 *   3.  Select a new "ready" thread to run, and set the "current"
 *       variable to that thread.
 *        - If no "ready" thread is available, examine the system
 *          state to handle this situation properly.
 *   4.  Return the context of the thread to run, so that a context-
 *       switch will be performed to start running the next thread.
 *
 * This function is global because it needs to be called from the assembly.
 */
ThreadContext *__sthread_scheduler(ThreadContext *context) {

    /* First call to scheduler returns NULL. Also, if the ready
       queue is empty or only has one thread, then the same
       thread should just continue running. */
    if (context == NULL) {
        current = queue_take(&ready_queue);
        current->state = ThreadRunning;
        return current->context;
    }

    /* Set the current thread's context to the given context. */
    current->context = context;

    /* Properly handle the current running thread's state. */
    if (current->state == ThreadRunning || current->state == ThreadBlocked) {
        if (current->state != ThreadBlocked) {
            current->state = ThreadReady;
        }
        /* Enqueue thread to appropriate queue. */
        enqueue_thread(current);
    }
    else if (current->state == ThreadFinished) {
        /* Delete finished threads. */
        __sthread_delete(current);
    }
    else {
        printf("Current thread is running. \n");
    }

    /* Select next thread from ready queue for excecution. */
    current = queue_take(&ready_queue);

    if (current == NULL && queue_empty(&blocked_queue)) {
        /* The blocked queue is empty and we are done all threads. */
        fprintf(stderr, "All Threads excecuted.\n");
        exit(0);
    }
    else if (current == NULL && !queue_empty(&blocked_queue)) {
        /* We've reached a thread deadlock if they are all blocked. */
        fprintf(stderr, "Thread deadlock. \n");
        exit(1);
    }
    else {
        /* Start running next thread. */
        current->state = ThreadRunning;
    }

    return current->context;

}

/************************************************************************
 * Thread operations.
 */

/*
 * Start the scheduler.
 */
void sthread_start(void)
{
    __sthread_start();
}

/*
 * Create a new thread.
 *
 * This function allocates a new context, and a new Thread
 * structure, and it adds the thread to the Ready queue.
 */
Thread * sthread_create(void (*f)(void *arg), void *arg) {
    /* Allocate memory for the thread's stack. */
    void * stack = malloc(DEFAULT_STACKSIZE);
    if (stack == NULL) {
        fprintf(stderr, "Could not allocate stack space.\n");
        exit(1);
    }

    // Allocate memory for the actual thread structure
    Thread * threadp = malloc(sizeof(Thread));
    if (threadp == NULL) {
        fprintf(stderr, "Could not allocate thread space.\n");
        exit(1);
    } else {
        // Initialize state
        threadp->state = ThreadReady;
        // Initialize memory (pointer the start of stack memory region)
        threadp->memory = stack;
        // Initalize context
        void * stack_start = (char*)stack + DEFAULT_STACKSIZE;
        threadp->context = __sthread_initialize_context(stack_start, f, arg);
        if (threadp->context == NULL) {
            fprintf(stderr,"Could not allocate thread space.\n");
        }
        // Place new thread on ready_queue
        queue_append(&ready_queue, threadp);

   }
    return threadp;

}


/*
 * This function is called automatically when a thread's function returns,
 * so that the thread can be marked as "finished" and can then be reclaimed.
 * The scheduler will call __sthread_delete() on the thread before scheduling
 * a new thread for execution.
 *
 * This function is global because it needs to be referenced from assembly.
 */
void __sthread_finish(void) {
    printf("Thread %p has finished executing.\n", (void *) current);
    current->state = ThreadFinished;
    __sthread_switch();
}


/*!
 * This function is used by the scheduler to release the memory used by the
 * specified thread.  The function deletes the memory used for the thread's
 * context, as well as the memory for the Thread struct.
 */
void __sthread_delete(Thread *threadp) {
    /* Free the thread's stack and the thread sturcture itself.  */
    free(threadp->memory);
    free(threadp);
}


/*!
 * Yield, so that another thread can run.  This is easy: just
 * call the scheduler, and it will pick a new thread to run.
 */
void sthread_yield() {
    __sthread_switch();
}


/*!
 * Block the current thread.  Set the state of the current thread
 * to Blocked, and call the scheduler.
 */
void sthread_block() {
    current->state = ThreadBlocked;
    __sthread_switch();
}


/*!
 * Unblock a thread that is blocked.  The thread is placed on
 * the ready queue.
 */
void sthread_unblock(Thread *threadp) {

    /* Make sure the thread was blocked */
    assert(threadp->state == ThreadBlocked);

    /* Remove from the blocked queue */
    queue_remove(&blocked_queue, threadp);

    /* Re-queue it */
    threadp->state = ThreadReady;
    enqueue_thread(threadp);
}
