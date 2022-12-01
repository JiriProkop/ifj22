#include <stdio.h>
#include "generator.h"
#include "dynstr.h"
#include "ll.h"
#include "stack.h"
#include "parser.h"
#include "expr.h"

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

void gen_expression(exprll* ll) {
	// expressions can consist of expression, all operators have two operands, 
	// can be a result of a expession
    static int prev_rule = -1; 
    switch (ll->rule) {
        case erule_plus:
            break;
        case erule_minus:
            break;
        case erule_div:
            break;
        case erule_mul:
            break;
        case erule_cat:
            break;
        case erule_comp:
            break;
        case erule_comp_neg:
            break;
        case erule_lower:
            break;
        case erule_lower_equal:
            break;
        case erule_greater:
            break;
        case erule_greater_equal:
            break;
        case erule_brackets:
            break;
    } // end of rule switch
    prev_rule = ll->rule;
}