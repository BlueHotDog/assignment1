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

        //====reading thread count====
        c = fgets(oneLine, LINE_LENGTH, hFile); /* get one line from the file */
        *ThreadCount = readThreadAmount(oneLine);
        //checking empty lines
        boolean foundChar = False;
        while (!foundChar) {
            fgets(oneLine, LINE_LENGTH, hFile);
            p = oneLine;
            while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
                p++; // skip whitespaces
            if(*p!=0)
                foundChar = True;
        }
        //===reading jobs count=====
        jobsAmount = readJobsAmount(oneLine);
        //initializing Jobs dataStracture
        jobs = calloc(jobsAmount, sizeof (JobState));
        for (i; i <= jobsAmount - 1; i++)
            jobs[i] = 0;

        //initializing jobDeps DataStracture
        jobDeps = calloc(jobsAmount,sizeof(boolean*));

        for(i=0;i<=jobsAmount-1;i++)
        {
            jobDeps[i] = calloc(jobsAmount,sizeof(boolean));
        }
        do {
            c = fgets(oneLine, LINE_LENGTH, hFile); /* get one line from the file */
            if (c != NULL)
                printf("%s", oneLine); /* display it on the monitor  */
        } while (c != NULL);
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

