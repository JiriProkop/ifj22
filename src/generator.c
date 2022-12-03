#include <stdio.h>
#include "generator.h"
#include "dynstr.h"
#include "ll.h"
#include "symtable.h"
#include "parser.h"
#include "error.h"
#include "scanner.h"

unsigned gen_number_while_start = 0;
unsigned gen_number_while_end = 0;

void gen_header() {
    printf(".IFJcode22\n");
}

void gen_function_def(dynstr_t *id, list_t* parameters) {
    list_node_t* i = parameters->first;
    // define all the arguments as temp variable for future use 
    while(i != NULL){
        printf("DEFVAR LF@%%%s_%s\n", id->array, i->id->array);
        i = i->next;
    }
    // jump end when giong through the code 
    printf("JUMP %s_end\n", id->array);
    // jump start for when the function is called 
    printf("LABEL %s_start\n", id->array);
    // fill the arguments with coresponding arguments
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
    printf("CALL %s_start\n", id->array);
}   


    void gen_closure(){
    printf("POPFRAME\n");
    printf("CLEARS\n");
    printf("EXIT int@0\n");
}

// this could be used for while as well ? 
void gen_def_variable(dynstr_t *variable, token_t *value, sym_table *tree_gen){
    // define if it is not already defined 
    sym_data* variable_node = st_search(tree_gen,variable);   
    if(variable_node == NULL){
        printf("DEFVAR LF@%s\n", variable->array);
    }
    // fill the variable with the right type 
    switch (value->type){
        case token_integer:
            printf("MOVE LF@%s int@%d\n", variable->array, value->attr.integer);
            break;
        case token_float:
            printf("MOVE LF@%s float@%d\n", variable->array, value->attr.doub);
            break;
        case token_string:
            printf("MOVE LF@%s string@%s\n", variable->array, value->attr.doub);
            break;
        // TODO expresion 
        // TODO void ?
        // case token_varieble:
        //     printf("MOVE LF@%s float@%d\n", variable->array, value->attr.doub);
        //     break;
        // TODO right errror ? 
        default:
            error_handle(0, other_semantic_error);
    }


}

// void gen_fill_variable(){
//     printf("MOVE LF%s %s\n", variable->array, value->array);
//     value->attr.
// } 

void gen_while_start(){
    // variable used to track if program went through the function 
    printf("DEFVAR LF@%%while_condition%u\n");
    printf("MOVE LF@%%while_condition%u int@0\n");
    printf("DEFVAR LF@ ");
    // start lable
    printf("LABEL %%while_start%u\n", gen_number_while_start);
    // jump to the end of definitions if it is not the first time through
    printf("JUMPIFNEQ %%while_define_end%u LF%%@while_condition%u int@0\n", gen_number_while_start, gen_number_while_start);
    gen_number_while_start++;
}



void gen_while_coindition(){

}

void gen_while_end(){
    printf("JUMPIFEQ %%while_start%u LF@ int@1", (gen_number_while_start - 1), );
}