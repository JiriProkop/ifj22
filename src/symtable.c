#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "symtable.h"
#include "dynstr.h"
#include "ll.h"

/**
 * @brief Initializes an empty symbol table.
 * 
 * @param tab Pointer to a pointer to a symbol table
 */
void st_init(sym_table **tab) {
    *tab = NULL;
}

/**
 * @brief Inserts a new node to a symbol table.
 * 
 * @param tab Pointer to a pointer to a symbol table
 * @param key Identifier
 * @param data Node data
 */
void st_insert(sym_table **tab, dynstr_t *id, sym_data *data) {
    if (*tab == NULL) {
        sym_table *new = malloc(sizeof(sym_table));
        if (new == NULL) {
            error_handle(0, compiler_error);
            return;
        }
        new->left = NULL;
        new->right = NULL;
        new->id = id;
        new->data = data;
        *tab = new;
    } else {
        int cmp = dynstrcmp(id, (*tab)->id);
        if (!cmp) {
            free((*tab)->data);
            (*tab)->data = data;
        } else if (cmp < 0) {
            st_insert(&(*tab)->left, id, data);
        } else {
            st_insert(&(*tab)->right, id, data);
        }
    }
}

/**
 * @brief Searches a symbol table for a node with a specified identifier and returns its data.
 * 
 * @param tab Pointer to a symbol table
 * @param key Identifier
 * @return A pointer to node data when found, NULL otherwise.
 */
sym_data *st_search(sym_table *tab, dynstr_t *id) {
    if (tab != NULL) {
        int cmp = dynstrcmp(id, tab->id);
        if (!cmp)
            return tab->data;
        else if (cmp < 0){
            return st_search(tab->left, id);
        }
        else{
            return st_search(tab->right, id);
        }
    }
    return NULL;
}

/**
 * @brief Function that repleaces target by his rightmost chid. Used in st_delete
 * 
 * @param target the target to be repleaced
 * @param tab the tree to search for the rightmost chid
 */
void replace_by_rightmost(sym_table *target, sym_table **tab) {
    if ((*tab)->right == NULL) {
        string_free(target->id);
        target->id = (*tab)->id;
        list_dispose(target->data->parameters);
        st_dispose(&target->data->local_frame);
        free(target->data);
        target->data = (*tab)->data;
        sym_table *d = *tab;
        *tab = d->left;
        free(d);
    } else {
        replace_by_rightmost(target, &(*tab)->right);
    }
}

/**
 * @brief Deletes a node from a symbol table.
 * When a node with the specified identifier isn't found, no operation is performed.
 * When the node has one subtree, it is inherited by the deleted node's parent.
 * When the node has both subtrees, it is replaced by the rightmost node of the left subtree.
 * 
 * @param tab Pointer to a pointer to a symbol table
 * @param key Identifier
 */
void st_delete(sym_table **tab, dynstr_t *id) {
    if (*tab != NULL) {
        int cmp = dynstrcmp(id, (*tab)->id);
        if (!cmp) {
            if (((*tab)->left == NULL) && ((*tab)->right == NULL)) {
                string_free((*tab)->id);
                list_dispose((*tab)->data->parameters);
                st_dispose(&(*tab)->data->local_frame);
                free((*tab)->data);
                free(*tab);
                *tab = NULL;
            } else if (((*tab)->left != NULL) && ((*tab)->right != NULL)) {
                replace_by_rightmost(*tab, &(*tab)->left);
                return;
            } else {
                sym_table *d = *tab;
                if (d->left == NULL){
                    *tab = d->right;
                }
                else{
                    *tab = d->left;
                }
                string_free(d->id);
                list_dispose(d->data->parameters);
                st_dispose(&d->data->local_frame);
                free(d->data);
                free(d);
            }
        } else if (cmp < 0) {
            st_delete(&(*tab)->left, id);
        } else {
            st_delete(&(*tab)->right, id);
        }
    }
}

/**
 * @brief Disposes a symbol table by returning it to the initial state.
 * 
 * @param tab Pointer to a pointer to a symbol table
 */
void st_dispose(sym_table **tab) {
    if (*tab != NULL) {
        st_dispose(&(*tab)->left);
        st_dispose(&(*tab)->right);
        st_dispose(&(*tab)->data->local_frame);
        string_free((*tab)->id);
        list_dispose((*tab)->data->parameters);
        free((*tab)->data);
        free(*tab);
        *tab = NULL;
    }
}
