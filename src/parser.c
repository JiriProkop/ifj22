#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"
#include "error.h"
#include "expr.h"
#include "parser.h"
#include "symtable.h"
#include "generator.h"










/*


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




*/






/**
 * A global variable used for the current token.
*/
token_t *current_tkn = NULL;

/**
 * A global variable to tell the functions that a token has already been loaded.
*/
bool tkn_already_loaded = false;

/**
 * A global variable for the symtable tree.
*/
sym_table *tree;

int tkn_num = 0; // TODO - cislo jen na debug

void abort() {
	free(current_tkn);
    st_dispose(&tree);
    exit(ret);
}

void get_tkn() {
    if(current_tkn == NULL) {
        current_tkn = malloc(sizeof(token_t));
        if(current_tkn == NULL) {
            error_handle(0, compiler_error);
        }
    }
    if(get_token(current_tkn) == false) { // -> error, free everything and abort
        abort();
    }
    tkn_num++; // TODO - debug cislo
}

void free_tkn() {
    free(current_tkn);
    current_tkn = NULL;
} 

void add_node(sym_table **tree, dynstr_t *id, bool is_function, list_t *parameters, unsigned int params, keywords type, bool can_be_null) {
    sym_data *data = malloc(sizeof(sym_data));
    if(data == NULL) {
        error_handle(0,compiler_error);
        abort();
    }
    data->defined = true;
    if(is_function) {
        sym_table *subtree = malloc(sizeof(sym_data));
        if(subtree == NULL) {
            error_handle(0,compiler_error);
            abort();
        }
        st_init(&subtree);
        data->is_function = true;
        data->local_frame = subtree;
        data->parameters = parameters;
        data->params = params;
        data->return_type = type;
        data->can_be_null = can_be_null;
    } else {
        data->is_function = false;
        data->type = type;
        data->can_be_null = can_be_null;
    }
    
    st_insert(tree, id, data);
}

