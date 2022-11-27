#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/scanner.h"
#include "../src/ll.h"
#include "../src/error.h"

void print_add_node(list_t *list, token_type type, bool first, char id[]){
    char *str = malloc(sizeof(char)* (strlen(id)+1));
    str = strcpy(str, id);
    if(list == NULL){
        return;
    }

    if(list->last->type != type){
        printf("wrong type \n");
        return;
    }
    if(!dynstr_compare(list->last->id, str)){
        printf("wrong id \n");
        return;
    }
    if(first && list->last->next != NULL){
        printf("should be first or last but has nexx != NULL \n");
        return;
    }
    if((!first) && list->first->next == NULL){
        printf("should not be last but next == NULL \n");
        return;
    }
    printf("added succesfully \n");
    free(str);
}

void print_list(list_t *list){
    list_node_t *i = list->first; 
    printf("list from the start : ");
    while(i != NULL){
        printf(" '%s',", i->id->array);
        i = i->next;
    }
    printf("\n");
}

void add_node(list_t *list, char id[], token_type type){
    char* tmp_str = malloc(sizeof(char)*(strlen(id) + 1));
    if(tmp_str == NULL){
        printf("failed to malloc dynstr in add node tests_ll");
    }
    dynstr_t *tmp_dstr = malloc(sizeof(dynstr_t));
    dynstr_init(tmp_dstr);
    tmp_str = strcpy(tmp_str, id);
    dynstr_add_string(tmp_dstr, tmp_str);
    list_add(list, type, tmp_dstr);
    free(tmp_str);
}

int ret = 0;

int main(){
    printf("--- [LINKED LIST TEST] ---\n");
    list_t *list = malloc(sizeof(list_t));
    if(list == NULL){
        error_handle(0, compiler_error);
        return 1;
    }

    // ll initialization 
    printf("\n[list init test]\n");
    list_init(list);
    if(list->last == NULL){
        printf("initialization succesful \n");
    }
    // ll empty 
    printf("\n[list is empty test]\n");
    if(list_is_empty(list)){
        printf("list is empty\n");
    }
    // ll add node foo (int hello , char there , bool Obi-wan);
    printf("\n[list add test]\n");

    add_node(list, "hello", token_varieble);
    print_add_node(list, token_varieble, true, "hello");

    add_node(list, "there", token_string);
    print_add_node(list, token_string, false, "there");

    add_node(list, "Obi-wan", token_dot);
    print_add_node(list,token_dot, false, "Obi-wan");

    print_list(list);
    // ll list not empty 
    printf("\n[list not empty test]\n");
    if(!list_is_empty(list)){
        printf("list is not empty \n");
    }

    // test delete all (use valgrind)
    list_dispose(list);
    // test list delete_first
    list = malloc(sizeof(list_t));
    

    printf("--- [LINKED LIST END TEST] ---\n");
    return ret;
}


