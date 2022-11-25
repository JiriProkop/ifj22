#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "ll.h"


void list_init(list *list){
	list->top = NULL;
}

// bool list_is_empty(list *list){
// 	return list->top == NULL;
// }

// int list_push(list*list, token_t *token){
// 	list_node_t *tmp = malloc(sizeof(list_node_t));
// 	if(tmp == NULL){
// 		return MALLOC_ERROR;
// 	}
// 	if(list == NULL){
// 		tmp->next = NULL;
// 	} else{
// 		tmp->next = list->top;
// 	}
// 	tmp->current = token;
// 	list->top = tmp;
// 	return 0;
// }

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
