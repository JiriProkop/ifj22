/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Basic parser tests.
 *
 * @author Patrik Čerbák xcerba00 <xcerba00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/generator.h"
#include "../src/dynstr.h"
#include "../src/ll.h"
#include "../src/error.h"
#include "../src/scanner.h"
#include "../src/symtable.h"
#include "../src/parser.h"



#define COUNT 10

void print_tree_actual(sym_table *tree, int space){
    if(tree == NULL){
        return;
    }
    space += COUNT;

    print_tree_actual(tree->right, space);
    printf("\n");
    for(int i = COUNT; i <space; i++){
        printf(" ");
    }
    printf("%s\n", tree->id->array);

    print_tree_actual(tree->left, space);
}

void print_tree(sym_table *tree){
    if(tree == NULL){
        printf("tree is empty\n");
    }
    printf("\n");
    print_tree_actual(tree, 0);
    return;
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

void add_list_node(list_t *list, char id[], keywords type){
    dynstr_t *tmp_dstr = malloc(sizeof(dynstr_t));
    if(tmp_dstr == NULL){
        printf("failed to malloc dynstr in add node tests_generator\n");
        error_handle(0, compiler_error);
        return;
    }
    dynstr_init(tmp_dstr);
    dynstr_add_string(tmp_dstr, id);
    list_add(list, type, tmp_dstr);
}

dynstr_t *prep_id(char *id) {
    dynstr_t *str = malloc(sizeof(dynstr_t));
    if (str == NULL) {
        error_handle(0, compiler_error);
        return str;
    }
    dynstr_init(str);
    dynstr_add_string(str, id);
    return str;
}

list_t *prep_params() {
    list_t *list = malloc(sizeof(list_t));
    if (list == NULL) {
        error_handle(0, compiler_error);
        return list;
    }
    list_init(list);
    add_list_node(list, "teststr", keyword_string);
    list->last->can_be_null = false;
    add_list_node(list, "testint", keyword_int);
    list->last->can_be_null = false;
    add_list_node(list, "testfloat", keyword_float);
    list->last->can_be_null = false;
    return list;
}

list_t *prep_no_params() {
    list_t *list = malloc(sizeof(list_t));
    if (list == NULL) {
        error_handle(0, compiler_error);
        return list;
    }
    list_init(list);
    return list;
}

void add_tree_node(sym_table **tree, char id[], keywords type, list_t* params, bool is_func, bool nullable){
    sym_data *new_symdata = malloc(sizeof(sym_data));
    dynstr_t *new_id = malloc(sizeof(dynstr_t));
    dynstr_init(new_id);
    if (is_func)
        new_symdata->return_type = type;
    else
        new_symdata->type = type;
    new_symdata->can_be_null = nullable;
    new_symdata->parameters = params;
    new_symdata->local_frame = NULL;
    new_symdata->is_function = is_func;
    new_symdata->defined = is_func; // true when it's a function
    dynstr_add_string(new_id, id);
    st_insert(tree, new_id ,new_symdata);

}


int main() {
    printf("--- [GENERATOR TESTS] ---\n");

    // test gen header
    printf("\n[header gen test]\n");
    gen_header();

    // prepare function id and parameters
    dynstr_t *id = prep_id("testfunc");
    if (id == NULL)
        return ret;
    list_t *params = prep_params();
    if (params == NULL)
        return ret;
    printf("\n[info: parameter list]\n");
    print_list(params); // print parameter list
    dynstr_t *voidfunc = prep_id("voidfunc");
    if (voidfunc == NULL)
        return ret;
    list_t *no_params = prep_no_params();
    if (no_params == NULL)
        return ret;

    // test gen function def
    printf("\n[function def gen test - non-void function with 3 parameters]\n");
    gen_function_def(id, params);
    printf("\n[function def gen test - void function without parameters]\n");
    gen_function_def(voidfunc, no_params);

    sym_table *tree;
    st_init(&tree);
    add_tree_node(&tree, "testfunc", keyword_int, params, true, false);
    add_tree_node(&tree, "voidfunc", keyword_void, no_params, true, false);
    printf("\n[info: current tree]\n");
    print_tree(tree);

    // test gen function def end
    printf("\n[function def end gen test - non-void function with 3 parameters]\n");
    gen_function_def_end(id, tree);
    printf("\n[function def end gen test - void function without parameters]\n");
    gen_function_def_end(voidfunc, tree);

    // test gen function call function no casting 
    printf("\n[function def call test - non-void function with 3 parameters]\n");
    gen_function_call(id,params,tree);
    printf("\n[function def call test - void function without parameters]\n");
    gen_function_call(voidfunc,no_params,tree);

    // test gen write
    printf("\n[write gen test]\n");
    gen_write(params);

    // TODO test gen cast call?

    // test gen return
    printf("\n[return gen test - non-void function with 3 parameters - no exit]\n");
    gen_return(id, tree, false);
    temp_var_counter++;
    printf("\n[return gen test - non-void function with 3 parameters - exit]\n");
    gen_return(id, tree, true);
    temp_var_counter++;
    printf("\n[return gen test - void function without parameters - no exit]\n");
    gen_return(voidfunc, tree, false);
    temp_var_counter++;
    printf("\n[return gen test - void function without parameters - exit]\n");
    gen_return(voidfunc, tree, true);
    temp_var_counter++;

    // test gen def variable
    dynstr_t *testvar = prep_id("testvar");
    add_tree_node(&tree, testvar->array, keyword_string, NULL, false, false);
    printf("\n[info: current tree]\n");
    print_tree(tree);
    printf("\n[used variable def gen test - variable not defined]\n");
    gen_define_used_vars(tree);
    printf("\n[used variable def gen test - variable defined]\n");
    gen_define_used_vars(tree);

    // test gen fill variable
    printf("\n[fill variable gen test]\n");
    gen_fill_variable(testvar);
    temp_var_counter++;

    // test gen assign value
    printf("\n[value assignment gen test]\n");
    gen_assign_value(testvar);

    // test gen while
    printf("\n[while gen test]\n");
    gen_while_start();
    gen_while_check_condition();
    gen_while_end();
    temp_var_counter++;

    // test gen if
    printf("\n[if gen test]\n");
    gen_if_start();
    gen_if_start_else();
    gen_if_end();
    temp_var_counter++;

    // test gen closure
    printf("\n[closure gen test]\n");
    gen_closure();

    // cleanup
    st_dispose(&tree);
    string_free(id);
    string_free(voidfunc);
    string_free(testvar);
    // no need to list_dispose(params); because it's done by st_dispose()

    return ret;
}
