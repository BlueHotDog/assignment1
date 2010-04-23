#include "tests.h"



void test_thread_printf() {
    printf("TEST:test_thread_printf\n");
    // int threadID =  create_thread(sampleThread, 0);

}

void test_find_thread_byID() {
    /*
        printf("TEST:test_find_thread_byID\n");
        thread_manager_init(0,NULL);
        int threadID = create_thread(a, 0);
        node_t_p currentNode = container->container;
        get_thread_byID(currentNode, threadID);
        threadToString(THREAD_DATA(currentNode));
     */
}

int TestReadFile() {
    printf("TEST:TestReadFile\n");
    int threads = 0;
    Jobs jobs;
    JobsDeps deps;
    JobsForThreads jobsForThreads;
    ASSERT(readFile("/home/danni/test", &deps, &jobs, &jobsForThreads, &threads) == OP_SUCCESS);
}

int TestQ1() {
/*
    printf("TEST:TestQ1\n");
    runType = malloc(sizeof(run_t));
    *runType = RR;
    thread_manager_init(0, NULL, 0);
    int i=create_thread(a, 0, 0, 0);
    printf("%d",i);
    create_thread(b, 0, 0, 0);
    create_thread(c, 0, 0, 0);
    threads_start();
    int waitAmount = switches_wait(1);
    printf("T1:%d", waitAmount);
    waitAmount = switches_wait(2);
    printf("T2:%d", waitAmount);
    waitAmount = switches_wait(3);
    printf("T3:%d", waitAmount);
    printf("\ndone...\n");
    free(runType);
*/
}
