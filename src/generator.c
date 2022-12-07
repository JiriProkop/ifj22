#include <stdio.h>
#include "generator.h"
#include "dynstr.h"
#include "ll.h"
#include "symtable.h"
#include "parser.h"
#include "error.h"
#include "scanner.h"

unsigned gen_number_while_start = 0;
unsigned gen_number_while_open = 0;

unsigned gen_number_if = 0;
unsigned gen_number_open_if = 0;

// type_casting
void gen_type_casting() {
    printf("DEFVAR GF@%%cast_var%%\n");
    printf("DEFVAR GF@%%cast_type%%\n");
    printf("DEFVAR GF@%%cast_null%%\n");
    printf("DEFVAR GF@%%cast_type_temp%%\n");
    printf("JUMP %%type_casting_end\n");

    printf("LABEL %%type_casting\n");
    printf("POPS GF@%%cast_var%%\n");
    printf("POPS GF@%%cast_type%%\n");
    printf("POPS GF@%%cast_null%%\n");
    printf("TYPE GF@%%cast_type_temp%% GF@%%cast_var%%\n");

    // check for null
    printf("JUMPIFNEQ %%cast_continue1 GF@%%cast_type_temp%% string@nil\n");
    printf("JUMPIFNEQ %%cast_continue1_1 GF@%%cast_null%% bool@true\n");
    printf("PUSHS GF@%%cast_var%%\n");
    printf("RETURN\n");
    printf("LABEL %%cast_continue1_1\n");
    printf("JUMPIFNEQ %%cast_continue1_2 GF@%%cast_type%% string@int\n");
    printf("PUSHS int@0\n");
    printf("RETURN\n");
    printf("LABEL %%cast_continue1_2\n");
    printf("JUMPIFNEQ %%cast_continue1_3 GF@%%cast_type%% string@float\n");
    printf("PUSHS float@0x0p+0\n");
    printf("RETURN\n");
    printf("LABEL %%cast_continue1_3\n");
    printf("PUSHS string@\n");
    printf("RETURN\n");
    printf("LABEL %%cast_continue1\n");
    // type should be int
    printf("JUMPIFNEQ %%cast_continue2 GF@%%cast_type%% string@int\n");
    printf("JUMPIFNEQ %%cast_continue2_1 GF@%%cast_type_temp%% string@int\n");
    printf("PUSHS GF@%%cast_var%%\n");
    printf("RETURN\n");
    printf("LABEL %%cast_continue2_1\n");
    printf("JUMPIFNEQ %%cast_error GF@%%cast_type_temp%% string@float\n");
    printf("FLOAT2INT GF@%%cast_var%% GF@%%cast_var%%\n");
    printf("PUSHS GF@%%cast_var%%\n");
    printf("RETURN\n");
    printf("LABEL %%cast_continue2\n");
    // type should be float
    printf("JUMPIFNEQ %%cast_continue3 GF@%%cast_type%% string@float\n");
    printf("JUMPIFNEQ %%cast_continue3_1 GF@%%cast_type_temp%% string@float\n");
    printf("PUSHS GF@%%cast_var%%\n");
    printf("RETURN\n");
    printf("LABEL %%cast_continue3_1\n");
    printf("JUMPIFNEQ %%cast_error GF@%%cast_type_temp%% string@int\n");
    printf("INT2FLOAT GF@%%cast_var%% GF@%%cast_var%%\n");
    printf("PUSHS GF@%%cast_var%%\n");
    printf("RETURN\n");
    printf("LABEL %%cast_continue3\n");
    // type should be string
    printf("JUMPIFNEQ %%cast_error GF@%%cast_type%% GF@%%cast_type_temp%%\n");
    printf("PUSHS GF@%%cast_var%%\n");
    printf("RETURN\n");

    printf("LABEL %%cast_error\n");
    printf("DPRINT string@Type\\032compability\\032error\\032in\\032arithmetic\\032string\\032or\\032relation\\032expression\\010\n");
    printf("EXIT int@7\n");
    printf("LABEL %%type_casting_end\n");
}

