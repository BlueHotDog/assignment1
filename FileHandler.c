#include "FileHandler.h"
#include "GlobalDefs"
op_status readFile(string file_name,JobsDeps &jobDeps,Jobs &jobs,JobsForThreads &jobsForThreads,int &ThreadCount)
{
    FILE* hFile;
hFile = fopen( file_name, "r");
if (hFile == NULL)
{
// Error, file not found
}
else
{
// Process & close file
fclose(hFile);
}

    /*
     * Open file (maybe check if exists)
     * read line from file
     * call readThreadAmount and save return value
     * while current line is empty continue to next line
     * call readJobsAmount and save return value
     * while current line is empty continue to next line
     * init current job=1
     * do while current line is not empty
     * call readJobsDeps with current job
     * .
     * .
     * .
     */
}