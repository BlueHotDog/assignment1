#include "jobs.h"

boolean hasPendingJobs(tID threadId) {
    ThreadJobs threadJobs = jobsForThreads[threadId];
    int i = 0;
    for (i; i < threadJobs.jobsAmount; i++) {
        tID testedJob = threadJobs.jobs[i] - 1;
        if (jobs[testedJob] == NotStarted)
            return true;
    }
    return false;
}

/* return next job for thread.*/
tID getJobForThread(tID threadId) {
    tID yourNextJob = NULL_TID;
    if (!hasPendingJobs(threadId)) {
        return NULL_TID;
    }
    ThreadJobs threadJobs = jobsForThreads[threadId];
    int i = 0;
    for (i = 0; i < threadJobs.jobsAmount; i++) {
        tID jobId = threadJobs.jobs[i] - 1;
        int prevJob = 0;
        boolean legalJobToExecute = true;
        for (prevJob = 0; prevJob < jobsAmount && legalJobToExecute; prevJob++) {
            if (jobs[jobId] != NotStarted || (deps[prevJob][jobId] == 1 && jobs[prevJob] != Done)) {
                legalJobToExecute = false;
                break;
            }
            //printf("jobs[%d] != NotStarted=>%d || (deps[%d][%d]==1=>%d && jobs[%d] != Done =>%d\n",jobId,(jobs[jobId] != NotStarted),(jobId-1),(prevJob-1),(deps[jobId-1][prevJob-1]==1),prevJob,(jobs[prevJob] != Done));
        }
        if (legalJobToExecute) {
            yourNextJob = jobId;
            break;
        }
    }
    return yourNextJob;
}

op_status execJob(tID jobId) {
    ASSERT(jobs);
    jobs[jobId] = InProgress;
    printf("Thread %d performed job %d\n", jobsForThreads[current_thread->id - 1].threadID - 1, jobId);
    jobs[jobId] = Done;
}

void printData() {
    printf("===============info about the input:===============\n");
    printf("ThreadNumber=%d\n", threadsAmount);
    printf("jobsAmount=%d\n", jobsAmount);
    printf("Deps:\n");
    ASSERT(deps);
    int i = 0, j = 0;
    for (i = 0; i < jobsAmount; i++) {
        for (j = 0; j < jobsAmount; j++) {
            printf("%X ", deps[i][j]);
        }
        printf("\n");
    }
    printf("Jobs:\n");
    ASSERT(jobs);
    i = 0;
    for (i = 0; i < jobsAmount; i++) {
        switch (jobs[i]) {
            case NotStarted:
                printf("job[%d]=NotStarted ", i);
                break;
            case InProgress:
                printf("job[%d]=InProgress ", i);
                break;
            case Done:
                printf("job[%d]=Done ", i);
                break;
            default:
                printf("job[%d]=ERROR! ", i);
        }
    }
    printf("\n");
    i = 0;
    for (i = 0; i != threadsAmount; i++) {
        printf("jobsForThreads %d:\n\tJob Amount:%d\n\tThread ID: %d\n\tJobs: ", i, jobsForThreads[i].jobsAmount, jobsForThreads[i].threadID);
        for (j = 0; jobsForThreads[i].jobs[j] != 0; j++) {
            printf("%d ", jobsForThreads[i].jobs[j]);
        }
        printf("\n");
    }

}