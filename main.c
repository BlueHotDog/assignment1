/* 
 * File:   main.c
 * Authors: Danni Friedland & Yaniv Dudu
 *
 * Created on March 26, 2010, 4:40 PM
 */

#include "ydThreads.h"
#include "fileHandler.h"
#include "jobs.h"
static int read_from_file_thread_amount = -1;

void a() {
    int i = 0;
    for (i = 0; i < 2; i++) {
        printf("Thread %d\n", current_thread->id);
        thread_yield(0, 0, false);
    }
    thread_term();
}

void b() {

    int i = 0;
    for (i = 0; i < 1; i++) {
        printf("Thread %d\n", current_thread->id);
        thread_yield(0, 0, false);
    }
    thread_term();
}

void c() {

    int i = 0;
    for (i = 0; i < 1; i++) {
        printf("Thread %d\n", current_thread->id);
        thread_yield(0, 0, false);
    }
    thread_term();
}

void runThread() {
    int jobsDone = 0;
    node_t_p node = container->stats;
    while (node) {
        if (THREAD_STATS(node)->id == current_thread_id()) {
            THREAD_STATS(node)->curr_jobs_wait = 0;
            THREAD_STATS(node)->max_jobs_wait = 0;
            break;
        }
        node = node->next;
    }
    while (hasPendingJobs(current_thread->id - 1)) {
        tID jobId = getJobForThread(current_thread->id - 1);
        if (jobId != NULL_TID) {
            execJob(jobId);
            jobsDone++;
            thread_yield(0, jobsDone, true);
        } else {
            ASSERT_PRINT("Thread %d will wait because there is no free job for him to execute\n", current_thread->id - 1);
            thread_yield(0, jobsDone, false);
        }
        jobsDone = 0;
    }
    printf("Thread %d performed all jobs\n", current_thread->id);
    thread_term();
}
mctx_t_p ui_thread;

void create_ui_thread(void* ui_func) {
    void* new_thread_stack = calloc(MAX_STACK_SIZE, sizeof (void));
    memset(new_thread_stack, 0, MAX_STACK_SIZE * sizeof (void));
    mctx_t_p new_thread = malloc(sizeof (mctx_t));
    memset(new_thread, 0, sizeof (mctx_t));
    // new_thread->threadStack = new_thread_stack;
    mctx_create(new_thread, ui_func, NULL, new_thread_stack, (sizeof (char) * MAX_STACK_SIZE), NULL, 0);
    next_id--;
    ui_thread = new_thread;
    return;
}

void clear_ui_thread() {
    ASSERT(ui_thread);
    free(ui_thread->uc.uc_stack.ss_sp);
    free(ui_thread);
    ui_thread = NULL;

}

void clear_container() {
    ASSERT(container);
    free(container);
    container = NULL;
}

void clear_manager_thread() {
    ASSERT(manager_thread);
    free(manager_thread->uc.uc_stack.ss_sp);
    free(manager_thread);
    manager_thread = NULL;
}

void free_file_info() {
    int i = 0, j = 0;
    for (i = 0; i < jobsAmount; i++)
        free(deps[i]);
    free(deps);
    ASSERT(jobs);
    free(jobs);
    for (i = 0; i < read_from_file_thread_amount; i++) {
        free(jobsForThreads[i].jobs);
    }
    free(jobsForThreads);
}

void free_pb() {
    free(PB_array);
}

void free_memory() {
    delete_statistics();
    clear_ui_thread();
    clear_manager_thread();
    list_clear_all_threads(container->container);
    free_file_info();
    clear_container();
    free_pb();
}

void init_jobs_array() {
    int i = 0;
    for (i = 0; i < jobsAmount; i++)
        (jobs)[i] = NotStarted;
}

