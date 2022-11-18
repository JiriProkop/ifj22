#ifndef _STACK_H
#define _STACK_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

#define MALLOC_ERROR 99

/**
 * @struct stack node.
 */
typedef struct stack_node {
    token_t *current;
    struct stack_node *next;
} stack_node_t;

/**
 * @brief inicialize stack 
 * 
 * @param stack the stack to be inicialize
 */
void stack_init(stack_node_t **stack);

/**
 * @brief returns 1 if stack is empty 0 if it is not 
 * 
 * @param stack 
 * @return true 
 * @return false 
 */
bool stack_is_empty(stack_node_t **stack);

/**
 * @brief adds/pushes token on stack 
 * 
 * @param stack the stack on which the token is to be pushed
 * @param token the token to be pushed 
 */
int stack_push(stack_node_t **stack, token_t *token);

/**
 * @brief removes/pops token of stack, if the stack is empty does nothing 
 * 
 * @param stack the stack from which to pop
 */
void stack_pop(stack_node_t **stack);

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