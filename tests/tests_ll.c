#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/scanner.h"
#include "../src/ll.h"
#include "../src/error.h"

void print_same_node(list *list, token_type type, bool first, char id[]){
    char *str = malloc(sizeof(strlen(id)));
    strcpy(str, id);
    if(list->end->type != type){
        printf("wrong type \n");
        return;
    }
    if(!dynstr_compare(list->end->id, str)){
        printf("wrong id \n");
    }
    if(first && list->end->next != NULL){
        printf("should be first or last but has nexx != NULL \n");
        return;
    }
    if((!first) && list->end->next == NULL){
        printf("should not be last but next == NULL \n");
    }
    printf("succesful \n");
}

void print_list(list *list){
    
}

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
    // ll add node foo (int hello , char there , bool Obi-wan);
    printf("\n[list add test]\n");
    dynstr_t *added_dynstr = malloc(sizeof(dynstr_t));

    dynstr_add_string(added_dynstr, "hello");
    list_add(list, token_varieble, added_dynstr);
    print_same_node(list, token_varieble, true, "hello");
    dynstr_clear(added_dynstr);

    dynstr_add_string(added_dynstr,"there");
    list_add(list, token_string, added_dynstr);
    print_same_node(list, token_string, false, "there");
    dynstr_clear(added_dynstr);

    dynstr_add_string(added_dynstr, "Obi-wan");
    list_add(list, token_dot, added_dynstr);
    print_same_node(list,token_dot, false, "Obi-wan");
    dynstr_clear(added_dynstr);

    // ll list not empty 
    printf("\n[list not empty test]\n");
    if(!list_is_empty(list)){
        printf("list is not empty \n");
    }

    // 
    string_free(added_dynstr);
    printf("--- [LINKED LIST END TEST] ---\n");
    return ret;
}


