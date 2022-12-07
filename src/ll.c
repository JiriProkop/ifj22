/**
 * Project: compiler of IFJ22 programming language
 *
 * @file ll.c
 * @brief Header file of linked list usef for storing parameters.
 *
 * @author Štěpán Czajkowski <xczajk01@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "ll.h"
#include "dynstr.h"
#include "error.h"


void list_init(list_t *list){
	list->last = NULL;
	list->first = NULL;
}

int list_add(list_t *list, keywords type, dynstr_t* id){
	list_node_t *node = malloc(sizeof(list_node_t));
	if(node == NULL){
		error_handle(0,compiler_error);
	}
	node->id = id;
	node->type = type;
	// if the only node in the list
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

list_node_t *list_first(list_t *list){
	if(list->first == NULL){
		return NULL;
	}
	return list->first;
}

void list_dispose(list_t *list){
	if(list == NULL){
		return;
	}
	list_node_t *delete_node, *i = list->first;
	while(i != NULL){
		if(i->id != NULL && i->id->array != NULL) {
			dynstr_delete(i->id);
			free(i->id);
			i->id->array = NULL;
		}

		delete_node = i;
		i = i->next;
		free(delete_node);

		delete_node = NULL;
	}
	free(list);
	list = NULL;
}
