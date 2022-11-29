#ifndef _ll_H
#define _ll_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include "dynstr.h"

/**
 * @brief list node_struct
 * 
 */
typedef struct list_node{
    token_type type; 
    dynstr_t *id; 
    struct list_node *next;
} list_node_t;


/**
 * @struct list struct
 */
typedef struct{
    struct list_node *first;
	struct list_node *last; 
}list_t;

/**
 * @brief inicialize list 
 * 
 * @param list the list to be inicialize
 */
void list_init(list_t *list);

/**
 * @brief returns 1 if list is empty 0 if it is not 
 * 
 * @param list 
 * @return true is empty 
 * @return false is not empty 
 */
bool list_is_empty(list_t *list);

/**
 * @brief adds node on the end of the list 
 * 
 * @param list 
 * @param token  
 */
int list_add(list_t *list, token_type type, dynstr_t* id);

/**
 * @brief removes first node of the list, if the list is empty does nothing 
 * 
 * @param list list from which to delete
 */
void list_delete_first(list_t *list);

/**
 * @brief returns the first node in the list
 * 
 * @param list the list to from which the first node will be 
 * @return first node or NULL if the list is empty 
 */
list_node_t *list_first(list_t *list);


/**
 * @brief disposes of the list 
 * 
 * @param list the list to be disposed
 */
void list_dispose(list_t *list);


list_node_t *list_search(list_t *list, dynstr_t *searched_str);


#endif