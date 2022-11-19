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
    bool value = false;
    printf("[DEBUG INFO]: currently in konec(), token number: %d\n", tkn_num);
    if(current_tkn->type == token_none) {
        value = true;
    }
    printf("[DEBUG INFO]: currently in konec(), returning: %d\n", value);
    return value;
}

bool definice() {
    bool value = false;
    printf("[DEBUG INFO]: currently in definice(), token number: %d\n", tkn_num);
    // rule: <definice> -> FUNCTION ID ( <parametry> ) : TYP { <prikaz_fce> }
    if(current_tkn->attr.keyword == keyword_function) {
        value = true;
        // ID
        get_tkn();
        if(value && current_tkn->type == token_identifier) {
            // TODO pridat identifikator do stromu
            string_free(current_tkn->attr.str);
        } else {
            value = false;
        }
        // (
        get_tkn();
        if(value && current_tkn->type != token_parentheses_left) {
            value = false;
        }
        // <parametry>
        get_tkn();
        if(value && !parametry()) {
            value = false;
        }
        // )
        get_tkn();
        if(value && current_tkn->type != token_parentheses_right) {
            value = false;
        }
        // :
        get_tkn();
        if(value && current_tkn->type != token_colon) {
            value = false;
        }
        // TYP
        get_tkn();
        if(value && current_tkn->attr.keyword == keyword_void) {
            // TODO pridat ostatni moznosti typu a asi ulozit do stromu
        } else {
            value = false;
        }
        // {
        get_tkn();
        if(value && current_tkn->type != token_curly_left) {
            value = false;
        }
        // <prikaz_fce>
        get_tkn();
        if(value && !prikaz_fce()) {
            value = false;
        }
        // }
        get_tkn();
        if(value && current_tkn->type != token_curly_right) {
            value = false;
        }
    }
    printf("[DEBUG INFO]: currently in definice(), returning: %d\n", value);
    return value;
}

bool prikaz() {
    bool value = false;
    printf("[DEBUG INFO]: currently in prikaz(), token number: %d\n", tkn_num);
    // rule: <prikaz> -> RETURN <vyraz> ;
    if(current_tkn->attr.keyword == keyword_return) {
        value = true;
        // <vyraz>
        get_tkn();
        if(value && !vyraz()) {
            value = false; 
        }
        // ;
        get_tkn();
        if(value && current_tkn->type != token_semicol) {
            value = false;
        }
    // rule: <prikaz> -> ID ( <vol_parametry> ) ;
    } else if(current_tkn->type == token_identifier) {
        // TODO - kontrola ID
        value = true;
        // (
        get_tkn();
        if(value && current_tkn->type != token_parentheses_left) {
            value = false;
        }
        // <vol_parametry>
        get_tkn();
		// undefined reference to `vol_parametry'
        // if(value && !vol_parametry()) {
        //     value = false;
        // }
        // )
        get_tkn();
        if(value && current_tkn->type != token_parentheses_right) {
            value = false;
        }
        // ;
        get_tkn();
        if(value && current_tkn->type != token_semicol) {
            value = false;
        }
    }
    printf("[DEBUG INFO]: currently in prikaz(), returning: %d\n", value);
    return value;
}

bool parametry() {
    bool value = false;
    bool is_type = false; // is true if there was a token of a type
    printf("[DEBUG INFO]: currently in parametry(), token number: %d\n", tkn_num);
    // rule: <parametry> -> eps
    if(current_tkn->type == token_parentheses_right) {
        value = true;
    // rule: <parametry> -> TYP VAR_ID <param>
    } else if(current_tkn->attr.keyword == keyword_int) {
        // TODO zkontrolovat ostatní typy a nekam ulozit (do stromu)
        is_type = true;
        get_tkn();
    } else if(current_tkn->attr.keyword == keyword_float) {
        // TODO zkontrolovat ostatní typy a nekam ulozit (do stromu)
        is_type = true;
        get_tkn();
    }

    // if the previous token was type, continues to check if the current token is VAR_ID
    if(is_type && current_tkn->type == token_varieble) {
        // TODO ulozit hodnotu
        string_free(current_tkn->attr.str);
        value = param();
    }
    printf("[DEBUG INFO]: currently in parametry(), returning: %d\n", value);
    return value;
}

bool param() {
    printf("[DEBUG INFO]: currently in param(), token number: %d\n", tkn_num);
    printf("[DEBUG INFO]: currently in param(), returning: %d\n", 1);
    return true;
}

bool prikaz_fce() {
    printf("[DEBUG INFO]: currently in prikaz_fce(), token number: %d\n", tkn_num);
    printf("[DEBUG INFO]: currently in prikaz_fce(), returning: %d\n", 1);
    return true;
}

bool vyraz() {
    return true;
}