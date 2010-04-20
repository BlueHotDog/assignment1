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
typedef char boolean;
#define false 0
#define true (!false)

typedef char* string;

/* thread stack size */
#define MAX_STACK_SIZE 4000
#define MAX_INPUT_LENGTH 50
#define NULL ((void *)0)
#define NULL_TID -1
#define OUT
#define IN

//#define DEBUG

//DEBUGGING
# define __ASSERT_VOID_CAST (void)
#ifdef DEBUG
    #define ASSERT(expr)							\
      ((expr)								\
       ? __ASSERT_VOID_CAST (0)						\
       : exit(printf("=========assertion failed on line:%d\t file:%s\n",__LINE__,__FILE__)))
    #define ASSERT_RUN(expr) (expr)
    #define ASSERT_PRINT(expr...)  (printf(expr))
#else
    #define ASSERT(expr)        __ASSERT_VOID_CAST (0)
    #define ASSERT_RUN(expr)    __ASSERT_VOID_CAST (0)
    #define ASSERT_PRINT(expr...)  __ASSERT_VOID_CAST (0)
#endif
#endif	/* _GLOBALDEFS_H */

