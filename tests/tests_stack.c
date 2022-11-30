#include <stdlib.h>
#include <stdio.h>
#include "../src/stack.h"
#include "../src/error.h"
#include "../src/scanner.h"
#include "../src/dynstr.h"


// pushes token and fills token without token_att, it is not required to test stack
void push_token(stack *stack, token_type type, unsigned line){
    token_t *token = malloc(sizeof(token_t));
    if(token == NULL){
        printf("token failed to malloc");
        return;
    }
    // fill in token
    token->type = type;
    token->line = line;
    token->attr.str = NULL;
    // push token 
    stack_push(stack, token);
}

void fill_stack(stack* stack){
    if(!stack_is_empty(stack)){
        printf("filling not empty stack \n");
    }
    int size = 6;
    unsigned lines[6] = {1,2,3,4,5,6};
    token_type types[6] ={token_parentheses_left, token_minus, token_integer, token_plus, token_string, token_identifier};
    for(int i = 0; i< size; i++){
        push_token(stack, types[i], lines[i]);
    }
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
        printf("token type keyword_w_null\n");
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
        printf("token type parentheses left\n");
        break;
    case token_multiply:
        printf("token type multiply\n");
        break;
    case token_plus:
        printf("token type plus\n");
        break;
    case token_minus:
        printf("token type minus\n");
        break;
    case token_keyword:
        printf("token type keyword\n");
        break;
    case token_comma:
        printf("token type comma\n");
        break;
    default:
        printf("undefined behavior in print_type \n");
        break;
    }
}


int main() {
    printf("--- [STACK TESTS] ---\n");
    stack *stack = malloc(sizeof(stack));
    if(stack == NULL){
        printf("failed to malloc stack\n");
        return 1;
    }

    //----test inicialization----
    printf("\n[stack inicialization test]\n");
    stack_init(stack);
    if(stack->top == NULL){
        printf("stack initialized succesfully\n");
    } else{
        printf("stack failed to initialize\n");
        return 1;
    }

    //----test is_empty----
    printf("\n[stack is_empty test]\n");
    if(stack_is_empty(stack)){
        printf("stack is empty \n");
    }

    //----test push----
    printf("\n[stack push test]\n");
    printf("pushing first token : type none, line 50\n");
    push_token(stack, token_none, 50);
    print_type((stack->top->current));
    printf("first token inserted line %u \n",(stack->top->current->line));
    if(stack->top->next == NULL){
        printf("only node (next == NULL) \n");
    }

    printf("inserting second token: type division, line 25\n");
    push_token(stack, token_division, 25);
    print_type(stack->top->current);
    printf("second token inserted line %u \n",(stack->top->current->line));
    if(stack->top->next != NULL){
        printf("test if second node points to first node \n");
        printf("pointed node line %u \n",(stack->top->current->line));
        print_type(stack->top->next->current);
    } else{
        printf("connectiong of the stack failed");
    }

    printf("inserting third token: type token identifier, line 72\n");
    push_token(stack, token_identifier, 72);
    print_type(stack->top->current);
    printf("third token inserted line %u \n", stack->top->current->line);


    // ----test stack is not empty----
    printf("\n[stack is not empty test]\n");
    if(!stack_is_empty(stack)){
        printf("stack is not empty \n");
    }

    // ----top and pop----
    printf("\n[stack top and pop tests]\n");
    token_t *returning_value;
    returning_value = stack_top(stack);
    printf("current top token line %u \n", returning_value->line);
    print_type(returning_value);

    printf("stack pop 0\n");
    stack_pop(stack);
    returning_value = stack_top(stack);
    printf("Current top token line %u \n", returning_value->line);
    print_type(returning_value);

    printf("stack pop 1\n");
    stack_pop(stack);
    returning_value = stack_top(stack);
    printf("Current top token line %u \n", returning_value->line);
    print_type(returning_value);

    printf("stack should not be empty \n");
    if(stack_is_empty(stack)){
        printf("stack is empty \n");
    }else{
        printf("stack is not empty \n");
    }
    printf("stack pop 3 \n");
    stack_pop(stack);

    printf("stack should be empty \n");
    if(stack_is_empty(stack)){
        printf("stack is empty \n");
    }else{
        printf("stack is not empty \n");
    }
    printf("stack pop 4 \n");


    // ----test dispose all----
    printf("\n[stack dispose all test]\n");
    fill_stack(stack);

    dynstr_t *dispose_str = malloc(sizeof(dynstr_t));
    dynstr_init(dispose_str);
    dynstr_add_char(dispose_str, 'f');
    stack->top->current->attr.str = dispose_str;

    stack_dispose_all(stack);
    printf("No segfault, use valgrind for details\n");
    
    token_t *top_terminal;
    fill_stack(stack);

    // ---- test pop and save ----
    printf("\n[stack test pop and save]\n");
    push_token(stack, token_compare, 32);
    top_terminal = stack_save_pop(stack);
    if(top_terminal->type == token_compare){
        printf("stack_save_pop working\n");
    }
    free(top_terminal);

    // ---- test insert shift ----
    printf("\n[stack test insert shift]\n");
    stack_insert_shift(stack);
    stack_pop(stack);
    top_terminal = stack_save_pop(stack);
    if(top_terminal->type == token_expr_shift){
        printf("stack insert shift working\n");
    }
    free(top_terminal);

    stack_dispose_all(stack);
    // ---- test tokens to shift ----
    printf("\n[stack test tokens to shift]\n");
    push_token(stack, token_expr_shift, 12);
    push_token(stack, token_compare_neg, 77);
    push_token(stack, token_curly_left, 76);
    if(tokens_to_shift(stack) == 2){
        printf("tokens to shift works");
    }
    stack_dispose_all(stack);
    free(stack);
    printf("\n[end stack test]\n");
    return ret;
}

