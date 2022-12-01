/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Auxiliary data structures for expression parsing and code generation
 *
 * @author Štěpán Czajkowski xczajk01 <xczajk01@stud.fit.vutbr.cz>
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>

#include "dynstr.h"
#include "error.h"
#include "parser.h"
#include "scanner.h"
#include "stack.h"

// stack created by Štěpán Czajkowski

void stack_init(stack *stack) {
    stack->top = NULL;
}

bool stack_is_empty(stack *stack) {
    return stack->top == NULL;
}

void stack_push(stack *stack, token_t *token) {
    stack_node_t *tmp = malloc(sizeof(stack_node_t));
    if (tmp == NULL) {
        error_handle(0, compiler_error);
        stack_dispose_all(stack);
        abort();
    }
    if (stack == NULL) {
        tmp->next = NULL;
    } else {
        tmp->next = stack->top;
    }
    tmp->current = token;
    stack->top = tmp;
}

token_t *stack_save_pop(stack *stack) {
    stack_node_t *tmp_node;
    token_t *tmp_token;
    if (stack->top == NULL) {
        return NULL;
    }
    tmp_node = stack->top;
    tmp_token = stack->top->current;
    stack->top = stack->top->next;
    free(tmp_node);
    tmp_node = NULL;
    return tmp_token;
}

void stack_pop(stack *stack) {
    token_t *tmp_token;
    stack_node_t *tmp_node;
    if (stack->top == NULL) {
        return;
    }
    tmp_token = stack->top->current;
    tmp_node = stack->top;
    stack->top = stack->top->next;
    if (tmp_token->type == token_identifier || tmp_token->type == token_string) {
        if (tmp_token->attr.str != NULL) {
            string_free(tmp_token->attr.str);
        }
    }
    free(tmp_token);
    tmp_token = NULL;
    free(tmp_node);
    tmp_node = NULL;
}

token_t *stack_top(stack *stack) {
    if (stack->top == NULL) {
        return NULL;
    }
    return stack->top->current;
}

void stack_dispose_all(stack *stack) {
    token_t *tmp_token;
    stack_node_t *tmp_node;
    while (stack->top != NULL) {
        tmp_token = stack->top->current;
        tmp_node = stack->top;
        stack->top = stack->top->next;
        if (tmp_token->type == token_identifier || tmp_token->type == token_string) {
            if (tmp_token->attr.str != NULL) {
                string_free(tmp_token->attr.str);
            }
        }
        free(tmp_token);
        tmp_token = NULL;
        free(tmp_node);
        tmp_node = NULL;
    }
}

token_t *stack_top_terminal(stack *stack) {
    stack_node_t *i = stack->top;
    while (i != NULL) {
        if (i->current->type == token_expr_shift || i->current->type == token_expr_e) {
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

void stack_insert_shift(stack *stack) {
    if (stack == NULL) {
        return;
    }

    stack_node_t *i;
    i = stack->top;
    if (i->current->type != token_expr_e) {
        token_t *tok = malloc(sizeof(token_t));
        if (tok == NULL) {
            error_handle(0, compiler_error);
            stack_dispose_all(stack);
            abort();
        }
        tok->type = token_expr_shift;
        stack_push(stack, tok);
        return;
    }

    stack_node_t *new_node = malloc(sizeof(stack_node_t));
    if (new_node == NULL) {
        error_handle(0, compiler_error);
        stack_dispose_all(stack);
        abort();
    }
    token_t *new_token = malloc(sizeof(token_t));
    if (new_token == NULL) {
        error_handle(0, compiler_error);
        stack_dispose_all(stack);
        abort();
    }

    new_node->next = i->next;
    new_node->current = new_token;
    new_token->type = token_expr_shift;
    i->next = new_node;
}

unsigned tokens_to_shift(stack *stack) {
    if (stack == NULL) {
        return NO_SHIFT;
    }

    unsigned sum = 0;
    stack_node_t *i;
    i = stack->top;
    while (i != NULL &&
           i->current->type != token_expr_shift) {
        i = i->next;
        sum++;
    }
    if (i == NULL) {
        return NO_SHIFT;
    }
    return sum;
}

// -----------------------------------------------------------------
// linked list created by Jiří Prokop

void exprll_init(exprll **ll) {
    *ll = NULL;
}

bool exprll_add(exprll *ll, int data, token_t *ptok) {
    if (!ll) {
        ll = malloc(sizeof(exprll));
        if (!ll) {
            error_handle(0, compiler_error);
            return false;
        }
        ll->next = NULL;
        ll->rule = data;
        return true;
    } else {
        exprll *tmp = ll;
        ll = malloc(sizeof(exprll));
        if (!ll) {
            error_handle(0, compiler_error);
            return false;
        }
        ll->next = tmp;
        ll->rule = data;
        return true;
    }
}

void expll_dispose(exprll *ll) {
    while (ll != NULL) {
        exprll *tmp = ll;
        ll = ll->next;
        free(tmp->ptok);
        free(tmp);
    }
}