/* 
 * File:   global_def.h
 * Authors: Danni Friedland & Yaniv Dudu
 *
 * Created on April 9, 2010, 6:20 PM
 */

#ifndef _GLOBALDEFS_H
#define	_GLOBALDEFS_H
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int tID;
typedef enum op_status {
    OP_SUCCESS=1,
    OP_FAIL=-1,
    OP_DONE
} op_status;
typedef enum boolean{
    True = 1,
    False = 0
} boolean;

typedef char* string;

/* thread stack size */
#define MAX_STACK_SIZE 4000
//#define TRUE 1
//#define FALSE 0
#define DEBUG 1
#define MAX_INPUT_LENGTH 50
#define NULL ((void *)0)
#define NULL_TID -1
#define OUT
#define IN
#define NULL    ((void *)0)

#endif	/* _GLOBALDEFS_H */

