/* 
 * File:   yd_threads.h
 * Authors: Danni Friedland & Yaniv Dudu
 *
 * Created on March 26, 2010, 11:32 PM
 */

#ifndef _YDTHREADS_H
#define	_YDTHREADS_H

#include <ucontext.h>
#include "globalDefs.h"
#include "linkedList.h"

typedef enum state_e {
    RUN_THREAD,
    ENQ_THREAD,
    TERM_THREAD,
}state_t;

typedef struct mctx_st {
    ucontext_t uc;
    tID id;
} mctx_t,*mctx_t_p;

typedef struct th_container_s {
    node_t_p container;
} th_container_t,*th_container_t_p;

#define NULL    ((void *)0)

/* save machine context */
#define MCTX_SAVE(mctx) (void)getcontext(&(mctx)->uc)

/* restore machine context */
#define MCTX_RESTORE(mctx) (void)setcontext(&(mctx)->uc); 
        

/* save and restore context */
#define MCTX_SWAP(mctx_out, mctx_in)   \
        do { \
            MCTX_SAVE(mctx_out);       \
            if (state == RUN_THREAD) { \
                MCTX_RESTORE(mctx_in); \
            }                          \
            state = RUN_THREAD;        \
        } while (0)\

/* contain the next numeric id to give the next new thread */
static int next_id = 0;

/* index for the round rubin scheduling method - temporary untill part 2-3 */
static int scheduler_index = 0;

/* create machine context which can later be used to save & restore threads */
void mctx_create(mctx_t_p mctx, void (*sf_addr)(), void *sf_arg, void *sk_addr,size_t sk_size);

/* This function receives as arguments a pointer to the thread’s main function and a pointer to
 * its argument list. The function should allocate the thread and prepare it but not run it. Once
 * the thread’s fields are all set, the thread is inserted into the threads container.
 * The function returns the newly created thread’s id or -1 in case of a failure to create a
 * thread */
int create_thread( void (*sf_addr)(), void *sf_arg);

/* This function saves the current thread’s context and resumes the manager (restores the
 * manager’s context). The argument pInfo is related to the requested priority upon yielding
 * (see part three) and the statInfo is an argument which relates to the user’s application, and
 * allows for statistics gathering (part three) */
void thread_yield(int pInfo, int statInfo);

/* Terminates the thread and transfer’s control to the manager. Note that this is the graceful
 * means to terminate a thread – an alternate means to terminate all threads is by terminating
 * the manager (we assume that the program completes after the manager exits) */
void thread_term();

/* Initializes the manager and the thread container data structure */
void thread_manager_init(void* arg);

/* This function starts the user space threads. All it does is simply let the manager run
 * (restores the manager’s context) */
void threads_start();

/* scheduler is responsable to choose one thread from the container list*/
mctx_t_p scheduler(th_container_t_p container);

/* manager is the main function of the manager_thread
 * all threads are comunicating with this the manager_thread thread 
 * to schedule next thread to run.
 * the manager uses scheduler method to determine that order */
void manager();

/* Returns the ID of the currently running thread */
tID current_thread_id();

/* generate id by using an itarator */
tID id_itarator();

/* global manager and current thread*/
mctx_t_p manager_thread, current_thread;

/* the thread container holds all thread in a list data type*/
th_container_t_p container;

/* Flag indicating that the current_thread has completed. */
static volatile state_t state;

#endif	/* _YD_THREADS_H */

