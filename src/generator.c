#include <stdio.h>
#include "generator.h"
#include "dynstr.h"
#include "ll.h"

void gen_header() {
    printf(".IFJcode22\n");
}

void gen_function_def(dynstr_t *id, list_t* parameters) {
    printf("JUMP %s_end\n", id->array);
    list_t* i = parameters->first;
    while(i != NULL){
        printf("DEFVAR LF@%%s_%s", id->array, i->first->id->array);
        i = i->first->next;
    }
    printf("LABEL %s_start\n", id->array);
    printf("CREATEFRAME\n");
    i = parameters->first;
    while(i != NULL){
        printf("DEFVAR TF@s", i->first->id->array);
        printf("MOVE TF@%s LF@%%s_%s",i->first->id->array, id->array, i->first->id->array);
    }
    printf("PUSHFRAME\n");
}

void gen_function_defend(dynstr_t *id) {
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL %s_end\n", id->array);
}

void gen_function_call(dynstr_t *id, list_t* parameters){
    
}