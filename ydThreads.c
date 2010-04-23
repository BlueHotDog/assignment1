#include "ydThreads.h"
#include "globalDefs.h"
#include "jobs.h"



//void mctx_create(mctx_t *mctx, void (*sf_addr)( ), void *sf_arg, void *sk_addr,size_t sk_size)
tID mctx_create(mctx_t_p const mctx, void (*sf_addr)(), const void *sf_arg, void *sk_addr, const size_t sk_size) {
    /* fetch current context */
    getcontext(&(mctx->uc));
    /* adjust to new context */
    mctx->uc.uc_link = NULL;
    mctx->uc.uc_stack.ss_sp = sk_addr;
    mctx->uc.uc_stack.ss_size = sk_size;
    mctx->uc.uc_stack.ss_flags = 0;
    /* set new thread id to the new theard */
    mctx->id = id_iterator();
    ASSERT_PRINT("added new thread id: %d\n", mctx->id);
    /* make new context */
    makecontext(&(mctx->uc), sf_addr, 1, sf_arg);
    return mctx->id;
}

//void mctx_create(mctx_t *mctx, void (*sf_addr)( ), void *sf_arg, void *sk_addr,size_t sk_size)
tID mctx_create_manager(mctx_t_p const mctx, void (*sf_addr)(), const void *sf_arg, void *sk_addr, const size_t sk_size, ucontext_t* ret_func, int arg_count) {
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
    makecontext(&(mctx->uc), sf_addr, arg_count, sf_arg);
    return mctx->id;
}

