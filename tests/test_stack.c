#include <stdlib.h>
#include <stdio.h>
#include "../stack.h"

int main() {
    printf("--- [STACK TESTS ---\n");
    stack_node_t **stack;

    //test inicialization
    printf("\n[stack inicialization test]\n");
    stack_init(stack);
    if(*stack == NULL){
        printf("stack initialized succesfully\n");
    } else{
        printf("stack failed to initialize\n");
    }

    //test is_emptu
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
    printf("First token inserted line %u \n", (*stack)->current->line);
    printf("First token inserted type %d \n", (*stack)->current->type);
    if((*stack)->next == NULL){
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
    printf("Second token inserted line %u \n", (*stack)->current->line);
    printf("Second token inserted type %d \n", (*stack)->current->type);
    if((*stack)->next != NULL){
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
    printf("Third token inserted line %u \n", (*stack)->current->line);
    printf("Third token inserted type %d \n", (*stack)->current->type);
    if((*stack)->next != NULL){
        printf("Third node next != NULL \n");
    }

    // test stack is not empty 
    printf("\n[stack is not empty test]\n");
    if(!stack_is_empty(stack)){
        printf("stack is not empty \n");
    }

    // top and pop
    printf("\n[stack top and pop tests]\n");
    token_t *returning_value;
    stack_top(stack, returning_value);
    printf("Current top token line %u \n", returning_value->line);
    printf("Current top token type %d \n", returning_value->type);

    stack_pop(stack);
    stack_top(stack, returning_value);
    printf("Current top token line %u \n", returning_value->line);
    printf("Current top token type %d \n", returning_value->type);

    stack_pop(stack);
    stack_top(stack, returning_value);
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

    stack_dispose_all(stack);
    if(token4 == NULL && token5 == NULL && token5 == NULL && stack == NULL){
        printf("stack dispose all succesfull \n");
    }

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
    if(token4 != NULL && token5 != NULL && token5 != NULL && stack == NULL){
        printf("stack dispose succesfull \n");
    }

    free(token7);
    free(token8);
    free(token9);
    return 0;
}
