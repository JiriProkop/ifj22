#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "symtable.h"

void st_init(sym_table **tab) {
    *tab = NULL;
}

void st_insert(sym_table **tab, char *key, sym_data *data) {
    if (*tab == NULL) {
        sym_table *new = malloc(sizeof(sym_table));
        if (new == NULL) {
            error_handle(0, compiler_error);
            return;
        }
        new->left = NULL;
        new->right = NULL;
        new->key = key;
        new->data = data;
        *tab = new;
    } else {
        int cmp = strcmp(key, (*tab)->key);
        if (!cmp) {
            free((*tab)->data);
            (*tab)->data = data;
        } else if (cmp < 0) {
            st_insert(&(*tab)->left, key, data);
        } else {
            st_insert(&(*tab)->right, key, data);
        }
    }
}

sym_data *tab_search(sym_table *tab, char *key) {
    if (tab != NULL) {
        int cmp = strcmp(key, tab->key);
        if (!cmp)
            return tab->data;
        else if (cmp < 0)
            return st_search(tab->left, key);
        else
            return st_search(tab->left, key);
    }
    return NULL;
}

void replace_by_rightmost(sym_table *target, sym_table **tab) {
    if ((*tab)->right == NULL) {
        target->key = (*tab)->key;
        target->data = (*tab)->data;
        sym_table *d = *tab;
        *tab = d->left;
        free(d);
    } else {
        replace_by_rightmost(target, &(*tab)->right);
    }
}

void st_delete(sym_table **tab, char *key) {
    if (*tab != NULL) {
        int cmp = strcmp(key, (*tab)->key);
        if (!cmp) {
            if (((*tab)->left == NULL) && ((*tab)->right == NULL)) {
                free(*tab);
                *tab = NULL;
            } else if (((*tab)->left != NULL) && ((*tab)->right != NULL)) {
                replace_by_rightmost(*tab, &(*tab)->left);
            } else {
                sym_table *d = *tab;
                if (d->left == NULL)
                    *tab = d->right;
                else
                    *tab = d->left;
                free(d);
            }
        } else if (cmp < 0) {
            st_delete(&(*tab)->left, key);
        } else {
            st_delete(&(*tab)->right, key);
        }
    }
}

// TODO additional functions?

void st_dispose(sym_table **tab) {
    if (*tab != NULL) {
        st_dispose(&(*tab)->left);
        st_dispose(&(*tab)->right);
        free(*tab);
        *tab = NULL;
    }
}
