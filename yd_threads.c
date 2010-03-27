#include "yd_threads.h"
#include<stdlib.h>
#include<stdio.h>


void mctx_create(mctx_t_p mctx, void (*sf_addr)(), void *sf_arg, void *sk_addr, size_t sk_size) {
    /* fetch current context */
    getcontext(&(mctx->uc));
    /* adjust to new context */
    mctx->uc.uc_link = NULL;
    mctx->uc.uc_stack.ss_sp = sk_addr;
    mctx->uc.uc_stack.ss_size = sk_size;
    mctx->uc.uc_stack.ss_flags = 0;
    /* make new context */
    makecontext(&(mctx->uc), sf_addr, 1, sf_arg);
    return;
}

node_t_p list_create(void* data)
{
	node_t_p node;
	if(!(node=malloc(sizeof(node_t)))) return NULL;
	node->data=data;
	node->next=NULL;
	return node;
}
node_t_p list_insert_after(node_t_p node, void *data)
{
	node_t_p newnode;
        newnode=list_create(data);
        newnode->next = node->next;
        node->next = newnode;
	return newnode;
}
node_t_p list_insert_beginning(node_t_p list, void *data)
{
	node_t_p newnode;
        newnode=list_create(data);
        newnode->next = list;
	return newnode;
}
int list_remove(node_t_p list, node_t_p node)
{
	while(list->next && list->next!=node) list=list->next;
	if(list->next) {
		list->next=node->next;
		free(node);
		return 0;
	} else return -1;
}
int list_foreach(node_t_p node, int(*func)(void*))
{
	while(node) {
		if(func(node->data)!=0) return -1;
		node=node->next;
	}
	return 0;
}

node_t_p list_find(node_t_p node, int(*func)(void*,void*), void *data)
{
	while(node) {
		if(func(node->data, data)>0) return node;
		node=node->next;
	}
	return NULL;
}