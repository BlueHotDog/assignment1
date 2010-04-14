#include "fileHandler.h"
#include "globalDefs.h"

op_status readFile(string file_name, JobsDeps* jobDeps, Jobs* jobs, JobsForThreads* jobsForThreads, int* ThreadCount) {
    FILE* hFile;
    hFile = fopen(file_name, "r");
    if (hFile == NULL) {
        return OP_FAIL;
    } else {

        while(!feof(hFile))
        {
            char *cLine = NULL;
            //fscanf(hFile, "%s", cLine);
            fscanf(hFile,"%[^\n]",cLine);
            printf("value read: [%s]\n", cLine);

        }
        fclose(hFile);
    }
}

int readThreadAmount(string line)
{
    char cChar;

}