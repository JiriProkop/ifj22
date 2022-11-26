#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "ll.h"
#include "error.h"


void list_init(list *list){
	list->end = NULL;
}

bool list_is_empty(list *list){
 	return list->end == NULL;
}

int list_add(list *list, token_type type, dynstr_t* id){
	list_node_t *node = malloc(sizeof(list_node_t));
	if(node == NULL){
		error_handle(0,compiler_error);
	}
	node->id = id;
	node->type = type;
	if(list->end == NULL){
		node->next = NULL;
	}else{
		node->next = list->end;
	}
	list->end = node;

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

// token_t *list_top(list *list){
// 	if(list->top == NULL){
// 		return NULL;
// 	}
// 	return list->top->current;
// }

// void list_dispose(list *list){
// 	list_node_t *tmp ;
// 	while(list->top != NULL){
// 		tmp = list->top;
// 		list->top = list->top->next;
// 		free(tmp);
// 	}
// }

// void list_dispose_all(list *list){
// 	token_t *tmp;
// 	list_node_t *tmp1;
// 	while(list->top != NULL){
// 		tmp = list->top->current;
// 		tmp1 = list->top;
// 		list->top = list->top->next;
// 		free(tmp);
// 		tmp = NULL;
// 		free(tmp1);
// 		tmp1 = NULL;
// 	}
// }

// token_t *ll_search(list *list);

// void *ll_insert_after(list *list);
