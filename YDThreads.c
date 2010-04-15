#include "yd_threads.h"

void mctx_create(mctx_t_p mctx, void (*sf_addr)(), void *sf_arg, void *sk_addr, size_t sk_size) {
    /* fetch current context */
    getcontext(&(mctx->uc));
    /* adjust to new context */
    mctx->uc.uc_link = NULL;
    mctx->uc.uc_stack.ss_sp = sk_addr;
    mctx->uc.uc_stack.ss_size = sk_size;
    mctx->uc.uc_stack.ss_flags = 0;
    /* set new thread id to the new theard */
    mctx->id = id_iterator();
    if (DEBUG) printf("added new thread id: %d\n",mctx->id);
    /* make new context */
    makecontext(&(mctx->uc), sf_addr, 1, sf_arg);
    return;
}

mctx_t_p scheduler (th_container_t_p container) {
    if (DEBUG) printf("%s\n","enter Scheduler function");
    mctx_t_p next_thread = ((mctx_t_p) list_at(container->container, scheduler_index));
    scheduler_index++;
    if (next_thread == NULL) {
        scheduler_index = 0;
        mctx_t_p returned_thread = ((mctx_t_p) list_at(container->container, scheduler_index));
        scheduler_index++;
        if (returned_thread == NULL) {
            if (DEBUG) printf ("Thread list is eampy, must mean that there are no more theards to run.\n");
            if (DEBUG) printf("exit Scheduler function with NULL value\n");
            return NULL;
        }
        if (DEBUG) printf("%s\n","exit Scheduler function");
        return returned_thread;
    }
    else {
        if (DEBUG) printf("returning thread %d exit Scheduler function\n",next_thread->id);
        return next_thread;
    }
}

void manager() {

    if (DEBUG) printf("%s\n","enter manager function");
    if (&container == NULL) {
        printf ("%s\n","ERROR, container reached manager as NULL");
    }
    while (!(list_is_empty(container->container))) {
        mctx_t_p curr_thread_pointer = scheduler(container);
        if (curr_thread_pointer == NULL) {
            if (DEBUG) printf ("no more theards in the list, exiting the tread manager and gracefully terminating run.\n");
            return;
        }
        current_thread = curr_thread_pointer;
        if (DEBUG) printf("swapping manager with current_thread id %d\n",current_thread->id);
        state = ENQ_THREAD;
        MCTX_SAVE(manager_thread);
        if (state == ENQ_THREAD) {
            state = RUN_THREAD;
            if (DEBUG) printf ("resuming thread id: %d\n", current_thread->id);
            MCTX_RESTORE(current_thread);
        }
        else if (state == TERM_THREAD) {
            tID saved_id = current_thread->id;
            if (DEBUG) printf ("THERM_THREAD state received, terminating thread id: %d\n",current_thread->id);
            op_status status = list_remove(container->container,current_thread->id);
            if (status == OP_FAIL) {
                printf ("ERROR fail to remove node at list_remove function\n");
                exit(5);
            }
            else if (status == OP_DONE) {
                container->container = NULL;
                if (DEBUG) printf ("removed thread id: %d and the container is empty!\n",saved_id);
            }
            else {
                scheduler_index--;
                if (DEBUG) printf ("removed thread id: %d\n",saved_id);
            }
        }
    }
}

void thread_manager_init(void* arg) {
    if (DEBUG) printf("%s\n","init manager thread");
    manager_thread = malloc (sizeof(mctx_t));
    current_thread = malloc (sizeof(mctx_t));
    void* manager_stack = malloc (sizeof(char)*MAX_STACK_SIZE);
    mctx_create(manager_thread,&manager,arg,manager_stack,(sizeof(char)*MAX_STACK_SIZE));
    container = malloc (sizeof(th_container_t));
}

int create_thread( void (*sf_addr)(), void *sf_arg) {
    do
    {
        void* new_thread_stack = malloc (sizeof(char)*MAX_STACK_SIZE);
        mctx_t_p new_thread = malloc (sizeof(mctx_t));
        if (!new_thread || !new_thread_stack)
            break;
        mctx_create(new_thread,sf_addr,sf_arg, new_thread_stack,(sizeof(char)*MAX_STACK_SIZE));
        if (container->container == NULL)
            container->container = list_create(new_thread);
        else
            return list_add_last(container->container, new_thread);
    } while(False);
    return -1;
}

void threads_start() {
    if (DEBUG) printf ("resuming manager thread\n");
    MCTX_RESTORE(manager_thread);
}

void thread_yield(int pInfo, int statInfo) {
    state = ENQ_THREAD;
    MCTX_SAVE(current_thread);
    if (state == ENQ_THREAD) {
        state = RUN_THREAD;
        if (DEBUG) printf ("resuming manager thread\n");
        MCTX_RESTORE(manager_thread);
    }
    state = RUN_THREAD;
}

void thread_term() {
    state = TERM_THREAD;
    MCTX_RESTORE(manager_thread);
}

tID id_iterator() {
    return (next_id++);
}

tID current_thread_id() {
    return current_thread->id;
}
