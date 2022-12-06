/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Header file of stack
 *
 * @author Štěpán Czajkowski xczajk01 <xczajk01@stud.fit.vutbr.cz>
 */

#ifndef _STACK_H
#define _STACK_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

#define NO_SHIFT 42

/**
 * @struct Stack node.
 */
typedef struct stack_node {
	token_t *current;
	struct stack_node *next;
} stack_node_t;

/**
 * @struct Stack
 */
typedef struct{
	struct stack_node *top; 
}stack;

/**
 * @brief Function for initialization of stack
 * 
 * @param stack The stack to be inicialize
 */
void stack_init(stack *stack);

/**
 * @brief Function for founding out if stack is empty 
 * 
 * @param stack the stack to be tested
 * @return true stack is empty 
 * @return false stack is not empty 
 */
bool stack_is_empty(stack *stack);

/**
 * @brief Function that pushes node on stack 
 * 
 * @param stack the stack on which the node is to be pushed
 * @param token the token which to be pushed 
 */
void stack_push(stack *stack, token_t *token);

/**
 * @brief Function that pops first node and returns token in the node, if the stack is empty returns NULL
 * 
 * @param stack stack from which to pop
 * @param return_token poped token is returned or NULL if empty stack
 */
token_t *stack_save_pop(stack *stack);

/**
 * @brief Function that pops token from stack, if the stack is empty does nothing 
 * 
 * @param stack the stack from which to pop
 */
void stack_pop(stack *stack);

/**
 * @brief Function that returns the first token in stack without removing it 
 * 
 * @param stack the stack to from which the first node will be 
 * @param return_token pointer to the first token in stack 
 */
token_t *stack_top(stack *stack);

/**
 * @brief Function that frees the stack and the tokens in it 
 * 
 * @param stack the stack to be disposed
 */
void stack_dispose_all(stack *stack);

/**
 * @brief Function that returns topmost token after all shifts(<) and E on top of the stack 
 * 
 * @param stack the stack to be searched
 * @return token_t* top terminal 
 */
token_t *stack_top_terminal(stack *stack);

/**
 * @brief Function thath pushes shift (<) to stack. If the on top of stack is token_expr_e, insert it after it instead.
 * 
 * @param stack the stack on which to push 
 */
void stack_insert_shift(stack *stack);

/**
 * @brief Function that retuns number tokens to nearest shift(<)
 * 
 * @param stack the stack to be searched 
 * @return unsigned number of tokens to nearest shift (<)
 */
unsigned tokens_to_shift(stack *stack);
#endif