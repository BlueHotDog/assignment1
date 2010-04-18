#include "jobs.h"

boolean hasPendingJobs (tID threadId)
{
    ThreadJobs threadJobs = jobsForThreads[threadId];
    int i=0;
    for (i; i<threadJobs.jobsAmount; i++) {
        tID testedJob = threadJobs.jobs[i];
        if (jobs[testedJob] == NotStarted)
            return True;
    }
    return False;
}

/* return next job for thread.*/
tID getJobForThread (tID threadId)
{
    tID yourNextJob = NULL_TID;
    if (!hasPendingJobs(threadId)) {
        return NULL_TID;
    }
    ThreadJobs threadJobs = jobsForThreads[threadId];
    int i=0;
    for (i=0; i< threadJobs.jobsAmount; i++) {
        tID jobId = threadJobs.jobs[i];
        int prevJob=0;
        boolean legalJobToExecute = True;
        for (prevJob=1; prevJob<= jobNumber && legalJobToExecute; prevJob++) {
            if (jobs[jobId] != NotStarted || (deps[prevJob-1][jobId-1]==1 && jobs[prevJob] != Done))
            {
                legalJobToExecute = False;
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

op_status execJob (tID jobId)
{
    jobs[jobId] = InProgress;
    printf("Thread %d performed job %d\n",jobsForThreads[current_thread->id].threadID, jobId);
    jobs[jobId] = Done;
}

void printData()
{
    printf("info about the input:\n");
    printf("ThreadNumber=%d\n",threadsNumber);
    printf("Deps:\n");
    int i,j;
    for (i=0; deps[i] != NULL; i++)
    {
        for (j=0; deps[j] != NULL; j++)
        {
            printf("%d ",deps[i][j]);
        }
        printf("\n");
    }
    printf("Jobs:\n");
    i=0;
    for (i=0; jobs[i] != 0; i++)
    {
        switch (jobs[i])
        {
            case NotStarted:
                printf("job[%d]=NotStarted ",i);
            break;
            case InProgress:
                printf("job[%d]=InProgress ",i);
            break;
            case Done:
                printf("job[%d]=Done ",i);
            break;
            default:
                printf("job[%d]=ERROR! ",i);
        }
    }
    printf("\n");
    i=0;
    for (i=0; i != threadsNumber; i++)
    {
        printf("jobsForThreads %d:\n\tJob Amount:%d\n\tThread ID: %d\n\tJobs: ",i ,jobsForThreads[i].jobsAmount, jobsForThreads[i].threadID);
        for (j=0; jobsForThreads[i].jobs[j] != 0; j++)
        {
            printf("%d ", jobsForThreads[i].jobs[j]);
        }
        printf("\n");
    }

}