#include <stdio.h>
#include <stdlib.h>

#include "../src/scanner.h"
#include "../src/ll.h"
#include "../src/error.h"

int ret = 0;

int main(){
    printf("--- [LINKED LIST TEST] ---\n");
    list *list = malloc(sizeof(list));
    if(list == NULL){
        error_handle(0, compiler_error);
        return 1;
    }

    // ll initialization 
    printf("\n[list init test]\n");
    list_init(list);
    if(list->end == NULL){
        printf("initialization succesful \n");
    }
    // ll empty 
    printf("\n[list is empty test]\n");
    if(list_is_empty(list)){
        printf("list is empty\n");
    }
    // ll add node 
    printf("\n[list add test]\n");
    list_node_t *node0, *node1, *node2, *node3, *node4, *node5;
    list_add(list, token_varieble, "hello");
    if(list->end->type == token_varieble && list->end->next == NULL && dynstr_compare(list->end->id, "hello")){
        printf("first node added succesfully");
    }
    //list_add(list, token_string, "there");


    return ret;
}