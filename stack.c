/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief scanner implementation.
 *
 * @author Štěpán Czajkowski xczajk01 <xczajk01@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "stack.h"


void stack_init(stack *stack){
	stack->top = NULL;
}

// bool stack_is_empty(stack *stack){
// 	return stack->top == NULL;
// }

// int stack_push(stack*stack, token_t *token){
// 	stack_node_t *tmp = malloc(sizeof(stack_node_t));
// 	if(tmp == NULL){
// 		return MALLOC_ERROR;
// 	}
// 	if(stack == NULL){
// 		tmp->next = NULL;
// 	} else{
// 		tmp->next = stack->top;
// 	}
// 	tmp->current = token;
// 	stack->top = tmp;
// }

// void stack_pop(stack_node_t **stack){
// 	token_t *tmp;
// 	stack_node_t *tmp1;
// 	if(*stack == NULL){
// 		return;
// 	}
// 	tmp = (*stack)->current;
// 	tmp1 = *stack;
// 	(*stack) = (*stack)->next;
// 	free(tmp);
// 	tmp = NULL;
// 	free(tmp1);
// 	tmp1 = NULL;
// }

// void stack_top(stack_node_t **stack, token_t *return_token){
// 	if(*stack == NULL){
// 		return_token = NULL;
// 	} else{
// 		return_token = (*stack)->current;
// 	}
// }

// void stack_dispose(stack_node_t **stack){
// 	stack_node_t *tmp ;
// 	while(*stack != NULL){
// 		tmp = *stack;
// 		*stack = (*stack)->next;
// 		free(tmp);
// 	}
// }

// void stack_dispose_all(stack_node_t **stack){
// 	token_t *tmp;
// 	stack_node_t *tmp1;
// 	while(*stack != NULL){
// 		tmp = (*stack)->current;
// 		tmp1 = *stack;
// 		*stack = (*stack)->next;
// 		free(tmp);
// 		tmp = NULL;
// 		free(tmp1);
// 		tmp1 = NULL;
// 	}
// }
