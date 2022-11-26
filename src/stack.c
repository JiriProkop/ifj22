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
#include "error.h"


void stack_init(stack *stack){
	stack->top = NULL;
}

bool stack_is_empty(stack *stack){
	return stack->top == NULL;
}

int stack_push(stack*stack, token_t *token){
	stack_node_t *tmp = malloc(sizeof(stack_node_t));
	if(tmp == NULL){
		return MALLOC_ERROR;
	}
	if(stack == NULL){
		tmp->next = NULL;
	} else{
		tmp->next = stack->top;
	}
	tmp->current = token;
	stack->top = tmp;
	return 0;
}

void stack_pop(stack *stack){
	token_t *tmp;
	stack_node_t *tmp1;
	if(stack->top == NULL){
		return;
	}
	tmp = stack->top->current;
	tmp1 = stack->top;
	stack->top = stack->top->next;
	free(tmp);
	tmp = NULL;
	free(tmp1);
	tmp1 = NULL;
}

token_t *stack_top(stack *stack){
	if(stack->top == NULL){
		return NULL;
	}
	return stack->top->current;
}

void stack_dispose(stack *stack){
	stack_node_t *tmp ;
	while(stack->top != NULL){
		tmp = stack->top;
		stack->top = stack->top->next;
		free(tmp);
	}
}

void stack_dispose_all(stack *stack){
	token_t *tmp;
	stack_node_t *tmp1;
	while(stack->top != NULL){
		tmp = stack->top->current;
		tmp1 = stack->top;
		stack->top = stack->top->next;
		free(tmp);
		tmp = NULL;
		free(tmp1);
		tmp1 = NULL;
	}
}

token_t *stack_top_terminal(stack *stack){
	if(stack == NULL){
		return NULL;
	}

	stack_node_t *i;
	i = stack->top;
	while(i != NULL &&
		i->current->type != token_integer &&
		i->current->type != token_float && 
		i->current->type != token_string &&
		i->current->type != token_varieble){
		i = i->next;
	}
	if (i == NULL){
		return NULL;
	}
	return i->current;
}

// && tmp->current->type != token_integer && tmp->current->type != token_float && tmp->current->type != token_varieble && tmp->next != NULL)