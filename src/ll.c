/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Implementation of linked list usef for storing parameters.
 *
 * @author Štěpán Czajkowski <xczajk01@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "ll.h"
#include "error.h"

/**
 * @brief Function for initialization of list 
 * 
 * @param list the list to be inicialize
 */
void list_init(list_t *list){
	list->last = NULL;
	list->first = NULL;
}

/**
 * @brief Function for finding out if list is empty
 * 
 * @param list 
 * @return true is empty 
 * @return false is not empty 
 */
bool list_is_empty(list_t *list){
 	return list->last == NULL;
}

/**
 * @brief Function that adds node on the end of the list 
 * 
 * @param list 
 * @param token  
 */
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

/**
 * @brief Function that removes first node of the list, if the list is empty does nothing 
 * 
 * @param list list from which to delete
 */
void list_delete_first(list_t *list){
	if(list->first == NULL){
		return;
	}
	list_node_t *del_node;
	del_node = list->first;
	list->first = del_node->next;
	string_free(del_node->id);
	free(del_node);
}

/**
 * @brief Function that returns the first node in the list
 * 
 * @param list the list to from which the first node will be taken  
 * @return First node or NULL if the list is empty 
 */
list_node_t *list_first(list_t *list){
	if(list->first == NULL){
		return NULL;
	}
	return list->first;
}

/**
 * @brief Function that disposes of the list
 * 
 * @param list the list to be disposed
 */
void list_dispose(list_t *list){
	if(list == NULL){
		return;
	}
	list_node_t *delete_node, *i = list->first;
	while(i != NULL){
		string_free(i->id);
		delete_node = i;
		i = i->next;
		free(delete_node);

		delete_node = NULL;
	}
	free(list);
	list = NULL;
}

/**
 * @brief Function that searches the list for a key
 * 
 * @param list the list to be searched 
 * @param searched_str the key 
 * @return list_node_t* the searched node or NULL if not found 
 */
list_node_t *list_search(list_t *list, dynstr_t *searched_str){
	if(list == NULL){
		return NULL;
	}

	list_node_t *i = list->first;
	while(i != NULL && !dynstr_compare(searched_str, i->id->array)){
		i = i->next;
	}
	return i;
}

