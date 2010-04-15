/* 
 * File:   Jobs.h
 * Author: Danni Friedland & Yaniv Dudu
 *
 * Created on April 12, 2010, 8:09 PM
 */

#ifndef _JOBS_H
#define	_JOBS_H
#include "globalDefs.h"
typedef enum JobState
{
    NotStarted,
    InProgress,
    Done
} JobState;

typedef struct sThreadJobs
{
    int jobsAmount; //total number of jobs
    tID threadID; //identifier
    int* jobs; //what jobs are assigned to this thread
} ThreadJobs,*pThreadJobs;

typedef JobState* Jobs;
typedef boolean** JobsDeps; //dependencies between jobs, the graph matrix
typedef ThreadJobs* JobsForThreads; //relation between the threads and the jobs


boolean hasPendingJobs(tID threadId, JobsForThreads* jft, Jobs* jobsStatus);

/*
 * return next job for thread.
 */
tID getJobForThread(tID threadId, JobsForThreads* jft, Jobs* jobsStatus);

#endif	/* _JOBS_H */

