#include "linkedList.h"
#include "globalDefs.h"
#include "ydThreads.h"

node_t_p list_create(void* data) {
	node_t_p node;
	if(!(node=malloc(sizeof(node_t)))) {
            printf("ERROR cannot allocate memory for node at list_create function\n");
            return NULL;
        }
	node->data=data;
	node->next=NULL;
	return node;
}

op_status list_insert_after(node_t_p node, void *data) {
	node_t_p newnode;
        newnode=list_create(data);
        newnode->next = node->next;
        node->next = newnode;
	return OP_SUCCESS;
}

node_t_p list_insert_beginning(node_t_p list, void *data) {
	node_t_p newnode;
        newnode=list_create(data);
        newnode->next = list;
     	return newnode;
}

op_status list_clear_all_threads(node_t_p list)
{
    while(list)
    {
        node_t_p node = list->next;
        free(((mctx_t_p)list->next->data)->uc.uc_stack.ss_sp);
        free(((mctx_t_p)list->next->data));
        free(list);
        list = node;
        node = NULL;
    }
}
op_status list_remove_thread(node_t_p list, tID id) {
    //TODO: fix this..
    if (((mctx_t_p) list->data)->id== id)
    {
        node_t_p node = list->next;
        if (node)
        {
            mctx_t_p data = list->data;
            free(data->uc.uc_stack.ss_sp);
            free(data);
            free(list);
            container->container = node;
            return OP_SUCCESS;
        }
        free(((mctx_t_p)list->data)->uc.uc_stack.ss_sp);
        list->data == NULL;
        free (((mctx_t_p)list->data));
        free (list);
        return OP_DONE;
    }
    node_t_p p_list = list;

    while(p_list->next && ((mctx_t_p)p_list->next->data)->id != id)
        p_list = p_list->next;
    if(p_list->next && ((mctx_t_p)p_list->next->data)->id == id) {

        node_t_p node = p_list->next;
        p_list->next = node->next;
        free(((mctx_t_p)node->data)->uc.uc_stack.ss_sp);
        free(((mctx_t_p)node->data));
        free(node);
        node = NULL;
        return OP_SUCCESS;
    } else return OP_FAIL;
}

int list_foreach(node_t_p node, int(*func)(void*)) {
	while(node) {
		if(func(node->data)!=0) return -1;
		node=node->next;
	}
	return 0;
}

node_t_p list_find(node_t_p node, int(*func)(void*,void*), void *data) {
    //TODO:not working correctly
	while(node) {
		if(func(node->data, data)>0) return node;
		node=node->next;
	}
	return NULL;
}

node_t_p list_at(node_t_p list, int index) {
    ASSERT(list);
    node_t_p current = list;
    int i= 0;
    if (index < 0)
        return NULL;
    for (i=0; i<index; i++){
        if (current != NULL)
            current = current->next;
        else
            return NULL;
    }
    if (current != NULL)
        return current->data;
    else
        return NULL;
}

op_status list_add_last (node_t_p list, void* data) {
    if (list == NULL)
        return OP_FAIL;
    return list_add_last_rec (list, data);
}

op_status list_add_last_rec (node_t_p list, void* data) {
    if (list->next == NULL){
        list->next = list_create(data);
        return OP_SUCCESS;
    }
    return list_add_last_rec (list->next, data);
}

boolean list_is_empty (node_t_p list) {
    if (list == NULL)
        return true;
    if (list-> data == NULL && list->next == NULL)
        return true;
    return false;
}