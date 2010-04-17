/* 
 * File:   main.c
 * Authors: Danni Friedland & Yaniv Dudu
 *
 * Created on March 26, 2010, 4:40 PM
 */

#include "ydThreads.h"
#include "fileHandler.h"

void a() {
    int i; //hhh
    for (i = 0; i < 10; i++) {
        printf("a\n");
        thread_yield(0, 0);
    }
    thread_term();
}

void b() {
    int i;
    for (i = 0; i < 8; i++) {
        printf("b\n");
        thread_yield(0, 0);
    }
    thread_term();
}

void c() {
    int i;
    for (i = 0; i < 15; i++) {
        printf("c\n");
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
    create_thread(a, 0);
    create_thread(b, 0);
    create_thread(c, 0);
    threads_start_with_ui(ui_thread);
    while (strcmp(command, "exit") != 0) {
        printf(">");
        scanf("%s", command);
        if (strcmp(command, "run") == 0 || strcmp(command, "load") == 0 || strcmp(command, "SW") == 0) {
            scanf("%s", parameter);
            if (strcmp(command, "run") == 0) {
                int threads = 0;
                Jobs jobs;
                JobsDeps deps;
                JobsForThreads jobsForThreads;
                readFile(parameter, &deps, &jobs, &jobsForThreads, &threads);
            } else if (strcmp(command, "SW") == 0) {
                int threadNumber = 0;
                sscanf(parameter, "%d", &threadNumber);
                int waitAmount = switches_wait(threadNumber);
                if (waitAmount)
                    printf("%d\n", waitAmount);
                else
                    printf("Unable to find thread");
            }

        } else if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "MSW") == 0) {
            int max = maximal_switch_wait();
            printf("%d\n", max);
        } else if (strcmp(command, "ASW") == 0) {
            float avg = avarage_switch_wait();
            printf("%f\n", avg);
        } else if(strcmp(command,"switches")==0) {
            int total = total_switch_wait();
            printf("%d\n",total);
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

