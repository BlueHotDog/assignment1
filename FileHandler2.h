/* 
 * File:   FileHandler.h
 * Authors: Danni Friedland & Yaniv Dudu
 *
 * Created on April 12, 2010, 4:50 PM
 */

#ifndef _FILEHANDLER_H
#define	_FILEHANDLER_H

#include "global_def.h"


typedef short tThread;
typedef short tJobID;
typedef short tVertexNumbrt;
typedef short tThreadNumber;
typedef node_t_p* tThreadsArray;
//typedef char* string;

typedef struct jobGraphData {
    tVertexNumbrt n;
    node_t_p job_list;
}tJobGraphData, *tJobGraphData_p;

typedef struct threadList {
    tThreadNumber k;
    tThreadsArray threads_array;
}tThreadList, *tThreadList_p;

class FileHandler {
private:
    tJobGraphData job_graph;
    tThreadList thread_list;
    
public:
    FileHandler(string file_name);
    /* read file and fill k, n and job_list */
    op_status readFile(string file_name, tJobGraphData &graph);
};

class Graph {
private:
    tJobGraphData_p graph;
    op_status updateGraph(tJobID job_id);

public:
    /* find the next job for given thread
     * and remove the job fron the job list */
    tJobID getNextJobForThread (tThread thread);
    tVertexNumbrt getVertexNumber ();
    op_status setVertexNumber (tVertexNumbrt n);
};

#endif	/* _FILEHANDLER_H */

