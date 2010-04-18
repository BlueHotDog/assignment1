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
//#define NULL ((void *)0)
#define NULL_TID -1
#define OUT
#define IN


//DEBUGGING
#ifdef DEBUG
# define __ASSERT_VOID_CAST (void)
# define assert(expr)							\
  ((expr)								\
   ? __ASSERT_VOID_CAST (0)						\
   : printf("=========assertion failed on line:%d in file:%s\n",__LINE__,__FILE__))
#else
    # define assert(expr) ASSERT_VOID_CAST (0)
#endif

#endif	/* _GLOBALDEFS_H */

