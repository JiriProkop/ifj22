#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "ll.h"
#include "error.h"


void list_init(list_t *list){
	list->last = NULL;
	list->first = NULL;
}

bool list_is_empty(list_t *list){
 	return list->last == NULL;
}

int list_add(list_t *list, token_type type, dynstr_t* id){
	list_node_t *node = malloc(sizeof(list_node_t));
	if(node == NULL){
		error_handle(0,compiler_error);
	}
	node->id = id;
	node->type = type;

	if(list->first == NULL){
		node->next = NULL;
		list->last = node;
		list->first = node;
	}else{
		list->last->next = node;
		node->next = NULL;
		list->last = node;
	}
	return 0;
}
// void list_pop(list *list){
// 	token_t *tmp;
// 	list_node_t *tmp1;
// 	if(list->top == NULL){
// 		return;
// 	}
// 	tmp = list->top->current;
// 	tmp1 = list->top;
// 	list->top = list->top->next;
// 	free(tmp);
// 	tmp = NULL;
// 	free(tmp1);
// 	tmp1 = NULL;
// }

token_t *list_first(list_t *list){
	if(list->first == NULL){
		return NULL;
	}
	return list->first;
}

void list_dispose(list_t *list){
	list_node_t *delete_node, *i = list->first;
	while(i != NULL){
		string_free(i->id);
		delete_node = i;
		i = i->next;
		free(delete_node);
		delete_node = NULL;
	}
	free(list);
}

// list_node_t *ll_search(list *list);

