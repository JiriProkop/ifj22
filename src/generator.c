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
unsigned temp_var_counter = 1;

// type_casting

void gen_header() {
    printf(".IFJcode22\n");
    printf("CRATEFRAME\n");
    // universal variable for conditions 
    printf("DEFVAR GF@%condition\n");
    printf("PUSHFRAME\n");
    // TODO typecasting 
}

void gen_function_def(dynstr_t *id, list_t* parameters){
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


void gen_function_def_end(dynstr_t *id, sym_table *gen_tree) {
    printf("POPFRAME\n");
    sym_data *func_data = st_search(gen_tree, id);
    // if you got on the end of void function exit with error 6;
    if(func_data->return_type != NULL){
        printf("EXIT int@6\n");
    }
    // if you got on the end of void function return
    printf("RETURN\n");
    printf("LABEL %s_end\n", id->array);
}

void gen_cast_call(bool can_be_null, keywords casted_type, char* id, bool global){
    // push can be null 
    if(can_be_null){
        printf("PUSH bool@true\n");
    } else{
        printf("PUSH bool@false\n");
    }
    // push type 
    switch (casted_type)
    {
    case keyword_int:
        printf("PUSH string@int\n");
        break;
    case keyword_float:
        printf("PUSH string@float\n");
        break;
    case keyword_string: 
        printf("PUSH string@string\n");
        break;
    default:
        break;
    }
    // push id
    if(global){
        printf("PUSH GF@%%%u\n", temp_var_counter);
    }else {
        printf("PUSH %s\n", id);
    }
}

void gen_return(dynstr_t *id_function, sym_table *gen_tree, bool exit){
    sym_data *data_func = st_search(gen_tree, id_function);
    if(exit){
        printf("EXIT GF%%%u\n");
        return;
    }
    if(data_func->return_type != keyword_void){
        gen_cast_call(data_func->can_be_null, data_func->return_type, "GF%%", true);
        printf("PUSH GF%%%u\n", temp_var_counter);
    }
    printf("POPFRAME");
    printf("RETURN\n");
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
    printf("CALL %s_start\n", id->array);
}   


    void gen_closure(){
    printf("POPFRAME\n");
    printf("CLEARS\n");
    printf("EXIT int@0\n");
}

void gen_def_variable(dynstr_t *variable, sym_table *tree_gen){
    // define if it is not already defined 
    sym_data* variable_node = st_search(tree_gen,variable);   
    if(variable_node->defined == false){
        printf("DEFVAR LF@%s\n", variable->array);
        variable_node->defined = true;
    }
}

void gen_fill_variable(dynstr_t *varible){
    printf("MOVE %s GF@%%%u\n", varible->array, temp_var_counter);
}

void gen_while_start(){
    // variable used to track if program went through the function 0 at the start  
    printf("DEFVAR LF@%%while%u_loop\n", gen_number_while_start);
    printf("MOVE LF@%%while%u_loop int@0\n", gen_number_while_start);
    // variable in which condition will be stored 
    // start lable
    printf("LABEL %%while%u_start\n", gen_number_while_start);
    printf("JUMPIFNEQ %%while%u_after_defvar int@0\n", gen_number_while_start);
    gen_number_while_start++;
    // go back to parser to print condition 
}

void gen_while_chceck_condition(){
    printf("JUMPIFEQ %%while%u_end LF@ int@1\n", gen_number_while_end);
}

void gen_while_end(){
    printf("JUMPIFEQ %%while%u_start LF@ int@1\n", gen_number_while_end);
    printf("LABEL while%u_end\n");
}

void gen_reads() {
    printf("JUMP reads_end\n");
    printf("LABEL reads_start\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%%reads\n");
    printf("PUSHFRAME\n");
    printf("READ LF@%%reads string\n");
    printf("PUSHS LF@%%reads\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL reads_end\n");
}

void gen_readi() {
    printf("JUMP readi_end\n");
    printf("LABEL readi_start\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%%readi\n");
    printf("PUSHFRAME\n");
    printf("READ LF@%%readi int\n");
    printf("PUSHS LF@%%readi\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL readi_end\n");
}

void gen_readf() {
    printf("JUMP readf_end\n");
    printf("LABEL readf_start\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@%%readf\n");
    printf("PUSHFRAME\n");
    printf("READ LF@%%readf float\n");
    printf("PUSHS LF@%%readf\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL readf_end\n");
}

void gen_write(list_t *parameters) {
    list_node_t *towrite = parameters->first;
    while (towrite != NULL)
    {
        printf("WRITE LF@%s\n", towrite->id->array);
        towrite = towrite->next;
    }
}

void gen_if_start(){
    
}
