/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief  Implementation of stack
 *
 * @author Štěpán Czajkowski xczajk01 <xczajk01@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "stack.h"
#include "error.h"
#include "dynstr.h"
#include "parser.h"

/**
 * @brief Function for initialization of stack
 * 
 * @param stack The stack to be inicialize
 */
void stack_init(stack *stack){
	stack->top = NULL;
}

/**
 * @brief Function for founding out if stack is empty 
 * 
 * @param stack the stack to be tested
 * @return true stack is empty 
 * @return false stack is not empty 
 */
bool stack_is_empty(stack *stack){
	return stack->top == NULL;
}

/**
 * @brief Function that pushes node on stack 
 * 
 * @param stack the stack on which the node is to be pushed
 * @param token the token which to be pushed 
 */
void stack_push(stack*stack, token_t *token){
	stack_node_t *tmp = malloc(sizeof(stack_node_t));
	if(tmp == NULL){
		error_handle(0,compiler_error);
		stack_dispose_all(stack);
		abort();
    }
    if(stack == NULL){
		tmp->next = NULL;
	} else{
		tmp->next = stack->top;
	}
	tmp->current = token;
	stack->top = tmp;
}

/**
 * @brief Function that pops first node and returns token in the node, if the stack is empty returns NULL
 * 
 * @param stack stack from which to pop
 * @param return_token poped token is returned or NULL if empty stack
 */
token_t *stack_save_pop(stack *stack){
	stack_node_t *tmp_node;
	token_t *tmp_token;
	if(stack->top == NULL){
		return NULL;
	}
	tmp_node = stack->top;
	tmp_token = stack->top->current;
	stack->top = stack->top->next;
	free(tmp_node);
	tmp_node = NULL;
	return tmp_token;
}

/**
 * @brief Function that pops token from stack, if the stack is empty does nothing 
 * 
 * @param stack the stack from which to pop
 */
void stack_pop(stack *stack){
	token_t *tmp_token;
	stack_node_t *tmp_node;
	if(stack->top == NULL){
		return;
	}
	tmp_token = stack->top->current;
	tmp_node = stack->top;
	stack->top = stack->top->next;
	if(tmp_token->type == token_identifier || tmp_token->type == token_string){
		if(tmp_token->attr.str != NULL){
			string_free(tmp_token->attr.str);
		}
	}
	free(tmp_token);
	tmp_token = NULL;
	free(tmp_node);
	tmp_node = NULL;
}

/**
 * @brief Function that returns the first token in stack without removing it 
 * 
 * @param stack the stack to from which the first node will be 
 * @param return_token pointer to the first token in stack 
 */
token_t *stack_top(stack *stack){
	if(stack->top == NULL){
		return NULL;
	}
	return stack->top->current;
}

/**
 * @brief Function that frees the stack and the tokens in it 
 * 
 * @param stack the stack to be disposed
 */
void stack_dispose_all(stack *stack){
	token_t *tmp_token;
	stack_node_t *tmp_node;
	while(stack->top != NULL){
		tmp_token = stack->top->current;
		tmp_node = stack->top;
		stack->top = stack->top->next;
		if(tmp_token->type == token_identifier || tmp_token->type == token_string){
			if(tmp_token->attr.str != NULL){
				string_free(tmp_token->attr.str);
			}
		}
		free(tmp_token);
		tmp_token = NULL;
		free(tmp_node);
		tmp_node = NULL;
	}
}

/**
 * @brief Function that returns topmost token after all shifts(<) and E on top of the stack 
 * 
 * @param stack the stack to be searched
 * @return token_t* top terminal 
 */
token_t *stack_top_terminal(stack *stack){
    stack_node_t *i = stack->top;
    while (i != NULL) {
		if(i->current->type == token_expr_shift || i->current->type == token_expr_e) {
        	i = i->next;
		} else {
            return i->current;
            }
        }
	if (i == NULL) {
    	error_handle(0, compiler_error);
    	abort();
	}
    return i->current;
}

/**
 * @brief Function thath pushes shift (<) to stack. If the on top of stack is token_expr_e, insert it after it instead.
 * 
 * @param stack the stack on which to push 
 */
void stack_insert_shift(stack *stack){
	if(stack == NULL){
		return;
	}

	stack_node_t *i;
	i = stack->top;
	if(i->current->type != token_expr_e) {
		token_t *tok = malloc(sizeof(token_t));
		if(tok == NULL) {
			error_handle(0, compiler_error);
	    	stack_dispose_all(stack);
        	abort();	
		}
        tok->type = token_expr_shift;
        stack_push(stack, tok);
        return;
    }

    stack_node_t *new_node = malloc(sizeof(stack_node_t));
	if(new_node == NULL){
		error_handle(0, compiler_error);
	    stack_dispose_all(stack);
        abort();
        }
	token_t *new_token = malloc(sizeof(token_t));
	if(new_token == NULL){
		error_handle(0, compiler_error);
		stack_dispose_all(stack);
		abort();
	}

	new_node->next = i->next;
	new_node->current = new_token;
	new_token->type = token_expr_shift;
	i->next = new_node;
}

/**
 * @brief Function that retuns number tokens to nearest shift(<)
 * 
 * @param stack the stack to be searched 
 * @return unsigned number of tokens to nearest shift (<)
 */
unsigned tokens_to_shift(stack *stack){
	if(stack == NULL){
		return NO_SHIFT;
	}

	unsigned sum = 0;
	stack_node_t *i;
	i = stack->top;
	while(i != NULL &&
		i->current->type != token_expr_shift){
		i = i->next;
		sum ++;
	}
	if (i == NULL){
		return NO_SHIFT;
	}
	return sum;
}