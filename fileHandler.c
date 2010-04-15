#include "fileHandler.h"
#include "globalDefs.h"

op_status readFile(string file_name, JobsDeps* jobDeps, Jobs* jobs, JobsForThreads* jobsForThreads, int* ThreadCount) {
    FILE* hFile;
    hFile = fopen(file_name, "r");
    int jobsAmount, i = 0;
    if (hFile == NULL) {
        return OP_FAIL;
    } else {
        string c;
        char* p;
        char oneLine[LINE_LENGTH];

        //reading thread count=======================
        c = fgets(oneLine, LINE_LENGTH, hFile); /* get one line from the file */
        *ThreadCount = readThreadAmount(oneLine);
        //===========================================
        //checking empty lines=======================
        GET_NEXT_LINE(oneLine, hFile);
        //===========================================
        //reading jobs count=========================
        jobsAmount = readJobsAmount(oneLine);
        //initializing Jobs dataStracture
        *jobs = calloc(jobsAmount, sizeof (JobState));
        for (i; i <= jobsAmount - 1; i++)
            (*jobs)[i] = 0;
        //===========================================
        //initializing jobDeps DataStracture=========
        *jobDeps = calloc(jobsAmount, sizeof (boolean*));

        for (i = 0; i <= jobsAmount - 1; i++) {
            (*jobDeps)[i] = calloc(jobsAmount, sizeof (boolean));
        }
        int col = 0;
        for (i = 0; i <= jobsAmount - 1; i++)//reading input file rows
        {
            GET_NEXT_LINE(oneLine, hFile);
            char* currChar = oneLine;
            for (col = 0; col <= jobsAmount - 1; col++)//reading input file colums
            {
                while (*currChar == ' ' || *currChar == '\t' || *currChar == '\r' || *currChar == '\n')
                    currChar++;
                int temp = 0;
                sscanf(currChar, "%d", &temp);
                (*jobDeps)[i][col] = temp;
                currChar++;
            }
        }
        //===========================================
        //Initializing Jobs for thread stracture=====
        *jobsForThreads = calloc(*ThreadCount, sizeof (ThreadJobs));
        for (i = 0; i<*ThreadCount; i++) {
            GET_NEXT_LINE(oneLine, hFile);
            if (*oneLine != 0) {
                int currentThread, currentJob, counter = 0;
                char* currChar = oneLine;
                while (*currChar == ' ' || *currChar == '\t')
                    currChar++;
                sscanf(currChar, "%d:", &currentThread);
                (*jobsForThreads)[currentThread-1].jobs = calloc(jobsAmount, sizeof (int));
                (*jobsForThreads)[currentThread-1].threadID = currentThread;

                currChar = currChar + 2;
                while (*currChar != '\r' && *currChar != '\n') {
                    while (*currChar == ' ' || *currChar == '\t' || *currChar == '\0')
                        currChar++;
                    if (*currChar != '\n' && *currChar != '\r') {
                        sscanf(currChar, "%d,", &currentJob);
                        while (*currChar != '\r' && *currChar != '\n' && *currChar != ',')
                            currChar++;
                        if (*currChar == ',')
                            currChar++;

                        (*jobsForThreads)[currentThread-1].jobs[counter++] = currentJob;
                    }
                }
                (*jobsForThreads)[currentThread-1].jobsAmount = counter;
            }
        }

        //===========================================
        fclose(hFile);
    }
}

int readThreadAmount(string line) {
    int toReturn;
    sscanf(line, "k = %d", &toReturn);
    return toReturn;
}

int readJobsAmount(string line) {
    int toReturn;
    sscanf(line, "n = %d", &toReturn);
    return toReturn;
}


