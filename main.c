/* 
 * File:   main.c
 * Authors: Danni Friedland & Yaniv Dudu
 *
 * Created on March 26, 2010, 4:40 PM
 */

#include "ydThreads.h"
#include "fileHandler.h"
#include "jobs.h"

void runThread() {
    while (hasPendingJobs(current_thread->id-1)) {
        tID jobId = getJobForThread(current_thread->id-1);
        if (jobId != NULL_TID)
        {
            execJob(jobId);
        }
        else {
            if (DEBUG) printf("Thread %d will wait because there is no free job for him to execute\n", current_thread->id-1);
        }
        thread_yield(0, 0);
    }
    thread_term();
}
mctx_t_p ui_thread;

mctx_t_p create_ui_thread(void* ui_func) {
    void* new_thread_stack = malloc(sizeof (char) * MAX_STACK_SIZE);
    mctx_t_p new_thread = malloc(sizeof (mctx_t));
    new_thread->threadStack = new_thread_stack;
    mctx_create(new_thread, ui_func, NULL, new_thread_stack, (sizeof (char) * MAX_STACK_SIZE), NULL);
    next_id--; //a hack to fix the iteration jump
    ui_thread = new_thread;
    return new_thread;
}

void ui() {
    string command = malloc(MAX_INPUT_LENGTH);
    string parameter = malloc(MAX_INPUT_LENGTH);

    while (strcmp(command, "exit") != 0) {
        printf(">");
        scanf("%s", command);
        if (strcmp(command, "load") == 0 || strcmp(command, "SW") == 0) {
            scanf("%s", parameter);
            if (strcmp(command, "SW") == 0) { //=============================SW==================================
                int threadNumber = 0;
                sscanf(parameter, "%d", &threadNumber);
                int waitAmount = switches_wait(threadNumber);
                if (waitAmount)
                    printf("%d\n", waitAmount);
                else
                    printf("Unable to find thread");
            } else if (strcmp(command, "load")==0) {//============================LOAD===================================
                if (readFile(parameter, &deps, &jobs, &jobsForThreads, &threadsNumber) != OP_SUCCESS) {
                    printf("ERROR readFile function did not return OS_SUCCESS (file name was:%s)\n", parameter);
                }
                if (DEBUG) printData();
            }

        } else if (strcmp(command, "exit") == 0) {//========================EXIT===================================
            break;
        } else if (strcmp(command, "MSW") == 0) {//=========================MSW===================================
            int max = maximal_switch_wait();
            printf("%d\n", max);
        } else if (strcmp(command, "ASW") == 0) {//=========================ASW====================================
            float avg = avarage_switch_wait();
            printf("%f\n", avg);
        } else if (strcmp(command, "switches") == 0) {//===================SWITCHES=================================
            int total = total_switch_wait();
            printf("%d\n", total);
        } else if (strcmp(command, "run")==0) {
            assert(deps && jobs && jobsForThreads && threadsNumber);
            int threadIndex = 0;
            for (threadIndex = 0; threadIndex < threadsNumber; threadIndex++) {
                assert(create_thread(runThread, 0) != -1);
            }
            threads_start_with_ui(ui_thread);
        }
    }
}

int main() {
    mctx_t_p ui_thread = create_ui_thread(ui);
    reset_iterator();
    thread_manager_init(0, &(ui_thread->uc));
    MCTX_RESTORE(ui_thread);
    return 0;
}

int runTests() {
    //test_thread_printf();
    //test_find_thread_byID();
    //TestReadFile();
    //TestQ1();
}