void ui() {
    string command = malloc(MAX_INPUT_LENGTH);
    string parameter = malloc(MAX_INPUT_LENGTH);
    memset(command, 0, MAX_INPUT_LENGTH);
    memset(parameter, 0, MAX_INPUT_LENGTH);

    readFile("/home/danni/test", &deps, &jobs, &jobsForThreads, &threadsAmount, &jobsAmount);
    read_from_file_thread_amount = threadsAmount;

    ASSERT_RUN(printData());

    while (strcmp(command, "exit") != 0) {
        printf(">");
        scanf("%s", command);
        if (strcmp(command, "load") == 0 || strcmp(command, "SW") == 0 || strcmp(command, "JW") == 0) {
            scanf("%s", parameter);
            if (strcmp(command, "SW") == 0) { //=============================SW==================================
                int threadNumber = 0;
                sscanf(parameter, "%d", &threadNumber); //converting parameter
                int waitAmount = switches_wait(threadNumber);
                if (waitAmount >= 0)
                    printf("%d\n", waitAmount);
                else
                    printf("Unable to find thread\n");
            } else if (strcmp(command, "load") == 0) {//============================LOAD===================================
                if (readFile(parameter, &deps, &jobs, &jobsForThreads, &threadsAmount, &jobsAmount) != OP_SUCCESS) {
                    printf("ERROR readFile function did not return OS_SUCCESS (file name was:%s)\n", parameter);
                }
                read_from_file_thread_amount = threadsAmount;
                ASSERT_RUN(printData());
            } else if (strcmp(command, "JW") == 0) {
                int threadNumber = 0;
                sscanf(parameter, "%d", &threadNumber); //converting parameter
                int waitAmount = jobs_wait(threadNumber);
                if (waitAmount >= 0)
                    printf("%d\n", waitAmount);
                else
                    printf("Unable to find thread\n");
            }

        } else if (strcmp(command, "exit") == 0) {//========================EXIT===================================
            break;
        } else if (strcmp(command, "MSW") == 0) {//=========================MSW===================================
            int res = maximal_switch_wait();
            printf("%d\n", res);
        } else if (strcmp(command, "MJW") == 0) {//=========================MJW===================================
            int res = maximal_jobs_wait();
            printf("%d\n", res);
        } else if (strcmp(command, "AJW") == 0) {//=========================AJW===================================
            float res = avarage_jobs_wait();
            printf("%f\n", res);
        } else if (strcmp(command, "tasks") == 0) {//=========================tasks===================================
            int res = total_jobs_wait();
            printf("%d\n", res);
        } else if (strcmp(command, "ASW") == 0) {//=========================ASW====================================
            float avg = avarage_switch_wait();
            printf("%f\n", avg);
        } else if (strcmp(command, "switches") == 0) {//===================SWITCHES=================================
            int total = total_switch_wait();
            printf("%d\n", total);
        } else if (strcmp(command, "run") == 0) {
            runType = malloc(sizeof (run_t));
            ASSERT(container && deps && jobs && jobsForThreads && threadsAmount);
            
            string sub_command = malloc(MAX_INPUT_LENGTH);
            memset(sub_command, 0, MAX_INPUT_LENGTH);
            scanf("%s", sub_command);
            if (strcmp(sub_command, "PB") == 0 || strcmp(sub_command, "pb") == 0 || strcmp(sub_command, "2") == 0) {
                *runType = PB;
				
                PB_array = calloc(threadsAmount, sizeof (PB_priority));
                int i = 1;
				
                scanf("%s", sub_command);
                PB_array[0] = atoi(sub_command);
                if (strcmp(sub_command, "-1") == 0)
                    for (i = 1; i < threadsAmount; i++) {
                        PB_array[i] = i + 1; //because there is an one offset between the thread index and thread's id
                    } else for (i; i < threadsAmount; i++) {
                        scanf("%s", sub_command);
                        PB_array[i] = atoi(sub_command);
                    }
                ASSERT_PRINT("The priority array is:");
                for (i = 0; i < threadsAmount; i++) {
                    ASSERT_PRINT("%d ", PB_array[i]);
                }
                ASSERT_PRINT("\n");
            } else if (strcmp(sub_command, "-1") == 0) {
                *runType = RR;
            } else if(strcmp(sub_command, "3") == 0) {
                *runType = YD;
                int i=0;
                for(i; i<threadsAmount; i++) {
                    PB_array[i] = 100;
                }
            }
            else {
                *runType = RR;
                int i=0;
                for(i; i<threadsAmount; i++) {
                    PB_array[i] = 0;
                }
            }
            int threadIndex = 0;
            for (threadIndex = 0; threadIndex < threadsAmount; threadIndex++) {
                if (*runType == PB || *runType==YD) {
                    if (create_thread(runThread, 0, 0, PB_array[threadIndex]) == -1) {
                        exit(67);
                    }
                } else {
                    if (create_thread(runThread, 0, 0, 0) == -1)
                        exit(67);
                }

            }
            threads_start_with_ui(ui_thread);

            //CLEAN UP
            if (sub_command) {
                free(sub_command);
                sub_command = NULL;
            }
            if (jobs)
                init_jobs_array(); //if we're on our second run
            threadsAmount = read_from_file_thread_amount;

            delete_statistics();

            /*
                        if (container->stats)
                        {
                                free(container->stats);
                                container->stats = NULL;
                        }
             */

            //list_clear_all_threads(container->container);

/*
            if (container->container) {

                if (container->container) {
                    free(container->container);
                    container->container = NULL;
                    if (container)
                        clear_container();
                }
            }
*/

            free_pb();
            if (runType) {
                free(runType);
                runType = NULL;
            }
            reset_iterator();
            id_iterator();
        }
    }

    if (runType) free(runType);
    free(command);
    free(parameter);
    free_memory();
}

int main() {
    //runTests();
    create_ui_thread(ui);
    reset_iterator();
    thread_manager_init(0, &(ui_thread->uc), 0);
    MCTX_RESTORE(ui_thread);
    return 0;
}

void initGlobalVars() {

}

int runTests() {
    //test_thread_printf();
    //test_find_thread_byID();
    //TestReadFile();
    //TestQ1();
}

