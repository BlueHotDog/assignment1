/*
 * File:   FileHandler.h
 * Author: Danni Friedland & Yaniv Dudu
 *
 * Created on April 12, 2010, 8:16 PM
 */
#ifndef _FILEHANDLER_H
#define	_FILEHANDLER_H
#include "jobs.h"
#define LINE_LENGTH 1000
/*
 * Reads the input file,
 * file name - file in current directory
 * jobDeps - Returns the job dependency matrix
 * Jobs - Creates an array of jobs in a not-started manner
 * JobsForThreads - self explained
 * ThreadCount - returns K from file
*/
op_status readFile(string file_name,JobsDeps* jobDeps,Jobs* jobs,JobsForThreads* jobsForThreads,int* ThreadCount);
//Reads the k=x and returns x, -1 otherwise.
int readThreadAmount(string line);
int readJobsAmount(string line);
//returns a list of dependencies for the jobID as a list of booleans
boolean* readJobsDeps(string line,tID jobID);
//reads the job and thread connection
ThreadJobs readThead(string line);





#endif	/* _FILEHANDLER_H */