// the parser functions start here:
bool start() {
    tree = malloc(sizeof(sym_table));
    if(tree == NULL) {
        error_handle(0, compiler_error);
        abort();
    }
    st_init(&tree); // initialize the symtable tree

    bool value = false;
    get_tkn();
    printf("[DEBUG INFO]: currently in start(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    // first it checks the token types and if then legal keywords
    if(current_tkn->type == token_identifier || current_tkn->type == token_varieble ||
       current_tkn->type == token_none || current_tkn->attr.keyword == keyword_function ||
       current_tkn->attr.keyword == keyword_function || current_tkn->attr.keyword == keyword_return ||
       current_tkn->attr.keyword == keyword_if || current_tkn->attr.keyword == keyword_while) {
        
        value = program() && konec();
    }
    printf("[DEBUG INFO]: currently in start(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool program() {
    printf("[DEBUG INFO]: currently in program(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    bool value = false;
    // rule: <program> -> eps
    if(current_tkn->type == token_none) {
        value = true;
    // rule: <program> -> <definice> <program>
    } else if(current_tkn->type == token_keyword && current_tkn->attr.keyword == keyword_function) {
        value = true;
        // <definice>
        if(value && !definice()) {
            value = false;
        }
        // <program>
        if(!tkn_already_loaded) {
            get_tkn();
        } else {
            tkn_already_loaded = false;
        }
        if(value && !program()) {
            value = false;
        }
    // rule: <program> -> <prikaz> <program>
    } else if(current_tkn->type == token_identifier || current_tkn->type == token_varieble ||
              current_tkn->attr.keyword == keyword_return ||
              current_tkn->attr.keyword == keyword_if ||
              current_tkn->attr.keyword == keyword_while) {

        value = true;
        // <prikaz>
        if(value && !prikaz()) {
            value = false;
        }
        // <program>
        if(!tkn_already_loaded) {
            get_tkn();
        } else {
            tkn_already_loaded = false;
        }
        if(value && !program()) {
            value = false;
        }
    }
    printf("[DEBUG INFO]: currently in program(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool definice() {
    bool value = false;
    printf("[DEBUG INFO]: currently in definice(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    // rule: <definice> -> FUNCTION ID ( <parametry> ) : TYP { <prikaz_fce> }
    if(current_tkn->attr.keyword == keyword_function) {
        dynstr_t *id;
        list_t *parameters = malloc(sizeof(list_t));
        if(parameters == NULL) {
            error_handle(0, compiler_error);
            abort();
        }
        list_init(parameters);
        unsigned int params = 0;
        keywords type;
        bool can_be_null = false;
        int line_num = 0;

        value = true;
        // ID
        get_tkn();
        if(value && current_tkn->type == token_identifier) {
            id = current_tkn->attr.str;
            line_num = current_tkn->line;
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
        // no get_tkn(), since the token is already loaded from previous function
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
        if(value && (current_tkn->attr.keyword == keyword_void ||
                     current_tkn->attr.keyword == keyword_int ||
                     current_tkn->attr.keyword == keyword_float ||
                     current_tkn->attr.keyword == keyword_string)) {
            type = current_tkn->attr.keyword;
            if(current_tkn->type == token_keyword_w_null) {
                can_be_null = true;
            }
        } else {
            value = false;
        }

        if(st_search(tree, id) == NULL) {
            add_node(&tree, id, 1, parameters, params, type, can_be_null);
            gen_function_def(id);
        } else {
            error_handle(line_num, func_def_error);
            abort();
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
        if(value && current_tkn->type != token_curly_right) {
            value = false;
        }
    }
    printf("[DEBUG INFO]: currently in definice(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool parametry() {
    bool value = false;
    bool is_type = false; // is true if there was a token of a type
    printf("[DEBUG INFO]: currently in parametry(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
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
        get_tkn();
        value = param();
    }
    printf("[DEBUG INFO]: currently in parametry(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool param() {
    bool value = false;
    bool is_type = false;
    printf("[DEBUG INFO]: currently in param(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    // rule: <param> -> eps
    if(current_tkn->type == token_parentheses_right) {
        value = true;
    // rule: <param> -> , TYP VAR_ID <param>
    } else if(current_tkn->type == token_comma) {
        value = true;
        // TYP
        get_tkn();
        if(value && current_tkn->attr.keyword == keyword_int) {
            // TODO pridat ostatni typy
            is_type = true;
            get_tkn();
        } else if(value && current_tkn->attr.keyword == keyword_float) {
            is_type = true;
            get_tkn();
        }
    }

    // if the previous token was type, continues to check if the current token is VAR_ID
    if(is_type && current_tkn->type == token_varieble) {
            // TODO ulozit hodnotu
            string_free(current_tkn->attr.str);
            get_tkn();
            value = param();
    }
    printf("[DEBUG INFO]: currently in param(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool prikaz_fce() {
    bool value = false;
    printf("[DEBUG INFO]: currently in prikaz_fce(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    // rule: <prikaz_fce> -> eps
    if(current_tkn->type == token_curly_right) {
        value = true;
    // rule: <prikaz_fce> -> <prikaz> <prikaz_fce>
    } else if(current_tkn->type == token_identifier || current_tkn->type == token_varieble ||
              current_tkn->attr.keyword == keyword_return || current_tkn->attr.keyword == keyword_if ||
              current_tkn->attr.keyword == keyword_while) {
        
        value = true;
        // <prikaz>
        if(value && !prikaz()) {
            value = false;
        }

        // <prikaz_fce>
        if(!tkn_already_loaded) {
            get_tkn();
        } else {
            tkn_already_loaded = false;
        }
        if(value && !prikaz_fce()) {
            value = false;
        }
    }
    printf("[DEBUG INFO]: currently in prikaz_fce(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool prikaz() {
    bool value = false;
    printf("[DEBUG INFO]: currently in prikaz(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    // rule: <prikaz> -> RETURN <vyraz> ;
    if(current_tkn->attr.keyword == keyword_return) {
        value = true;
        // <vyraz>
        get_tkn();
        if(value && !vyraz()) {
            value = false; 
        }
        // ;
        if(value && current_tkn->type != token_semicol) {
            value = false;
        }
    // rule: <prikaz> -> ID ( <vol_parametry> ) ;
    } else if(current_tkn->type == token_identifier) {
        dynstr_t *id = current_tkn->attr.str;

        value = true;
        // (
        get_tkn();
        if(value && current_tkn->type != token_parentheses_left) {
            value = false;
        }
        // <vol_parametry>
        get_tkn();
        if(value && !vol_parametry()) {
            value = false;
        }
        // )
        if(value && current_tkn->type != token_parentheses_right) {
            value = false;
        }
        // ;
        get_tkn();
        if(value && current_tkn->type != token_semicol) {
            value = false;
        }

        if(st_search(tree, id) == NULL) {
            error_handle(current_tkn->line, func_def_error);
            abort();
        }

    // rule: <prikaz> -> IF ( <vyraz> ) { <prikaz_fce> } <else>
    } else if(current_tkn->attr.keyword == keyword_if) {
        value = true;
        // (
        get_tkn();
        if(value && current_tkn->type != token_parentheses_left) {
            value = false;
        }
        // <vyraz>
        get_tkn();
        if(value && !vyraz()) {
            value = false; 
        }
        // )
        if(value && current_tkn->type != token_parentheses_right) {
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
        if(value && current_tkn->type != token_curly_right) {
            value = false;
        }
        // <else>
        get_tkn();
        if(value && !else_rule()) {
            value = false;
        }
    // rule: <prikaz>-> WHILE ( <vyraz> ) { <prikaz_fce> }
    } else if(current_tkn->attr.keyword == keyword_while) {
        value = true;
        // (
        get_tkn();
        if(value && current_tkn->type != token_parentheses_left) {
            value = false;
        }
        // <vyraz>
        get_tkn();
        if(value && !vyraz()) {
            value = false; 
        }
        // )
        if(value && current_tkn->type != token_parentheses_right) {
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
        if(value && current_tkn->type != token_curly_right) {
            value = false;
        }
    // rule: <prikaz> -> VAR_ID = <vyraz> ;
    } else if(current_tkn->type == token_varieble) {
        dynstr_t *id = current_tkn->attr.str;

        value = true;
        // =
        get_tkn();
        if(value && current_tkn->type != token_assign) {
            value = false;
        }

        if(st_search(tree, id) == NULL) {
            add_node(&tree, id, 0, NULL, 0, keyword_null, true);
        }

        // <vyraz>
        get_tkn();
        if(value && current_tkn->type == token_identifier) {
            value = prikaz();
        } else {
            value = vyraz();
            // ;
            if(value && current_tkn->type != token_semicol) {
                value = false;
            }
        }
    }
    printf("[DEBUG INFO]: currently in prikaz(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool else_rule() {
    bool value = false;
    printf("[DEBUG INFO]: currently in else_rule(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    // rule: <else> -> eps
    if(current_tkn->type == token_identifier || current_tkn->type == token_varieble ||
       current_tkn->type == token_none || current_tkn->type == token_curly_right ||
       current_tkn->attr.keyword == keyword_function || current_tkn->attr.keyword == keyword_return ||
       current_tkn->attr.keyword == keyword_if || current_tkn->attr.keyword == keyword_while) {
    
        tkn_already_loaded = true;
        value = true;
    // rule: <else> -> ELSE { <prikaz_fce> }
    } else if(current_tkn->attr.keyword == keyword_else) {
        value = true;
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
        if(value && current_tkn->type != token_curly_right) {
            value = false;
        }
    }
    printf("[DEBUG INFO]: currently in else_rule(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool vol_parametry() {
    bool value = false;
    printf("[DEBUG INFO]: currently in vol_parametry(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    // rule: <vol_parametry> -> eps
    if(current_tkn->type == token_parentheses_right) {
        value = true;
    //rule: <vol_parametry> -> <vyraz> <vol_param>
    } else if(current_tkn->type == token_integer || current_tkn->type == token_float ||
              current_tkn->type == token_string) { // TODO zbytek moznosti na vyraz?

        value = true;
        // <vyraz>
        // no get_tkn(), since the token in if -^ is also a first token from <vyraz>
        if(value && !vyraz()) {
            value = false;
        }
        // <vol_param>
        if(value && !vol_param()) {
            value = false;
        }
    // rule: <vol_parametry> -> VAR_ID <vol_param>
    } else if(current_tkn->type == token_varieble) {
        // TODO ulozit hodnotu
        string_free(current_tkn->attr.str);
        get_tkn();
        value = vol_param();
    }
    printf("[DEBUG INFO]: currently in vol_parametry(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool vol_param() {
    bool value = false;
    printf("[DEBUG INFO]: currently in vol_param(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    // rule: <vol_param> -> eps
    if(current_tkn->type == token_parentheses_right) {
        value = true;
    // rule: <vol_param> -> , <vol_par>
    } else if(current_tkn->type == token_comma) {
        get_tkn();
        value = vol_par();
    }
    printf("[DEBUG INFO]: currently in vol_param(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool vol_par() {
    bool value = false;
    printf("[DEBUG INFO]: currently in vol_par(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    // rule: <vol_par> -> VAR_ID <vol_param>
    if(current_tkn->type == token_varieble) {
        get_tkn();
        value = vol_param();
    // rule: <vol_par> -> <vyraz> <vol_param>
    } else if(current_tkn->type == token_integer) { // TODO pridat ostatni stavy vyrazu
        value = true;
        // <vyraz>
        // no get_tkn(), since the token in if -^ is also a first token from <vyraz>
        if(value && !vyraz()) {
            value = false;
        }
        // <vol_param>
        if(value && !vol_param()) {
            value = false;
        }
    }
    printf("[DEBUG INFO]: currently in vol_par(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool konec() {
    bool value = false;
    printf("[DEBUG INFO]: currently in konec(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);
    if(current_tkn->type == token_none) {
        value = true;
    }
    printf("[DEBUG INFO]: currently in konec(), returning: %d\n", value);
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool vyraz() {
    printf("[DEBUG INFO]: currently in vyraz(), token number: %d, token line: %d\n", tkn_num, current_tkn->line);

    printf("[DEBUG INFO]: currently in vyraz(), exiting\n");
    return expr(*current_tkn);
}

// TODO vyraz do prikazu
// TODO rozdeleni prirazeni do promenne u = 