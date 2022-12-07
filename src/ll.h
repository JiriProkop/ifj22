/**
 * Project: compiler of IFJ22 programming language
 *
 * @file ll.h
 * @brief Header file of linked list usef for storing parameters.
 *
 * @author Štěpán Czajkowski <xczajk01@stud.fit.vutbr.cz>
 */

#ifndef _ll_H
#define _ll_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include "dynstr.h"
#include "error.h"

/**
 * @brief list node_struct
 * 
 */
typedef struct list_node{
    keywords type;
    bool can_be_null;
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
 * @brief Function for initialization of list 
 * 
 * @param list the list to be inicialize
 */
void list_init(list_t *list);

/**
 * @brief Function for finding out if list is empty
 * 
 * @param list 
 * @return true is empty 
 * @return false is not empty 
 */
bool list_is_empty(list_t *list);

/**
 * @brief Function that adds node on the end of the list 
 * 
 * @param list 
 * @param token  
 */
int list_add(list_t *list, keywords type, dynstr_t* id);

/**
 * @brief Function that removes first node of the list, if the list is empty does nothing 
 * 
 * @param list list from which to delete
 */
void list_delete_first(list_t *list);

/**
 * @brief Function that returns the first node in the list
 * 
 * @param list the list to from which the first node will be taken  
 * @return First node or NULL if the list is empty 
 */
list_node_t *list_first(list_t *list);


/**
 * @brief Function that disposes of the list 
 * 
 * @param list the list to be disposed
 */
void list_dispose(list_t *list);

/**
 * @brief Function that searches the list for a key
 * 
 * @param list the list to be searched 
 * @param searched_str the key 
 * @return list_node_t* the searched node or NULL if not found 
 */
list_node_t *list_search(list_t *list, dynstr_t *searched_str);


#endif