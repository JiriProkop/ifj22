#include <stdlib.h>
#include <stdio.h>
#include "../src/stack.h"
#include "../src/error.h"
#include "../src/scanner.h"
#include "../src/dynstr.h"


#define MALLOC_ERROR 99

// pushes token and fills token without token_att, it is not required to test stack
void push_token(stack *stack, token_t *token, token_type type, unsigned line){
    token = malloc(sizeof(token_t));
    // malloc structures
    if(token == NULL){
        printf("token failed to malloc");
        return;
    }
    // fill in token
    token->type = type;
    token->line = line;
    token->attr.str = NULL;
    // push token 
    if(stack_push(stack, token) == MALLOC_ERROR){
        printf("failed to push token");
    };
}

void print_type(token_t *token){
    if(token == NULL){
        printf("token either not terminal or searched stack was empty or E or < (shift) \n");
        return;
    }
    switch (token->type){
    case token_none:
        printf("token type none\n");
        break;
    case token_division:
        printf("token type division\n");
        break;
    case token_string:
        printf("token type string\n");
        break;
    case token_varieble:
        printf("token type varialbe\n");
        break;
    case token_identifier:
        printf("token type identifier\n");
        break;
    case token_keyword_w_null:
        print("token type keyword_w_null\n");
        break;
    case token_integer:
        printf("token type integer\n");
        break;
    case token_float:
        printf("token type float\n");
        break;
    case token_greater:
        printf("token type greater\n");
        break;
    case token_greater_equal:
        printf("token type greater equal\n");
        break;
    case token_lower:
        printf("token lower\n");
        break;
    case token_lower_equal:
        printf("token type lower equal\n");
        break;
    case token_assign:
        printf("token type assign\n");
        break;
    case token_compare:
        printf("token type compare\n");
        break;
    case token_compare_neg:
        printf("token type compare neg\n");
        break;
    case token_semicol:
        printf("token type semicol\n");
        break;
    case token_dot:
        printf("token type  dot\n");
        break;
    case token_colon:
        printf("token type colon\n");
        break;
    case token_curly_left:
        printf("token type curly left\n");
        break;
    case token_curly_right:
        printf("token type curly right\n");
        break;
    case token_parentheses_right:
        printf("token type parentheses right\n");
        break;
    case token_parentheses_left:
        print_type("token type parentheses left\n");
        break;
    case token_multiply:
        printf("token type multiply");
        break;
    case token_plus:
        printf("token type plus\n");
        break;
    case token_minus:
        printf("token type minus");
        break;
    case token_keyword:
        printf("token type keyword");
        break;
    case token_comma:
        print("token type comma");
        break;
    default:
        printf("undefined behavior in print_type \n");
        break;
    }
}


