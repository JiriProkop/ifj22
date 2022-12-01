/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Basic parser tests.
 *
 * @author Patrik Čerbák xcerba00 <xcerba00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "../src/generator.h"
#include "../src/dynstr.h"
#include "../src/ll.h"
#include "../src/error.h"



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
        printf("failed to malloc dynstr in add node tests_ll\n");
    }
    dynstr_t *tmp_dstr = malloc(sizeof(dynstr_t));
    dynstr_init(tmp_dstr);
    tmp_str = strcpy(tmp_str, id);
    dynstr_add_string(tmp_dstr, tmp_str);
    list_add(list, type, tmp_dstr);
    free(tmp_str);
}

// TODO malloc, init, and fill dynstr_t




int ret = 0;
int main() {
    // test gen header
    gen_header();
    // tests function def

    return ret;
}