void gen_cast_to_bool() {
    printf("DEFVAR GF@%%cast_bool_var%%\n");
    printf("DEFVAR GF@%%cast_bool_type%%\n");
    printf("JUMP %%cast_bool_end\n");
    printf("LABEL %%cast_bool\n");

    printf("POPS GF@%%cast_bool_var%%\n");
    printf("TYPE GF@%%cast_bool_type%% GF@%%cast_bool_var%%\n");

    // int
    printf("JUMPIFNEQ %%cast_bool_continue1 GF@%%cast_bool_type%% string@int\n");
    printf("JUMPIFNEQ %%cast_bool_continue1_1 GF@%%cast_bool_var%% int@0\n");
    printf("PUSHS bool@false\n");
    printf("RETURN\n");
    printf("LABEL %%cast_bool_continue1_1\n");
    printf("PUSHS bool@true\n");
    printf("RETURN\n");
    printf("LABEL %%cast_bool_continue1\n");
    // float
    printf("JUMPIFNEQ %%cast_bool_continue2 GF@%%cast_bool_type%% string@float\n");
    printf("JUMPIFNEQ %%cast_bool_continue2_1 GF@%%cast_bool_var%% float@0x0p+0\n");
    printf("PUSHS bool@false\n");
    printf("RETURN\n");
    printf("LABEL %%cast_bool_continue2_1\n");
    printf("PUSHS bool@true\n");
    printf("RETURN\n");
    printf("LABEL %%cast_bool_continue2\n");
    // string
    printf("JUMPIFNEQ %%cast_bool_continue3 GF@%%cast_bool_type%% string@string\n");
    printf("JUMPIFNEQ %%cast_bool_continue3_1 GF@%%cast_bool_var%% string@\n");
    printf("PUSHS bool@false\n");
    printf("RETURN\n");
    printf("LABEL %%cast_bool_continue3_1\n");
    printf("PUSHS bool@true\n");
    printf("RETURN\n");
    printf("LABEL %%cast_bool_continue3\n");
    // bool
    printf("JUMPIFNEQ %%cast_bool_continue4 GF@%%cast_bool_type%% string@bool\n");
    printf("PUSHS GF@%%cast_bool_var%%\n");
    printf("RETURN\n");
    printf("LABEL %%cast_bool_continue4\n");

    printf("PUSHS bool@false\n");
    printf("RETURN\n");
    printf("LABEL %%cast_bool_end\n");
}

void gen_check_type() {
    printf("DEFVAR GF@%%check_type_var%%\n");
    printf("DEFVAR GF@%%check_type_type%%\n");
    printf("DEFVAR GF@%%check_type_bool%%\n");
    printf("DEFVAR GF@%%check_type_temp%%\n");
    printf("JUMP %%check_type_end\n");
    printf("LABEL %%check_type\n");

    printf("POPS GF@%%check_type_var%%\n");
    printf("POPS GF@%%check_type_type%%\n");
    printf("POPS GF@%%check_type_bool%%\n");

    printf("TYPE GF@%%check_type_temp%% GF@%%check_type_var%%\n");

    // the variable is null
    printf("JUMPIFNEQ %%check_type_continue GF@%%check_type_temp%% string@nil\n");
    printf("JUMPIFEQ %%check_type_error GF@%%check_type_bool%% bool@false\n");
    printf("RETURN\n");
    printf("LABEL %%check_type_continue\n");
    // other than null
    printf("JUMPIFNEQ %%check_type_error GF@%%check_type_type%% GF@%%check_type_temp%%\n");
    printf("RETURN\n");

    printf("LABEL %%check_type_error\n");
    printf("DPRINT string@Wrong\\032function\\032argument\\032type\\032and\\032or\\032count\\032or\\032wrong\\032return\\032type\\010\n");
    printf("EXIT int@4\n");
    printf("LABEL %%check_type_end\n");
}

