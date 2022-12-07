/**
 * Project: compiler of IFJ22 programming language
 *
 * @file symtable.h
 * @brief Symbol table implementation using binary search tree.
 *
 * @author Marek Chalupka <xchalu18@stud.fit.vut.cz>
 * @author Štěpán Czajkowski <xczajk01@stud.fit.vutbr.cz>
 */

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include <stdbool.h>
#include "scanner.h"
#include "ll.h"
#include "dynstr.h"

/// @struct Symbol data structure
typedef struct{
    bool is_function; // true if it is a function, false if it is a variable
    keywords type; // symbol type
    keywords return_type; // return type for functions
    bool can_be_null; // can the type be null?
    bool defined;
    struct tree_node *local_frame; // local symbol table
    unsigned int params; // number of parameters
    list_t *parameters;
} sym_data;

/// @struct Symbol table structure
typedef struct tree_node {
    dynstr_t *id; // identifier
    sym_data *data; // node data
    struct tree_node *left; // left child
    struct tree_node *right; // right child
} sym_table;

/**
 * @brief Initializes an empty symbol table.
 * 
 * @param tab Pointer to a pointer to a symbol table
 */
void st_init(sym_table **tab);

/**
 * @brief Inserts a new node to a symbol table.
 * 
 * @param tab Pointer to a pointer to a symbol table
 * @param key Identifier
 * @param data Node data
 */
void st_insert(sym_table **tab, dynstr_t *id, sym_data *data);

/**
 * @brief Searches a symbol table for a node with a specified identifier and returns its data.
 * 
 * @param tab Pointer to a symbol table
 * @param key Identifier
 * @return A pointer to node data when found, NULL otherwise.
 */
sym_data *st_search(sym_table *tab, dynstr_t *id);

/**
 * @brief Disposes a symbol table by returning it to the initial state.
 * 
 * @param tab Pointer to a pointer to a symbol table
 */
void st_dispose(sym_table **tab);

#endif