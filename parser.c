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

int tkn_num = 0; // TODO - cislo jen na debug

void get_tkn() {
    if(current_tkn == NULL) {
        current_tkn = malloc(sizeof(token_t));
        if(current_tkn == NULL) {
            error_handle(0, compiler_error);
        }
    }
    get_token(current_tkn);
    tkn_num++; // TODO - debug cislo
}

void free_tkn() {
    free(current_tkn);
    current_tkn = NULL;
} 

bool start() {
    bool value = false;
    get_tkn();
    printf("[DEBUG INFO]: currently in start(), token number: %d\n", tkn_num);
    // first it checks the token types and if then legal keywords
    if(current_tkn->type == token_identifier || current_tkn->type == token_varieble ||
       current_tkn->type == token_none || current_tkn->attr.keyword == keyword_function ||
       current_tkn->attr.keyword == keyword_function || current_tkn->attr.keyword == keyword_return ||
       current_tkn->attr.keyword == keyword_if || current_tkn->attr.keyword == keyword_while) {
        
        value = program() && konec();
    }
    printf("[DEBUG INFO]: currently in start(), returning: %d\n", value);
    return value;
}

bool program() {
    printf("[DEBUG INFO]: currently in program(), token number: %d\n", tkn_num);
    bool value = false;
    // rule: <program> -> eps
    if(current_tkn->type == token_none) {
        value = true;
    // rule: <program> -> <definice> <program>
    } else if(current_tkn->type == token_keyword && current_tkn->attr.keyword == keyword_function) {
        value = definice() && program();
    // rule: <program> -> <prikaz> <program>
    } else if(current_tkn->type == token_identifier || current_tkn->type == token_varieble ||
              current_tkn->attr.keyword == keyword_return ||
              current_tkn->attr.keyword == keyword_if ||
              current_tkn->attr.keyword == keyword_while) {

        value = prikaz() && program();
    }
    printf("[DEBUG INFO]: currently in program(), returning: %d\n", value);
    return value;
}

bool konec() {
    printf("[DEBUG INFO]: currently in konec(), token number: %d\n", tkn_num);
    printf("[DEBUG INFO]: currently in konec(), returning: %d\n", 1);
    return true;
}

bool definice() {
    get_tkn();
    printf("[DEBUG INFO]: currently in definice(), token number: %d\n", tkn_num);
    printf("[DEBUG INFO]: currently in definice(), returning: %d\n", 1);
    return true;
}

bool prikaz() {
    get_tkn();
    printf("[DEBUG INFO]: currently in prikaz(), token number: %d\n", tkn_num);
    printf("[DEBUG INFO]: currently in prikaz(), returning: %d\n", 1);
    return true;
}