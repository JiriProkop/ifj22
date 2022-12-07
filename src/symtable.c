/**
 * Project: compiler of IFJ22 programming language
 *
 * @file symtable.c
 * @brief Symbol table implementation using binary search tree.
 *
 * @author Marek Chalupka <xchalu18@stud.fit.vut.cz>
 * @author Štěpán Czajkowski <xczajk01@stud.fit.vutbr.cz>
 */

#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "symtable.h"
#include "dynstr.h"
#include "ll.h"

void st_init(sym_table **tab) {
    *tab = NULL;
}

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

void st_dispose(sym_table **tab) {
    if (*tab != NULL) {
        st_dispose(&(*tab)->left);
        st_dispose(&(*tab)->right);
        string_free((*tab)->id);
        list_dispose((*tab)->data->parameters);
        free((*tab)->data);
        free(*tab);
        *tab = NULL;
    }
}
