#include "fileHandler.h"
#include "globalDefs.h"

op_status readFile(string file_name, JobsDeps* jobDeps, Jobs* jobs, JobsForThreads* jobsForThreads, int* threadCount, int* jobsCount) {

    FILE* hFile;
    hFile = fopen(file_name, "r");
    int i=0, j = 0;
    if (hFile == NULL) {
        return OP_FAIL;
    } else {
        string c;
        char* p;
        char oneLine[LINE_LENGTH];

        //reading thread count=======================
        c = fgets(oneLine, LINE_LENGTH, hFile); /* get one line from the file */
        *threadCount = readThreadAmount(oneLine);
        //===========================================
        //checking empty lines=======================
        GET_NEXT_LINE(oneLine, hFile);
        //===========================================
        //reading jobs count=========================
        *jobsCount = readJobsAmount(oneLine);
        //initializing Jobs dataStracture
        if(!*jobs) *jobs = calloc(*jobsCount, sizeof (JobState));

        for (i=0; i < *jobsCount; i++)
            (*jobs)[i] = NotStarted;
        //===========================================
        //initializing jobDeps DataStracture=========
        *jobDeps = calloc(*jobsCount, sizeof (boolean*));
        for (i = 0; i < *jobsCount; i++) {
            if(!(*jobDeps)[i]) (*jobDeps)[i] = calloc(*jobsCount, sizeof (boolean));
            for (j = 0; j<*jobsCount; j++)
                (*jobDeps)[i][j] = false;

        }
        int col = 0;
        for (i = 0; i <= *jobsCount - 1; i++)//reading input file rows
        {
            GET_NEXT_LINE(oneLine, hFile);
            char* currChar = oneLine;
            for (col = 0; col <= *jobsCount - 1; col++)//reading input file colums
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
        if(!*jobsForThreads) *jobsForThreads = calloc(*threadCount, sizeof (ThreadJobs));
        for (i = 0; i<*threadCount; i++) {
            GET_NEXT_LINE(oneLine, hFile);
            if (*oneLine != 0) {
                int currentThread, currentJob, counter = 0;
                char* currChar = oneLine;
                while (*currChar == ' ' || *currChar == '\t')
                    currChar++;
                sscanf(currChar, "%d:%s", &currentThread,currChar);
                if(!(*jobsForThreads)[currentThread - 1].jobs) (*jobsForThreads)[currentThread - 1].jobs = calloc(*jobsCount, sizeof (int));
                (*jobsForThreads)[currentThread - 1].threadID = currentThread;

                //currChar = currChar + 2;
                while (*currChar != '\r' && *currChar != '\n') {
                    while (*currChar == ' ' || *currChar == '\t' || *currChar == '\0')
                        currChar++;
                    if (*currChar != '\n' && *currChar != '\r') {
                        if(sscanf(currChar, "%d,", &currentJob)==0)
                            sscanf(currChar, "%d", &currentJob);
                        while (*currChar != '\r' && *currChar != '\n' && *currChar != ',')
                            currChar++;
                        if (*currChar == ',')
                            currChar++;

                        (*jobsForThreads)[currentThread - 1].jobs[counter++] = currentJob;
                    }
                }
                (*jobsForThreads)[currentThread - 1].jobsAmount = counter;
            }
        }

        //===========================================
        fclose(hFile);
    }
    return OP_SUCCESS;
}

int readThreadAmount(string line) {
    int toReturn = -1;
    sscanf(line, "k = %d", &toReturn);
    return toReturn;
}

int readJobsAmount(string line) {
    int toReturn = -1;
    sscanf(line, "n = %d", &toReturn);
    return toReturn;
}


