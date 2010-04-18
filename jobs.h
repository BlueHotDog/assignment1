/* 
 * File:   Jobs.h
 * Author: Danni Friedland & Yaniv Dudu
 *
 * Created on April 12, 2010, 8:09 PM
 */

#ifndef _JOBS_H
#define	_JOBS_H

#include "globalDefs.h"
#include "ydThreads.h"

typedef enum JobState
{
    NotStarted=1,
    InProgress=2,
    Done=3
} JobState;

typedef struct sThreadJobs
{
    int jobsAmount;
    tID threadID;
    tID* jobs;
} ThreadJobs,*pThreadJobs;

typedef JobState* Jobs;
typedef boolean** JobsDeps; //dependencies between jobs, the graph matrix
typedef ThreadJobs* JobsForThreads; //relation between the threads and the jobs

boolean hasPendingJobs (tID threadId);

/* return next job for thread.*/
tID getJobForThread (tID threadId);

/* execute the job by changind the job status to done */
op_status execJob (tID jobId);

void printData();

JobsForThreads jobsForThreads;
Jobs jobs;
JobsDeps deps;
int threadsNumber;
int jobNumber;

#endif	/* _JOBS_H */
