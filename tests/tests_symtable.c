#include"../src/symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT 7

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
    print_tree_actual(tree, 0);
    return;
}


void add_tree_node(sym_table **tree, char id[], token_type type){
    sym_data *new_symdata = malloc(sizeof(sym_data));
    dynstr_t *new_id = malloc(sizeof(dynstr_t));
    dynstr_init(new_id);
    new_symdata->type = type;
    dynstr_add_string(new_id, id);
    printf("adding id: %s\n", new_id->array);
    printf("%p\n", *tree);
    st_insert(tree, new_id ,new_symdata);

}
int ret = 0;

void tree_empty(sym_table *tree){
    if(tree == NULL){
        printf("tree is empty\n");
    }
}

int main(){
    printf("\n --- TEST SYMTABLE --- \n");
    // test init 
    sym_table *tree;
    st_init(&tree);
    if (tree == NULL){
        printf("initalization succesfull\n");
    }
    tree_empty(tree);
    // test insert
    printf("%p\n", tree);
    add_tree_node(&tree, "50", token_none);
    tree_empty(tree);
    printf("first node added %s",tree->id->array);
    print_tree(tree);

    printf("\n --- TEST SYMTABLE OVER ---\n");
    
}