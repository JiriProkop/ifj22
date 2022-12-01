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
        printf("failed to malloc dynstr in add node tests_generator\n");
    }
    dynstr_t *tmp_dstr = malloc(sizeof(dynstr_t));
    dynstr_init(tmp_dstr);
    tmp_str = strcpy(tmp_str, id);
    dynstr_add_string(tmp_dstr, tmp_str);
    list_add(list, type, tmp_dstr);
    free(tmp_str);
}

// TODO malloc, init, and fill dynstr_t

dynstr_t *prep_func_id(char *id) {
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
    add_node(list, "teststr", token_string);
    add_node(list, "testint", token_integer);
    add_node(list, "testfloat", token_float);
    return list;
}


int main() {
    printf("--- [GENERATOR TESTS] ---\n");

    // test gen header
    printf("\n[header gen test]\n");
    gen_header();

    // prepare function id and parameters
    dynstr_t *id = prep_func_id("testfunc");
    if (id == NULL)
        return ret;
    list_t *params = prep_params();
    if (params == NULL)
        return ret;
    printf("\n[info: parameter list]\n");
    print_list(params); // print parameter list

    // test function def
    printf("\n[function def gen test]\n");
    gen_function_def(id, params);

    // test function def end
    printf("\n[function def end gen test]\n");
    gen_function_def_end(id);

    // cleanup
    string_free(id);
    list_dispose(params);

    return ret;
}
