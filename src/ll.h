#ifndef _ll_H
#define _ll_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

#define MALLOC_ERROR 99

// /**
//  * @struct list node.
//  */
// typedef struct list_node {
// 	token_t *current;
// 	struct list_node
//  *next;
// } list_node_t;

typedef struct list_node{
    enum token_type; 
    char *id; 
} list_node_t;


/**
 * @struct list
 */
typedef struct{
	struct list_node *top; 
}list;

/**
 * @brief inicialize list 
 * 
 * @param list the list to be inicialize
 */
void list_init(list *list);

// /**
//  * @brief returns 1 if list is empty 0 if it is not 
//  * 
//  * @param list 
//  * @return true 
//  * @return false 
//  */
// bool list_is_empty(list *list);

// /**
//  * @brief adds/pushes token on list 
//  * 
//  * @param list the list on which the token is to be pushed
//  * @param token the token to be pushed 
//  */
// int list_push(list *list, token_t *token);

// /**
//  * @brief removes/pops token of list, if the list is empty does nothing 
//  * 
//  * @param list the list from which to pop
//  */
// void list_pop(list *list);

// /**
//  * @brief returns the first node in list without it being removed
//  * 
//  * @param list the list to from which the first node will be 
//  * @param return_token pointer to the first token in list 
//  */
// token_t *list_top(list *list);

// /**
//  * @brief disposes of list leaves the tokens be
//  * 
//  * @param list the list to dispose off
//  */
// void list_dispose(list *list);

// /**
//  * @brief disposes of list and the tokens in it 
//  * 
//  * @param list the list to be disposed
//  */
// void list_dispose_all(list *list);

// /**
//  * @brief returns topmost terminal from list
//  * 
//  * @param list list to be searched
//  * @return token_t* terminal of NULL if no terminal found on list
//  */
// token_t *list_top_terminal(list *list);


// token_t *ll_search(list *list);

// void *ll_insert_after(list *list);


#endif