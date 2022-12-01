#include <stdio.h>
#include "generator.h"
#include "dynstr.h"
#include "ll.h"
#include "symtable.h"
#include "parser.h"
#include "error.h"
#include "scanner.h"

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

void gen_change_type(keywords new_type, keywords old_type, char *id,  char *cast_var, char *value){
    if(new_type == keyword_int && old_type == keyword_float){   // float to int 
        printf("FLOAT2INT LF@%%%s_%s LF@%s\n", id, cast_var, value);  // the variable that was defined at the gen_start_function with
    }
    if(new_type == keyword_float && old_type == keyword_int){   // int to float 
        printf("INT2FLOAT LF@%%%s_%s LF@%s\n", id, cast_var, value);
    } 
    // if any other cast shut it down
    error_handle(0 ,func_arr_or_ret_error);
    // TODO check if I chose the right error
    abort();
}

void gen_function_call(dynstr_t *id, list_t* parameters, sym_table *tree){
    list_node_t* recieve_parameters_i = parameters->first;
    sym_data *fce_data = st_search(tree, id);   // search for the function in tree
    list_node_t *expected_parameters_i = fce_data->parameters->first;

    while(recieve_parameters_i != NULL && expected_parameters_i != NULL){   // cast the parameter if necessary should also fill it
        if(recieve_parameters_i->type != expected_parameters_i->type){
            gen_change_type(recieve_parameters_i->type ,expected_parameters_i->type, id->array, expected_parameters_i->id->array, recieve_parameters_i->id->array);
        } else{                                                             // or just fill it 
        printf("MOVE LF@%%%s_%s LF@%s\n", id->array, expected_parameters_i->id->array, recieve_parameters_i->id->array);
        recieve_parameters_i = recieve_parameters_i->next;
        expected_parameters_i = expected_parameters_i->next;
        }
    }
    // TODO return value 
    printf("call %s_start\n", id->array);

}   
