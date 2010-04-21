/* 
 * File:   LinkedList.h
 * Authors: Danni Friedland & Yaniv Dudu
 *
 * Created on March 27, 2010, 3:09 PM
 */

#ifndef _LINKEDLIST_H
#define	_LINKEDLIST_H

#include <stdlib.h>
#include <stdio.h>
#include "globalDefs.h"

typedef struct node_s {
	void *data;
	struct node_s *next;
} node_t,*node_t_p;

node_t_p list_find(node_t_p node, int(*func)(void*,void*), void *data);
node_t_p list_at(node_t_p list, int index);
op_status list_remove_thread(node_t_p list, tID id);
op_status list_clear_all_threads(node_t_p list);
int list_foreach(node_t_p node, int(*func)(void*));
node_t_p list_insert_beginning(node_t_p list, void* data);
op_status list_insert_after(node_t_p node, void* data);
node_t_p list_create(void* data);
op_status list_add_last (node_t_p list, void* data);
op_status list_add_last_rec (node_t_p list, void* data);
boolean list_is_empty (node_t_p list);

#endif	/* _LINKEDLIST_H */

