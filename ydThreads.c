#include "ydThreads.h"
#include "globalDefs.h"

tID mctx_create(mctx_t_p const mctx, void (*sf_addr)(), const void *sf_arg, void *sk_addr, const size_t sk_size, ucontext_t* ret_func,int arg_count) {
    /* fetch current context */
    getcontext(&(mctx->uc));
    /* adjust to new context */
    mctx->uc.uc_link = ret_func;
    mctx->uc.uc_stack.ss_sp = sk_addr;
    mctx->uc.uc_stack.ss_size = sk_size;
    mctx->uc.uc_stack.ss_flags = 0;
    /* set new thread id to the new theard */
    mctx->id = id_iterator();
    ASSERT_PRINT("added new thread id: %d\n", mctx->id);
    /* make new context */
    makecontext(&(mctx->uc), sf_addr, arg_count,sf_arg);
    return mctx->id;
}

mctx_t_p scheduler(run_t runType) {
    switch (runType) {

        case PB:
            return scheduler_pb();
            break;
        case YD:
            return scheduler_yd();
            break;
        case RR:
        default:
            return scheduler_rr();
            break;

    }
}

mctx_t_p scheduler_rr() {
    ASSERT_PRINT("%s\n", "enter Scheduler function");
    mctx_t_p next_thread = ((mctx_t_p) list_at(container->container, scheduler_index));
    scheduler_index++;
    if (next_thread == NULL) {
        scheduler_index = 0;
        mctx_t_p returned_thread = ((mctx_t_p) list_at(container->container, scheduler_index));
        scheduler_index++;
        if (returned_thread == NULL) {
            ASSERT_PRINT("Thread list is empty, must mean that there are no more theards to run.\n");
            ASSERT_PRINT("exit Scheduler function with NULL value\n");
            return NULL;
        }
        ASSERT_PRINT("%s\n", "exit Scheduler function");
        return returned_thread;
    } else {
        ASSERT_PRINT("returning thread %d exit Scheduler function\n", next_thread->id);
        return next_thread;
    }
}

mctx_t_p scheduler_pb() {

}

mctx_t_p scheduler_yd() {

}

void manager() {
    ASSERT_PRINT("%s\n", "enter manager function");
    if (&container == NULL) {
        printf("%s\n", "ERROR, container reached manager as NULL");
        exit(-1);
    }
    while (!(list_is_empty(container->container))) {
        mctx_t_p curr_thread_pointer = scheduler(RR);
        if (curr_thread_pointer == NULL) {
            ASSERT_PRINT("no more theards in the list, exiting the tread manager and gracefully terminating run.\n");
            return;
        }
        current_thread = curr_thread_pointer;
        ASSERT_PRINT("swapping manager with current_thread id %d\n", current_thread->id - 1);
        state = ENQ_THREAD;
        MCTX_SAVE(manager_thread);
        if (state == ENQ_THREAD) {
            state = RUN_THREAD;
            ASSERT_PRINT("resuming thread id: %d\n", current_thread->id - 1);
            threads_stats_t_p stats;
            stats = get_thread_stats_byID(current_thread->id);
            ASSERT(stats);
            if (stats->curr_switch_wait > stats->max_switch_wait)
                stats->max_switch_wait = stats->curr_switch_wait;
            stats->curr_switch_wait = 0;
            MCTX_RESTORE(current_thread);
        } else if (state == TERM_THREAD) {
            tID saved_id = current_thread->id;
            ASSERT_PRINT("THERM_THREAD state received, terminating thread id: %d\n", current_thread->id - 1);
            op_status status = list_remove_thread(container->container, current_thread->id);
            //free_thread(current_thread);
            if (status == OP_FAIL) {
                printf("ERROR fail to remove node at list_remove function\n");
                exit(5);
            } else if (status == OP_DONE) {
                //free(container->container);
                container->container = NULL;
                ASSERT_PRINT("removed thread id: %d and the container is empty!\n", saved_id - 1);
            } else {
                scheduler_index--;
                ASSERT_PRINT("removed thread id: %d\n", saved_id);
            }
        }
    }
}