int main() {
    printf("--- [STACK TESTS ---\n");
    stack *stack = malloc(sizeof(stack));
    if(stack == NULL){
        printf("failed to malloc stack\n");
        return 1;
    }

    //test inicialization
    printf("\n[stack inicialization test]\n");
    stack_init(stack);
    if(stack->top == NULL){
        printf("stack initialized succesfully\n");
    } else{
        printf("stack failed to initialize\n");
        return 1;
    }

    //test is_empty
    printf("\n[stack is_empty test]\n");
    if(stack_is_empty(stack)){
        printf("stack is empty \n");
    }

    //test push 
    printf("\n[stack push test]\n");
    token_t *token0 = malloc(sizeof(token_t));
    if(token0 == NULL){
        printf("token0 malloc fail\n");
    }
    token0->line = 50;
    token0->type = token_none;
    if(stack_push(stack, token0) == MALLOC_ERROR){
        printf("failed to malloc token 0 stack node\n");
    }
    printf("First token inserted line %u \n", (stack->top->current->line));
    printf("First token inserted type %d \n", (stack->top->current->type));
    if(stack->top->next == NULL){
        printf("First node next == NULL \n");
    }


    token_t *token1 = malloc(sizeof(token_t));
    if(token1 == NULL){
        printf("token0 malloc fail\n");
    }
    token1->line = 25;
    token1->type = token_division;
    if(stack_push(stack, token1) == MALLOC_ERROR){
        printf("failed to malloc token 1 stack node\n");
    }
    printf("Second token inserted line %u \n", stack->top->current->line);
    printf("Second token inserted type %d \n", stack->top->current->type);
    if(stack->top->next != NULL){
        printf("Second node next != NULL \n");
    }

    token_t *token2 = malloc(sizeof(token_t));
    if(token2 == NULL){
        printf("token0 malloc fail\n");
    }
    token2->line = 72;
    token2->type = token_identifier;
    if(stack_push(stack, token2) == MALLOC_ERROR){
        printf("failed to malloc token 2 stack node\n");
    }


    // test stack is not empty 
    printf("\n[stack is not empty test]\n");
    if(!stack_is_empty(stack)){
        printf("stack is not empty \n");
    }

    // top and pop
    printf("\n[stack top and pop tests]\n");
    token_t *returning_value;
    returning_value = stack_top(stack);
    printf("Current top token line %u \n", returning_value->line);
    printf("Current top token type %d \n", returning_value->type);

    stack_pop(stack);
    returning_value = stack_top(stack);
    printf("Current top token line %u \n", returning_value->line);
    printf("Current top token type %d \n", returning_value->type);

    stack_pop(stack);
    returning_value = stack_top(stack);
    printf("Current top token line %u \n", returning_value->line);
    printf("Current top token type %d \n", returning_value->type);

    if(stack_is_empty(stack)){
        printf("stack is empty \n");
    }
    stack_pop(stack);

    // test dispose all
    printf("\n[stack dispose all test]\n");
    token_t *token4 = malloc(sizeof(token_t));
    if(token4 == NULL){
        printf("token4 malloc fail\n");
    }
    token4->line = 50;
    token4->type = token_none;
    if(stack_push(stack, token4) == MALLOC_ERROR){
        printf("failed to malloc token 4 stack node\n");
    }

    token_t *token5 = malloc(sizeof(token_t));
    if(token5 == NULL){
        printf("token5 malloc fail\n");
    }
    token5->line = 25;
    token5->type = token_division;
    if(stack_push(stack, token5) == MALLOC_ERROR){
        printf("failed to malloc token 5 stack node\n");
    }

    token_t *token6 = malloc(sizeof(token_t));
    if(token6 == NULL){
        printf("token6 malloc fail\n");
    }
    token6->line = 72;
    token6->type = token_identifier;
    if(stack_push(stack, token6) == MALLOC_ERROR){
        printf("failed to malloc token 6 stack node\n");
    }
    dynstr_t *dispose_str = malloc(sizeof(dynstr_t));
    dynstr_init(dispose_str);
    dynstr_add_char(dispose_str, 'f');
    token6->attr.str = dispose_str;

    stack_dispose_all(stack);
    printf("After stack_dispose_all use valgrind I know not how to check \n");

    //test stack dispose 
    printf("\n[stack dispose test]\n");
    token_t *token7 = malloc(sizeof(token_t));
    if(token7 == NULL){
        printf("token7 malloc fail\n");
    }
    token7->line = 50;
    token7->type = token_none;
    if(stack_push(stack, token7) == MALLOC_ERROR){
        printf("failed to malloc token 7 stack node\n");
    }

    token_t *token8 = malloc(sizeof(token_t));
    if(token8 == NULL){
        printf("token8 malloc fail\n");
    }
    token8->line = 25;
    token8->type = token_division;
    if(stack_push(stack, token8) == MALLOC_ERROR){
        printf("failed to malloc token 8 stack node\n");
    }

    token_t *token9 = malloc(sizeof(token_t));
    if(token9 == NULL){
        printf("token9 malloc fail\n");
    }
    token9->line = 72;
    token9->type = token_identifier;
    if(stack_push(stack, token9) == MALLOC_ERROR){
        printf("failed to malloc token 9 stack node\n");
    }

    stack_dispose(stack);
    if(token4 != NULL && token5 != NULL && token5 != NULL && stack->top == NULL){
        printf("stack dispose succesfull \n");
    }

    free(token7);
    token7 = NULL;
    free(token8);
    token8 = NULL;
    free(token9);
    token9 = NULL;
    // test stack top terminal
    printf("\n[stack top terminal]\n");
    token_t *top_terminal;
    // test no terminal
    push_token(stack, token0, token_parentheses_left, 4);
    push_token(stack, token1, token_minus, 5);
    top_terminal = stack_top_terminal(stack);
    print_type(top_terminal);
    // test integer terminal
    push_token(stack, token2, token_integer, 6);
    push_token(stack, token4, token_plus, 7);
    top_terminal = stack_top_terminal(stack);
    print_type(top_terminal);
    // test string terminal 
    push_token(stack, token5, token_string, 7);
    top_terminal = stack_top_terminal(stack);
    print_type(top_terminal);
    // test float
    push_token(stack, token7, token_float, 12);
    push_token(stack, token6, token_compare_neg, 8);
    top_terminal = stack_top_terminal(stack);
    print_type(top_terminal);
    // test variable
    push_token(stack,token8, token_varieble, 60);
    top_terminal = stack_top_terminal(stack);
    print_type(top_terminal);
    stack_dispose_all(stack);
    // test finds < (shift)
    push_token(stack, token0, token_varieble, 10);
    push_token(stack, token1, token_expr_shift, 12);
    push_token(stack, token2, token_multiply, 40);
    top_terminal = stack_top_terminal(stack);
    print_type(top_terminal);
    free(top_terminal);
    // test finds E 
    push_token(stack, token4, token_integer, 67);
    push_token(stack, token5, token_expr_e, 42);
    top_terminal = stack_top_terminal(stack);
    print_type(top_terminal);
    free(top_terminal);
    // pop and save
    printf("\n[stack test pop and save]\n");
    push_token(stack, token6, token_compare, 32);
    top_terminal = stack_save_pop(stack);
    if(top_terminal->type == token_compare){
        printf("stack_save_pop working\n");
    }
    free(top_terminal);
    // insert shift 
    printf("\n[stack test insert shift]\n");
    stack_insert_shift(stack);
    stack_pop(stack);
    top_terminal = stack_save_pop(stack);
    if(top_terminal->type == token_expr_shift){
        printf("stack insert shift working\n");
    }
    free(top_terminal);
    stack_dispose_all(stack);
    // tokens to shift
    push_token(stack, token0, token_expr_shift, 12);
    push_token(stack, token1, token_compare_neg, 77);
    push_token(stack, token2, token_curly_left, 76);
    if(tokens_to_shift(stack) == 2){
        printf("tokens to shift works");
    }
    stack_dispose_all(stack);
    free(stack);
    printf("\n[end stack test]\n");
    return ret;
}

