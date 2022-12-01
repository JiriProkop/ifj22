#include <stdio.h>
#include "generator.h"
#include "dynstr.h"
#include "ll.h"

void gen_header() {
    printf(".IFJcode22\n");
}

void gen_function_def(dynstr_t *id, list_t* parameters) {
    list_node_t* i = parameters->first;
    while(i != NULL){
        printf("DEFVAR LF@%%%s_%s\n", id->array, i->id->array);
        i = i->next;
    }
    printf("JUMP %s_end\n", id->array);
    printf("LABEL %s_start\n", id->array);
    printf("CREATEFRAME\n");
    i = parameters->first;
    while(i != NULL){
        printf("DEFVAR TF@%s\n", i->id->array);
        printf("MOVE TF@%s LF@%%%s_%s\n",i->id->array, id->array, i->id->array);
        i = i->next;
    }
    printf("PUSHFRAME\n");
}

void gen_function_def_end(dynstr_t *id) {
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL %s_end\n", id->array);
}

void gen_function_call(dynstr_t *id, list_t* parameters){
    
}