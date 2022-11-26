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
int stack_push(stack *stack, token_t *token);

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
 * @brief disposes of stack leaves the tokens be
 * 
 * @param stack the stack to dispose off
 */
void stack_dispose(stack *stack);

/**
 * @brief disposes of stack and the tokens in it 
 * 
 * @param stack the stack to be disposed
 */
void stack_dispose_all(stack *stack);

/**
 * @brief returns topmost terminal from stack. if it finds < (shift) or E before terminal it deos othing
 * 
 * @param stack stack to be searched
 * @return token_t* terminal of NULL if no terminal found on stack or found < or E first
 */
token_t *stack_top_terminal(stack *stack);

/**
 * @brief inserts < (shift) before first E it finds
 * 
 * @param stack the stack in which to insert to
 */
void stack_insert_shift(stack *stack);


unsigned tokens_to_shift(stack *stack);
#endif