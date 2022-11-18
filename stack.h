#ifndef _STACK_H
#define _STACK_H

#include "scanner.h"

#define MALLOC_ERROR 99

/**
 * @struct stack node.
 */
typedef struct stack {
    token_t *current;
    stack_node_t *next;
} stack_node_t;

/**
 * @brief inicialize stack 
 * 
 * @param stack the stack to be inicialize
 */
void stack_init(stack_node_t **stack);

/**
 * @brief adds/pushes token on stack 
 * 
 * @param stack the stack on which the token is to be pushed
 * @param token the token to be pushed 
 */
int stack_push(stack_node_t **stack, token_t *token);

/**
 * @brief removes/pops token of stack 
 * 
 * @param stack the stack from which to pop
 * @param return_token pointer to popped token, if the list is empty NULL
 */
void stack_pop(stack_node_t **stack, token_t *return_token);

/**
 * @brief returns the first node in stack without it being removed
 * 
 * @param stack the stack to from which the first node will be 
 * @param return_token pointer to the first token in stack 
 */
void stack_top(stack_node_t **stack, token_t *return_token);

/**
 * @brief disposes of stack leaves the tokens be
 * 
 * @param stack the stack to dispose off
 */
void stack_dispose(stack_node_t **stack);

/**
 * @brief disposes of stack and the tokens in it 
 * 
 * @param stack the stack to be disposed
 */
void stack_dispose_all(stack_node_t **stack);
#endif