/* 
 * File:   global_def.h
 * Authors: Danni Friedland & Yaniv Dudu
 *
 * Created on April 9, 2010, 6:20 PM
 */

#ifndef _GLOBAL_DEF_H
#define	_GLOBAL_DEF_H
#include <unistd.h>
#include <string.h>

typedef int tID;
typedef enum op_status {
    OP_SUCCESS=1,
    OP_FAIL=-1,
    OP_DONE
} op_status;


typedef char* string;

/* thread stack size */
#define MAX_STACK_SIZE 4000
#define TRUE 1
#define FALSE 0
#define DEBUG 1
#define MAX_INPUT_LENGTH 50
#define NULL ((void *)0)

#endif	/* _GLOBAL_DEF_H */

