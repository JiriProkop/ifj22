/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Symbol table implementation using binary search tree.
 * Based on solution of the second IAL homework.
 *
 * @author Marek Chalupka <xchalu18@stud.fit.vut.cz>
 */

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include <stdbool.h>

// TODO additional functions?, data types?, keep track of unused variables?

/// @enum Symbol type
typedef enum {
    s_func, // function
    s_var   // variable
} sym_type;

/// @struct Symbol data structure
typedef struct { 
    sym_type type; // symbol type
    bool defined;
    struct tree_node *local_frame; // local symbol table
    unsigned int params; // number of parameters
} sym_data;

/// @struct Symbol table structure
typedef struct tree_node {
    char *key; // identifier
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
void st_insert(sym_table **tab, char *key, sym_data *data);

/**
 * @brief Searches a symbol table for a node with a specified identifier and returns its data.
 * 
 * @param tab Pointer to a symbol table
 * @param key Identifier
 * @return A pointer to node data when found, NULL otherwise.
 */
sym_data *st_search(sym_table *tab, char *key);

/**
 * @brief Deletes a node from a symbol table.
 * When a node with the specified identifier isn't found, no operation is performed.
 * When the node has one subtree, it is inherited by the deleted node's parent.
 * When the node has both subtrees, it is replaced by the rightmost node of the left subtree.
 * 
 * @param tab Pointer to a pointer to a symbol table
 * @param key Identifier
 */
void st_delete(sym_table **tab, char *key);

/**
 * @brief Disposes a symbol table by returning it to the initial state.
 * 
 * @param tab Pointer to a pointer to a symbol table
 */
void st_dispose(sym_table **tab);

#endif