#ifndef _STACK_H
#define _STACK_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

#define NO_SHIFT 42

/**
 * @struct stack node.
 */
typedef struct stack_node {
	token_t *current;
	struct stack_node *next;
} stack_node_t;

/**
 * @struct stack
 */
typedef struct{
	struct stack_node *top; 
}stack;

/**
 * @brief inicialize stack 
 * 
 * @param stack the stack to be inicialize
 */
void stack_init(stack *stack);

/**
 * @brief returns 1 if stack is empty 0 if it is not 
 * 
 * @param stack 
 * @return true 
 * @return false 
 */
bool stack_is_empty(stack *stack);

/**
 * @brief adds/pushes token on stack 
 * 
 * @param stack the stack on which the token is to be pushed
 * @param token the token to be pushed 
 */
void stack_push(stack *stack, token_t *token);

/**
 * @brief pops first token and returns it in return token, if the stack is empty returns NULL
 * 
 * @param stack stack from which to pop
 * @param return_token poped element is returned or NULL if empty stack
 */
token_t *stack_save_pop(stack *stack);

/**
 * @brief removes/pops token of stack, if the stack is empty does nothing 
 * 
 * @param stack the stack from which to pop
 */
void stack_pop(stack *stack);

/**
 * @brief returns the first node in stack without it being removed
 * 
 * @param stack the stack to from which the first node will be 
 * @param return_token pointer to the first token in stack 
 */
token_t *stack_top(stack *stack);

/**
 * @brief disposes of stack and the tokens in it 
 * 
 * @param stack the stack to be disposed
 */
void stack_dispose_all(stack *stack);

/**
 * @brief returns topmost terminal from stack(skipps e and shift).
 * 
 * @param stack stack to be searched
 * @return token_t* terminal of NULL if no terminal found on stack or found < or E first
 */
token_t *stack_top_terminal(stack *stack);

/**
 * @brief Push '<' shift to stack. If the on top of stack is token_expr_e, insert it after it instead.
 * 
 * @param stack the stack in which to insert to
 */
void stack_insert_shift(stack *stack);

// TODO comment
unsigned tokens_to_shift(stack *stack);

/**
 * @struct linked list structure for expression code generation
 */
typedef struct {
    int rule;
    token_t *ptok;
    exprll *next;
} exprll;

/**
 * @brief Initialises linked list.
 * 
 * @param ll pointer to linked list
 */
void exprll_init(exprll **ll);

/**
 * @brief Adds new element on start of given linked list
 * 
 * @param ll pointer to linked list
 * @param data integer, will be put in new ll element
 * @param ptok pointer to allocated token if data == 0, is NULL otherwise.
 * @return false in case of malloc error, true otherwise
 */
bool exprll_add(exprll *ll, int data, token_t* ptok);

/**
 * @brief Deletes given linked list
 * 
 * @param ll pointer to linked list, will be set to NULL
 */
void expll_dispose(exprll *ll);

#endif