/* 
 * File:   main.c
 * Authors: Danni Friedland & Yaniv Dudu
 *
 * Created on March 26, 2010, 4:40 PM
 */

#include "yd_threads.h"

void a()
{
    int i;
    for (i = 0; i < 10; i++)
    {
        printf("a");
        thread_yield(0, 0);
    }
    thread_term();
}

void b()
{
    int i;
    for (i = 0; i < 8; i++)
    {
        printf("b");
        thread_yield(0, 0);
    }
    thread_term();
}

void c()
{
    int i;
    for (i = 0; i < 15; i++)
    {
        printf("c\n");
        thread_yield(0, 0);
    }
    thread_term();
}


int main()
{
  
  string command = malloc(MAX_INPUT_LENGTH);
  string parameter = malloc(MAX_INPUT_LENGTH);

  while(strcmp(command,"exit")!=0)
  {
      printf(">");
      scanf("%s",command);
      if(strcmp(command,"run")==0 || strcmp(command,"load") == 0)
              scanf("%s",parameter);
      else if(strcmp(command,"exit")!=0)
          break;
  }
/*
  thread_manager_init(0);
  create_thread(a, 0);
  create_thread(b, 0);
  create_thread(c, 0);
  threads_start();
*/

  printf("\ndone...\n");
  return 0;
}