void thread_manager_init(void* arg, ucontext_t* ret_thread,int arg_count) {
    ASSERT_PRINT("%s\n", "init manager thread");
    if (!manager_thread && !current_thread) {
        int i = 0;
        manager_thread = malloc(sizeof (mctx_t));
        memset(manager_thread, 0, sizeof (mctx_t));

        current_thread = malloc(sizeof (mctx_t));
        memset(current_thread, 0, sizeof (mctx_t));

        ASSERT(manager_thread && current_thread);
        
        void* manager_stack = calloc(MAX_STACK_SIZE, sizeof (void));
        memset(manager_stack, 0, MAX_STACK_SIZE * sizeof (void));
        ASSERT(manager_stack);

        mctx_create(manager_thread, &manager, arg, manager_stack, (sizeof (char) * MAX_STACK_SIZE), ret_thread,arg_count);

        if (!container) {
            container = malloc(sizeof (th_container_t));
            memset(container, 0, sizeof (th_container_t));
        }
    } else
        ASSERT_PRINT("manager thread already initialized\n");
    ASSERT(container && manager_thread && current_thread);
}

int create_thread(void (*sf_addr)(), void *sf_arg,int arg_count) {
    int threadID = -1;
    do {
        void* new_thread_stack = calloc(MAX_STACK_SIZE, sizeof (void));
        if (!new_thread_stack) //not enough memory
            break;
        mctx_t_p new_thread = malloc(sizeof (mctx_t));

        new_thread->initPriority = 0;
        new_thread->priority = 0;

        if (!new_thread) //error handling
        {
            free(new_thread_stack);
            break;
        }
        threadID = mctx_create(new_thread, sf_addr, sf_arg, new_thread_stack, (sizeof (void) * MAX_STACK_SIZE), NULL,arg_count);
        if (!container)
            container = malloc(sizeof (th_container_t));

        if (container->container == NULL) {
            container->container = list_create(new_thread);
        } else
            list_add_last(container->container, new_thread);
        threads_stats_t_p stats = malloc(sizeof (threads_stats_t));
        ASSERT(stats);
        stats->id = new_thread->id;
        stats->max_switch_wait = 0;
        stats->curr_switch_wait = 0;
        stats->curr_jobs_wait = 0;
        stats->max_jobs_wait = 0;
        if (container->stats == NULL)
            container->stats = list_create(stats);
        else
            list_add_last(container->stats, stats);
    } while (false);
    return threadID;
}

void threads_start() {
    ASSERT_PRINT("resuming manager thread\n");
    MCTX_RESTORE(manager_thread);
}

void threads_start_with_ui(mctx_t_p ui_thread) {
    ASSERT_PRINT("threads_start with ui_thread param\n");
    ASSERT(ui_thread);
    state = ENQ_THREAD;
    MCTX_SAVE(ui_thread);
    if (state == ENQ_THREAD) {
        state = RUN_THREAD;
        ASSERT_PRINT("resuming manager thread  with ui_thread param\n");
        MCTX_RESTORE(manager_thread);
    }
    state = RUN_THREAD;
}

