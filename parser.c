#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"
#include "error.h"
#include "parser.h"

/**
 * A global variable used for the current token.
*/
token_t *current_tkn = NULL;

void get_tkn() {
    if(current_tkn == NULL) {
        current_tkn = malloc(sizeof(token_t));
        if(current_tkn == NULL) {
            error_handle(0, compiler_error);
        }
    }
    get_token(current_tkn);
}

void free_tkn() {
    free(current_tkn);
    current_tkn = NULL;
} 

bool start() {
    get_tkn();
    // first it checks the token types and if then legal keywords
    if(current_tkn->type == token_identifier || current_tkn->type == token_varieble ||
       current_tkn->type == token_none || current_tkn->attr.keyword == keyword_function ||
       current_tkn->attr.keyword == keyword_function || current_tkn->attr.keyword == keyword_return ||
       current_tkn->attr.keyword == keyword_if || current_tkn->attr.keyword == keyword_while) {
        
        return program() && konec();
    } else {
        return false; // TODO
    }
}

bool program() {
    return true;
}

bool konec() {
    return true;
}