mctx_t_p scheduler() {
    switch (*runType) {
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
    node_t_p node = ((node_t_p) list_at(container->container, scheduler_index));
    mctx_t_p next_thread = NULL;
    scheduler_index++;
    if (node == NULL) {
        scheduler_index = 0;
        mctx_t_p returned_thread = ((mctx_t_p) list_at(container->container, scheduler_index)->data);
        scheduler_index++;
        if (returned_thread == NULL) {
            ASSERT_PRINT("Thread list is empty, must mean that there are no more theards to run.\n");
            ASSERT_PRINT("exit Scheduler function with NULL value\n");
            return NULL;
        }
        ASSERT_PRINT("%s\n", "exit Scheduler function");
        return returned_thread;
    } else {
        next_thread = (mctx_t_p) node->data;
        ASSERT_PRINT("returning thread %d exit Scheduler function\n", next_thread->id);
        return next_thread;
    }
}

mctx_t_p scheduler_pb() {
    mctx_t_p thread_to_return;
    int offset;
    if (container->lastRunThreadID == NULL_TID) {
        offset = 0;
    } else {
        if(state == TERM_THREAD)
            offset = container->indexOfLastThreadInContainer;
        else
            offset = container->indexOfLastThreadInContainer +1;
        ASSERT_PRINT("offset is: %d\n", offset);
        ASSERT(offset >= 0);
    }
    thread_to_return = search_for_highest_priority_thread(offset);
    ASSERT(thread_to_return);
    container->lastRunThreadID = thread_to_return->id;
    container->indexOfLastThreadInContainer = thread_index_in_list(thread_to_return->id);
    return thread_to_return;
}

mctx_t_p scheduler_yd() {
    mctx_t_p thread_to_return;
    if(container->lastRunThreadID == NULL_TID){
        node_t_p node = list_at(container->container, 0);
        thread_to_return = node->data;
        container->lastRunThreadID = thread_to_return->id;
        container->indexOfLastThreadInContainer = thread_index_in_list(container->lastRunThreadID);
        return thread_to_return;
    }

    if(jobs[container->lastRunThreadID -1] == Done) { //current_thread execute its job so choosing one of its child
        tID* children = find_all_children(container->lastRunThreadID);
        thread_to_return = find_highest_priority_thread_by_id(children);
        if(thread_to_return != NULL_THREAD) {
            container->lastRunThreadID = thread_to_return->id;
            container->indexOfLastThreadInContainer = thread_index_in_list(container->lastRunThreadID);
            return thread_to_return;
        }
        else {  // thread_to_return is NULL_THREAD i.e: there this thread don't have parents.
            thread_to_return = get_by_rr_with_offset_next_thread(container->lastRunThreadID);
            container->lastRunThreadID = thread_to_return->id;
            container->indexOfLastThreadInContainer = thread_index_in_list(container->lastRunThreadID);
            return thread_to_return;
        }
    }
    else { //current thread did NOT execute a job - choosing one if its parents.
        tID* parents = find_all_parents(container->lastRunThreadID);
        thread_to_return = find_highest_priority_thread_by_id(parents);
        if (thread_to_return != NULL_THREAD) {
            container->lastRunThreadID = thread_to_return->id;
            container->indexOfLastThreadInContainer = thread_index_in_list(container->lastRunThreadID);
            return thread_to_return;
        }
        else { // thread_to_return is NULL_THREAD i.e: there this thread don't have parents.
            thread_to_return = get_by_rr_with_offset_next_thread(container->lastRunThreadID);
            container->lastRunThreadID = thread_to_return->id;
            container->indexOfLastThreadInContainer = thread_index_in_list(container->lastRunThreadID);
            return thread_to_return;
        }
    }
}


int thread_index_in_list(tID threadID) {
    node_t_p node = container->container;
    int index_result = 0;
    while(node != NULL) {
        mctx_t_p thread = (mctx_t_p) node->data;
        if(thread->id == threadID) {
            return index_result;
        }
        node = node->next;
        index_result++;
    }
    return -1;
}

mctx_t_p get_by_rr_with_offset_next_thread(tID threadID) {
    int offset = thread_index_in_list(threadID);
    node_t_p node = list_at(container->container, (offset +1) % threadsAmount);
    return (mctx_t_p) node->data;
}

tID* find_all_parents(tID threadID) {
    tID* result_array = calloc(threadsAmount +1, sizeof(tID));
    int row;
    int array_index = 0;
    int column = threadID -1;
    for(row=0; row<jobsAmount; row++) {
        if(deps[row][column] && jobs[row] != Done) {
            result_array[array_index] = row +1;
            array_index ++;
        }
    }
    result_array[array_index] = NULL_TID;
    return result_array;
}

tID* find_all_children(tID threadID) {
    tID* result_array = calloc(threadsAmount +1, sizeof(tID));
    int row = threadID -1;
    int array_index = 0;
    int column;
    for(column=0; column<jobsAmount; column++) {
        if(deps[row][column]  && jobs[column] != Done) {
            result_array[array_index] = column +1;
            array_index ++;
        }
    }
    result_array[array_index] = NULL_TID;
    return result_array;
}

mctx_t_p find_highest_priority_thread_by_id(tID* children) {
    int i=0;
    PB_priority max_priority = MIN_PRIORITY-1;
    mctx_t_p thread_to_return = NULL_THREAD;
    for(i; children[i] != NULL_TID; i++) {
        tID threadID = children[i];
        node_t* node = container->container;
        mctx_t_p tested_thread = NULL;
        while (node) {
            tested_thread = (mctx_t_p) node->data;
            if (tested_thread->id == threadID)
                break;
            node = node->next;
        }
/*
        if(!node) {
            int i=0;
            printf("children: ");
            for(i; children[i] != NULL_TID; i++) {
                 printf("%d ", children[i]);
            }
            printf("\nid's in the system: ");
            node_t* node = container->container;
            while (node) {
                tested_thread = (mctx_t_p) node->data;
                printf("%d ", tested_thread->id);
                node = node->next;
            }
            exit (4);
        }
        ASSERT(node != NULL);
*/
        if(tested_thread && jobs[tested_thread->id -1] && tested_thread->priority > max_priority && jobs[tested_thread->id -1]) {
            max_priority = tested_thread->priority;
            thread_to_return = tested_thread;
        }
    }
    return thread_to_return;
}

mctx_t_p search_for_highest_priority_thread(int offset) {
    mctx_t_p highest_priority_thread = NULL_THREAD;
    PB_priority max_priority = MIN_PRIORITY - 1;
    int i = 0;
    if(threadsAmount == 0) {exit(printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"));}
    for (i; i < threadsAmount; i++) {
        node_t_p current_node = list_at(container->container, (i + offset) % threadsAmount);
        ASSERT(current_node);
        PB_priority thread_priority = ((mctx_t_p) (current_node->data))->priority;
        if (thread_priority > max_priority) {
            max_priority = thread_priority;
            highest_priority_thread = (mctx_t_p) current_node->data;
        }
    }
    return highest_priority_thread;
}

void manager() {
    threads_stats_t_p stats;
    ASSERT_PRINT("%s\n", "enter manager function");
    if (&container == NULL) {
        printf("%s\n", "ERROR, container reached manager as NULL");
        exit(-1);
    }
    while (!(list_is_empty(container->container))) {
        mctx_t_p curr_thread_pointer = scheduler();
        if (curr_thread_pointer == NULL) {
            ASSERT_PRINT("no more theards in the list, exiting the tread manager and gracefully terminating run.\n");
            return;
        }
        current_thread = curr_thread_pointer;
        ASSERT_PRINT("swapping manager with current_thread id %d\n", current_thread->id);
        stats = get_thread_stats_byID(current_thread->id);
        ASSERT(stats);
        stats->thread_state = ALIVE_THREAD;
        state = ENQ_THREAD;
        MCTX_SAVE(manager_thread);
        if (state == ENQ_THREAD) {
            state = RUN_THREAD;
            ASSERT_PRINT("resuming thread id: %d\n", current_thread->id);
            MCTX_RESTORE(current_thread);
        } else if (state == TERM_THREAD) {
            tID saved_id = current_thread->id;
            stats = get_thread_stats_byID(current_thread->id);
            ASSERT(stats);
            stats->thread_state = DEAD_THREAD;
            ASSERT_PRINT("THERM_THREAD state received, terminating thread id: %d\n", current_thread->id);
            op_status status = list_remove_thread(container->container, current_thread->id);
            if (status == OP_FAIL) {
                printf("ERROR fail to remove node at list_remove function\n");
                exit(5);
            } else if (status == OP_DONE) {
                container->container = NULL;
                threadsAmount = -1;
                scheduler_index = -1;
                ASSERT_PRINT("removed thread id: %d\n", saved_id);
                printf("All threads terminated.\n");
            } else {
                scheduler_index--;
                threadsAmount--;
                ASSERT_PRINT("removed thread id: %d\n", saved_id);
            }
        }
    }
}

void thread_manager_init(void* arg, ucontext_t* ret_thread, int arg_count) {
    ASSERT_PRINT("%s\n", "init manager thread");
    if (!manager_thread && !current_thread) {
        int i = 0;
        manager_thread = malloc(sizeof (mctx_t));
        memset(manager_thread, 0, sizeof (mctx_t));

        /*
                current_thread = malloc(sizeof (mctx_t));
                memset(current_thread, 0, sizeof (mctx_t));

                ASSERT(manager_thread && current_thread);
         */

        void* manager_stack = calloc(MAX_STACK_SIZE, sizeof (void));
        memset(manager_stack, 0, MAX_STACK_SIZE * sizeof (void));
        ASSERT(manager_stack);

        mctx_create_manager(manager_thread, &manager, arg, manager_stack, (sizeof (char) * MAX_STACK_SIZE), ret_thread, arg_count);

        if (!container) {
            container = malloc(sizeof (th_container_t));
            memset(container, 0, sizeof (th_container_t));
            container->lastRunThreadID = NULL_TID;
            container->indexOfLastThreadInContainer = -1;
        }
    } else
        ASSERT_PRINT("manager thread already initialized\n");
    ASSERT(container && manager_thread);
}

//  int create_thread( void (*sf_addr)(), void *sf_arg,)

int create_thread(void (*sf_addr)(), void *sf_arg) {
    int threadID = -1;
    do {
        void* new_thread_stack = NULL;
        new_thread_stack = calloc(MAX_STACK_SIZE, sizeof (void));

        if (!new_thread_stack) //not enough memory
            break;
        mctx_t_p new_thread = malloc(sizeof (mctx_t));

        if (!new_thread) //error handling
        {
            free(new_thread_stack);
            break;
        }

        threadID = mctx_create(new_thread, sf_addr, sf_arg, new_thread_stack, (sizeof (void) * MAX_STACK_SIZE));
        if (!container)
            container = malloc(sizeof (th_container_t));

        if (container->container == NULL) {
            container->container = list_create(new_thread);
        } else
            list_add_last(container->container, new_thread);

        new_thread->initPriority = PB_array[threadID -1];
        new_thread->priority = PB_array[threadID -1];
        threads_stats_t_p stats = malloc(sizeof (threads_stats_t));
        ASSERT(stats);
        
        stats->id = new_thread->id;
        stats->max_switch_wait = 0;
        stats->curr_switch_wait = 0;
        stats->curr_jobs_wait = 0;
        stats->max_jobs_wait = 0;
        stats->thread_state = UNBORN_THREAD;

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
    state = ENQ_THREAD;
    MCTX_SAVE(ui_thread);
    if (state == ENQ_THREAD) {
        state = RUN_THREAD;
        ASSERT_PRINT("resuming manager thread  with ui_thread param\n");
        MCTX_RESTORE(manager_thread);
    }
    state = RUN_THREAD;
}

void thread_yield(int pInfo, int statInfo, boolean worked) {
    ASSERT_PRINT("thread %d yielding\n", current_thread_id() - 1);
    increase_switch_wait_for_all_except(current_thread_id()); //TODO:do not increase for dead threads
    increase_jobs_wait_for_all_except(current_thread_id(), statInfo);
    threads_stats_t_p stats = get_thread_stats_byID(current_thread->id);
        ASSERT(stats);
        if (stats->curr_switch_wait > stats->max_switch_wait)
            stats->max_switch_wait = stats->curr_switch_wait;
        stats->curr_switch_wait = 0;

    state = ENQ_THREAD;
    if (worked && *runType == PB) {
        current_thread->priority = current_thread->initPriority;
    } else if (current_thread->priority > 0 && (*runType == PB || *runType == YD)) {
        current_thread->priority--;
        if (*runType == YD) {
            tID* children = find_all_children(current_thread->id);
            int i=0;
            for(i; children[i] != NULL_TID; i++) {
                mctx_t_p thread = get_thread_byID(children[i]);
                thread->priority--;
            }
        }
    }
    MCTX_SAVE(current_thread);
    if (state == ENQ_THREAD) {
        state = RUN_THREAD;
        ASSERT_PRINT("resuming manager thread\n");
        MCTX_RESTORE(manager_thread);
    }
    state = RUN_THREAD;
    stats = get_thread_stats_byID(current_thread->id);
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

mctx_t_p get_thread_byID(const IN tID threadID) {
    ASSERT(container && container->container);
    node_t_p node = container->container;
    while (node) {
        if (THREAD_DATA(node)->id == threadID)
            break;
        node = node->next;
    }
    return THREAD_DATA(node);
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
        if (THREAD_STATS(node)->id != threadID && THREAD_STATS(node)->thread_state == ALIVE_THREAD)
            THREAD_STATS(node)->curr_switch_wait++;
        node = node->next;
    }
}

void increase_jobs_wait_for_all_except(tID threadID, int amount) {
    node_t_p node = container->stats;
    while (node) {
        if (THREAD_STATS(node)->id != threadID && THREAD_STATS(node)->thread_state == ALIVE_THREAD)
            THREAD_STATS(node)->curr_jobs_wait += amount;
        node = node->next;
    }
}

void reset_iterator(int num) {
    next_id = num;
}

op_status delete_statistics() {
    ASSERT(container);
    node_t_p node = container->stats;
    threads_stats_t_p stats_t_p;
    node_t_p curr_node;
    while (node) {
        free(node->data);
        node->data = NULL;
        curr_node = node;
        node = node->next;
        free(curr_node);
        curr_node = NULL;
    }
    container->stats = NULL;
}