void thread_yield(int pInfo, int statInfo) {
    ASSERT_PRINT("thread %d yielding\n", current_thread_id() - 1);
    increase_switch_wait_for_all_except(current_thread_id()); //TODO:do not increase for dead threads
    increase_jobs_wait_for_all_except(current_thread_id(), statInfo);
    state = ENQ_THREAD;
    MCTX_SAVE(current_thread);
    if (state == ENQ_THREAD) {
        state = RUN_THREAD;
        ASSERT_PRINT("resuming manager thread\n");
        MCTX_RESTORE(manager_thread);
    }
    state = RUN_THREAD;
    threads_stats_t_p stats = get_thread_stats_byID(current_thread->id);
    ASSERT(stats);
    if (stats->curr_jobs_wait > stats->max_jobs_wait)
        stats->max_jobs_wait = stats->curr_jobs_wait;
    stats->curr_jobs_wait = 0;
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

int switches_wait(tID threadID) {
    ASSERT(container && container->stats);
    threads_stats_t_p threadStats = get_thread_stats_byID(threadID);
    if (!threadStats)
        return OP_FAIL;
    if (threadStats)
        return threadStats->max_switch_wait;
    else
        return -1;
}

void get_thread_byID(OUT mctx_t_p result, const IN tID threadID) {
    ASSERT(container && container->container);
    node_t_p node = container->container;
    while (node) {
        if (THREAD_DATA(node)->id == threadID) break;
        node = node->next;
    }
    result = THREAD_DATA(node);
}

threads_stats_t_p get_thread_stats_byID(const IN tID threadID) {
    ASSERT(container && container->stats);
    node_t* node = container->stats;
    while (node) {
        if (THREAD_STATS(node)->id == threadID) break;
        node = node->next;
    }
    if (node)
        return THREAD_STATS(node);
    else
        return NULL;
}

int maximal_switch_wait() {
    ASSERT(container && container->stats);
    node_t* node = container->stats;
    int max = 0;
    while (node) {
        if (THREAD_STATS(node)->max_switch_wait > max) max = THREAD_STATS(node)->max_switch_wait;
        node = node->next;
    }
    return max;
}

float avarage_switch_wait() {
    ASSERT(container && container->stats);
    node_t* node = container->stats;
    int total = 0, count = 0;
    while (node) {
        count++;
        total += THREAD_STATS(node)->max_switch_wait;
        node = node->next;
    }
    if (count != 0)
        return total / count;
    else
        return 0;
}

int total_switch_wait() {
    ASSERT(container && container->stats);
    node_t* node = container->stats;
    int total = 0;
    while (node) {
        total += THREAD_STATS(node)->max_switch_wait;
        node = node->next;
    }
    return total;
}

int jobs_wait(tID threadID) {
    ASSERT(container && container->stats);
    threads_stats_t_p threadStats = get_thread_stats_byID(threadID);
    if (!threadStats)
        return OP_FAIL;
    if (threadStats)
        return threadStats->max_jobs_wait;
    else
        return -1;
}

int maximal_jobs_wait() {
    ASSERT(container && container->stats);
    node_t* node = container->stats;
    int max = 0;
    while (node) {
        if (THREAD_STATS(node)->max_jobs_wait > max) max = THREAD_STATS(node)->max_jobs_wait;
        node = node->next;
    }
    return max;
}

float avarage_jobs_wait() {
    ASSERT(container && container->stats);
    node_t* node = container->stats;
    int total = 0, count = 0;
    while (node) {
        count++;
        total += THREAD_STATS(node)->max_jobs_wait;
        node = node->next;
    }
    if (count != 0) {
        float res = (float) total / (float) count;
        return res;
    } else
        return 0;
}

int total_jobs_wait() {
    ASSERT(container && container->stats);
    node_t* node = container->stats;
    int total = 0;
    while (node) {
        total += THREAD_STATS(node)->max_jobs_wait;
        node = node->next;
    }
    return total;
}

void containerToString(const th_container_t_p const threadContainer) {
    printf("THREAD ID:\t%d\n", THREAD_DATA(threadContainer->container)->id);
    printf("THREAD MaxSwitchWait:\t%d\n", THREAD_STATS(threadContainer->stats)->max_switch_wait);
}

void increase_switch_wait_for_all_except(tID threadID) {
    node_t_p node = container->stats;
    while (node) {
        if (THREAD_STATS(node)->id != threadID)
            THREAD_STATS(node)->curr_switch_wait++;
        node = node->next;
    }
}

void increase_jobs_wait_for_all_except(tID threadID, int amount) {
    node_t_p node = container->stats;
    while (node) {
        if (THREAD_STATS(node)->id != threadID)
            THREAD_STATS(node)->curr_jobs_wait += amount;
        node = node->next;
    }
}

void reset_iterator() {
    next_id = 0;
}

op_status delete_statistics() {
    ASSERT(container);
    node_t_p node = container->stats;
    threads_stats_t_p stats_t_p;
    node_t_p curr_node;
    while (node) {
        stats_t_p = THREAD_STATS(node);
        curr_node = node;
        node = node->next;
        free(stats_t_p);
        free(curr_node);
    }
}
void free_thread(mctx_t_p thread)
{
    ASSERT(thread);
    ASSERT_PRINT("freeing thread:%d\n",thread->id);
    free(thread);
}