// built-in functions:
void gen_reads() {
    printf("JUMP reads_end\n");
    printf("LABEL reads_start\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%reads\n");
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
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%readi\n");
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
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%readf\n");
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

void gen_floatval() {
    printf("DEFVAR GF@%%floatval_term\n");
    printf("JUMP floatval_end\n");
    printf("LABEL floatval_start\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@term\n");
    printf("MOVE TF@term GF@%%floatval_term\n");
    printf("PUSHFRAME\n");

    printf("PUSHS bool@false\n");
    printf("PUSHS string@float\n");
    printf("PUSHS LF@term\n");
    printf("CALL %%type_casting\n");
    printf("POPS LF@term\n");
    printf("PUSHS LF@term\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL floatval_end\n");
}

void gen_intval() {
    printf("DEFVAR GF@%%intval_term\n");
    printf("JUMP intval_end\n");
    printf("LABEL intval_start\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@term\n");
    printf("MOVE TF@term GF@%%intval_term\n");
    printf("PUSHFRAME\n");

    printf("PUSHS bool@false\n");
    printf("PUSHS string@int\n");
    printf("PUSHS LF@term\n");
    printf("CALL %%type_casting\n");
    printf("POPS LF@term\n");
    printf("PUSHS LF@term\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL intval_end\n");
}

void gen_strval() {
    printf("DEFVAR GF@%%strval_term\n");
    printf("JUMP strval_end\n");
    printf("LABEL strval_start\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@term\n");
    printf("MOVE TF@term GF@%%strval_term\n");
    printf("PUSHFRAME\n");

    printf("PUSHS bool@false\n");
    printf("PUSHS string@string\n");
    printf("PUSHS LF@term\n");
    printf("CALL %%type_casting\n");
    printf("POPS LF@term\n");
    printf("PUSHS LF@term\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL strval_end\n");
}

void gen_strlen() {
    printf("DEFVAR GF@%%strlen_s\n");
    printf("JUMP strlen_end\n");
    printf("LABEL strlen_start\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@s\n");
    printf("MOVE TF@s GF@%%strlen_s\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@len\n");
    printf("STRLEN LF@len LF@s\n");
    printf("PUSHS LF@len\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL strlen_end\n");
}

void gen_substring() {
    printf("DEFVAR GF@%%substring_s\n");
    printf("DEFVAR GF@%%substring_i\n");
    printf("DEFVAR GF@%%substring_j\n");
    printf("JUMP substring_end\n");
    printf("LABEL substring_start\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@s\n");
    printf("DEFVAR TF@i\n");
    printf("DEFVAR TF@j\n");
    printf("MOVE TF@s GF@%%substring_s\n");
    printf("MOVE TF@i GF@%%substring_i\n");
    printf("MOVE TF@j GF@%%substring_j\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@chk\n");
    printf("LT LF@chk LF@i int@0\n");
    printf("JUMPIFEQ substring_error LF@chk bool@true\n");
    printf("LT LF@chk LF@j int@0\n");
    printf("JUMPIFEQ substring_error LF@chk bool@true\n");
    printf("GT LF@chk LF@i LF@j\n");
    printf("JUMPIFEQ substring_error LF@chk bool@true\n");
    printf("DEFVAR LF@len\n");
    printf("STRLEN LF@len LF@s\n");
    printf("LT LF@chk LF@i LF@len\n");
    printf("JUMPIFNEQ substring_error LF@chk bool@true\n");
    printf("GT LF@chk LF@j LF@len\n");
    printf("JUMPIFEQ substring_error LF@chk bool@true\n");

    printf("DEFVAR LF@c\n");
    printf("DEFVAR LF@substr\n");
    printf("MOVE LF@substr string@\n");
    printf("JUMPIFEQ substring_loop_end LF@i LF@j\n");
    printf("LABEL substring_loop\n");
    printf("GETCHAR LF@c LF@s LF@i\n");
    printf("CONCAT LF@substr LF@substr LF@c\n");
    printf("ADD LF@i LF@i int@1\n");
    printf("JUMPIFNEQ substring_loop LF@i LF@j\n");
    printf("LABEL substring_loop_end\n");

    printf("PUSHS LF@substr\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL substring_error\n");
    printf("PUSHS nil@nil\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL substring_end\n");
}

void gen_ord() {
    printf("DEFVAR GF@%%ord_c\n");
    printf("JUMP ord_end\n");
    printf("LABEL ord_start\n");
    printf("JUMPIFEQ ord_empty GF@%%ord_c string@\n");
    printf("PUSHS GF@%%ord_c\n");
    printf("PUSHS int@0\n");
    printf("STRI2INTS\n");
    printf("RETURN\n");
    printf("LABEL ord_empty\n");
    printf("PUSHS int@0\n");
    printf("RETURN\n");
    printf("LABEL ord_end\n");
}

void gen_chr() {
    printf("DEFVAR GF@%%chr_i\n");
    printf("JUMP chr_end\n");
    printf("LABEL chr_start\n");
    printf("PUSHS GF@%%chr_i\n");
    printf("INT2CHARS\n");
    printf("RETURN\n");
    printf("LABEL chr_end\n\n");
}

void gen_header() {
    printf(".IFJcode22\n");
    printf("CREATEFRAME\n");
    // universal variable for conditions and assignments
    printf("DEFVAR GF@%%condition\n");
    printf("PUSHFRAME\n");
    // jump to the temporary variable definitions
    printf("CALL %%temp_var_definitions\n");
    
    gen_type_casting();
    gen_cast_to_bool();
    gen_check_type();
    gen_reads();
    gen_readi();
    gen_readf();
    gen_floatval();
    gen_intval();
    gen_strval();
    gen_strlen();
    gen_substring();
    gen_ord();
    gen_chr();
}

void gen_function_def(dynstr_t *id, list_t* parameters){
    list_node_t* i = parameters->first;
    // define all the arguments as temp variable for future use 
    while(i != NULL){
        printf("DEFVAR GF@%%%s_%s\n", id->array, i->id->array);
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
        printf("MOVE TF@%s GF@%%%s_%s\n",i->id->array, id->array, i->id->array);
        i = i->next;
    }
    printf("PUSHFRAME\n");

    // define all the variables used in the function
    printf("JUMP %s_definitions\n", id->array);
    printf("LABEL %s_definitions_back\n", id->array);
}

void gen_define_used_vars(sym_table *tree) {
    if(tree != NULL) {
        if(!tree->data->is_function && !tree->data->defined) {
            printf("DEFVAR LF@%s\n", tree->id->array);
            tree->data->defined = true;
        }
        gen_define_used_vars(tree->left);
        gen_define_used_vars(tree->right);
    }
}

void gen_function_def_end(dynstr_t *id, sym_table *gen_tree) {
    printf("POPFRAME\n");
    sym_data *func_data = st_search(gen_tree, id);
    // if you got on the end of void function exit with error 4
    if(func_data->return_type != keyword_void){
        printf("DPRINT string@Wrong\\032function\\032argument\\032type\\032and\\032or\\032count\\032or\\032wrong\\032return\\032type\\010\n");
        printf("EXIT int@4\n");
    }
    // if you got on the end of void function return
    printf("RETURN\n");

    // define all the variables used in the function
    printf("JUMP %s_definitions_skip\n", id->array);
    printf("LABEL %s_definitions\n", id->array);
    gen_define_used_vars(current_frame);
    printf("JUMP %s_definitions_back\n", id->array);
    printf("LABEL %s_definitions_skip\n", id->array);

    printf("LABEL %s_end\n", id->array);
}

void gen_cast_call(bool can_be_null, keywords casted_type, char* id, bool global){
    // push can be null 
    if(can_be_null){
        printf("PUSHS bool@true\n");
    } else{
        printf("PUSHS bool@false\n");
    }
    // push type 
    switch (casted_type)
    {
    case keyword_int:
        printf("PUSHS string@int\n");
        break;
    case keyword_float:
        printf("PUSHS string@float\n");
        break;
    case keyword_string: 
        printf("PUSHS string@string\n");
        break;
    default:
        break;
    }
    // push id
    if(global){
        printf("PUSHS GF@%%%u\n", temp_var_counter);
    }else {
        printf("PUSHS %s\n", id);
    }
    printf("CALL %%type_casting\n");
}

void gen_return(dynstr_t *id_function, sym_table *gen_tree, bool exit) {
    // if the return is from the "main" function
    if(exit) {
        printf("CLEARS\n");
        printf("EXIT int@0\n");
        return;
    } else {
        sym_data *data_func = st_search(gen_tree, id_function);
        // only push to the stack if the function is not void
        if(data_func->return_type != keyword_void) {
            if(data_func->can_be_null) {
                printf("PUSHS bool@true\n");
            } else {
                printf("PUSHS bool@false\n");
            }
            if(data_func->return_type == keyword_int) {
                printf("PUSHS string@int\n");
            } else if(data_func->return_type == keyword_float) {
                printf("PUSHS string@float\n");
            } else if(data_func->return_type == keyword_string) {
                printf("PUSHS string@string\n");
            }
            printf("PUSHS GF@%%%u\n", temp_var_counter);
            printf("CALL %%check_type\n"); // calling the check type function

            // push the return value to stack
            printf("PUSHS GF@%%%u\n", temp_var_counter);
        }
        printf("POPFRAME\n");
        printf("RETURN\n");
    }   
}

void gen_function_call(dynstr_t *id, list_t* parameters, sym_table *tree){
    list_node_t* recieve_parameters_i = parameters->first;
    sym_data *fce_data = st_search(tree, id);   // search for the function in tree
    list_node_t *expected_parameters_i = fce_data->parameters->first;

    while(recieve_parameters_i != NULL && expected_parameters_i != NULL){
        // check if the parameter is the right type, but ONLY if the var type is not void
        if(expected_parameters_i->type != keyword_void) {
            if(expected_parameters_i->can_be_null) {
                printf("PUSHS bool@true\n");
            } else {
                printf("PUSHS bool@false\n");
            }
            if(expected_parameters_i->type == keyword_int) {
                printf("PUSHS string@int\n");
            } else if(expected_parameters_i->type == keyword_float) {
                printf("PUSHS string@float\n");
            } else if(expected_parameters_i->type == keyword_string) {
                printf("PUSHS string@string\n");
            }
            printf("PUSHS LF@%s\n", recieve_parameters_i->id->array);
            printf("CALL %%check_type\n"); // calling the check type function
        }

        printf("MOVE GF@%%%s_%s LF@%s\n", id->array, expected_parameters_i->id->array, recieve_parameters_i->id->array);
        recieve_parameters_i = recieve_parameters_i->next;
        expected_parameters_i = expected_parameters_i->next;
    }
    printf("CALL %s_start\n", id->array);
}   

void gen_temp_var_definitions() {
    printf("JUMP %%temp_var_definitions_end\n");
    printf("LABEL %%temp_var_definitions\n");

    // define the variable for all used numbers
    for(unsigned int i = 0; i < temp_var_counter; i++) {
        printf("DEFVAR GF@%%%u\n", i);
    }

    // also define all the used variables in the "main" frame
    gen_define_used_vars(current_frame);

    printf("RETURN\n");
    printf("LABEL %%temp_var_definitions_end\n");
}

void gen_closure(){
    printf("POPFRAME\n");
    printf("CLEARS\n");
    printf("EXIT int@0\n");

    gen_temp_var_definitions(); // temporary variable definitons
}

void gen_fill_variable(dynstr_t *variable){
    printf("MOVE LF@%s GF@%%%u\n", variable->array, temp_var_counter);
}

void gen_assign_value(dynstr_t *variable) {
    printf("POPS LF@%s\n", variable->array);
}

void gen_while_start(){
    printf("LABEL %%while%u_start\n", gen_number_while_start);
    gen_number_while_start++;
    gen_number_while_open++;
}

void gen_while_check_condition(){
    // cast current expression result to bool
    printf("PUSHS GF@%%%u\n", temp_var_counter);
    printf("CALL %%cast_bool\n");
    printf("POPS GF@%%%u\n", temp_var_counter);
    // start if 
    printf("JUMPIFEQ %%while%u_end GF@%%%u bool@true\n", gen_number_while_start - gen_number_while_open, temp_var_counter);
}

void gen_while_end(){
    printf("JUMP %%while%u_start\n", gen_number_while_start - gen_number_while_open);
    printf("LABEL %%while%u_end\n", gen_number_while_start - gen_number_while_open);
    gen_number_while_open--;
}

void gen_if_start(){
    // cast current expression result to bool
    printf("PUSHS GF@%%%u\n", temp_var_counter);
    printf("CALL %%cast_bool\n");
    printf("POPS GF@%%%u\n", temp_var_counter);
    // start if 
    printf("JUMPIFEQ if%u_else GF@%%%u bool@false\n", gen_number_if, temp_var_counter);
    gen_number_if++;
    gen_number_open_if++;
}

void gen_if_start_else(){
    // start else
    printf("JUMP if%u_end\n", gen_number_if - gen_number_open_if);
    printf("LABEL if%u_else\n", gen_number_if - gen_number_open_if);
}

void gen_if_end(){
    printf("LABEL if%u_end\n", gen_number_if - gen_number_open_if);
    gen_number_open_if--;
}
