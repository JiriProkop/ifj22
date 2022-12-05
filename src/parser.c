#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"
#include "error.h"
#include "expr.h"
#include "parser.h"
#include "symtable.h"
#include "generator.h"

/**
 * A global variable used for the current token.
*/
token_t *current_tkn = NULL;

/**
 * A global variable used for temporary variables names.
*/
unsigned int temp_var_counter = 0;

/**
 * A global variable to tell the functions that a token has already been loaded.
*/
bool tkn_already_loaded = false;

/**
 * A global variable for the symtable tree.
*/
sym_table *tree;

/**
 * A global variable for the tree of current frame.
*/
sym_table *current_frame;

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
    data->defined = false;
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

unsigned int convert_list_to_subtree(list_t *list, sym_table **subtree) {
    unsigned int num_of_params = 0;
    list_node_t *temp = list_first(list);
    while(temp != NULL) {
        add_node(subtree, temp->id, 0, NULL, 0, temp->type, temp->can_be_null);
        num_of_params++;
        temp = temp->next;
    }
    return num_of_params;
}


// the parser functions start here:
bool start() {
    tree = malloc(sizeof(sym_table));
    if(tree == NULL) {
        error_handle(0, compiler_error);
        abort();
    }
    st_init(&tree); // initialize the symtable tree
    current_frame = tree; // the current frame is the now allocated tree

    bool value = false;
    get_tkn();
    // first it checks the token types and if then legal keywords
    if(current_tkn->type == token_identifier || current_tkn->type == token_varieble ||
       current_tkn->type == token_none || current_tkn->attr.keyword == keyword_function ||
       current_tkn->attr.keyword == keyword_function || current_tkn->attr.keyword == keyword_return ||
       current_tkn->attr.keyword == keyword_if || current_tkn->attr.keyword == keyword_while) {
        
        value = program() && konec();
    }
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool program() {
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
    } else {
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
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool definice() {
    bool value = false;
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
        if(value && !parametry(id, parameters)) {
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

        // adding the function to the symtable tree
        if(st_search(tree, id) == NULL) {
            add_node(&tree, id, 1, parameters, params, type, can_be_null);
            current_frame = st_search(tree, id)->local_frame;
            st_search(tree, id)->params = convert_list_to_subtree(parameters, &current_frame);
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

        // return the frame back to the main frame
        current_frame = tree;
    }
    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool parametry(dynstr_t *fun_id, list_t *parameters) {
    bool value = false;

    keywords type;
    bool can_be_null = false;
    // rule: <parametry> -> eps
    if(current_tkn->type == token_parentheses_right) {
        value = true;
    // rule: <parametry> -> TYP VAR_ID <param>
    } else if(current_tkn->attr.keyword == keyword_int ||
              current_tkn->attr.keyword == keyword_float ||
              current_tkn->attr.keyword == keyword_string) {

        type = current_tkn->attr.keyword;
        if(current_tkn->type == token_keyword_w_null) {
            can_be_null = true;
        }

        // VAR_ID
        get_tkn();
        if(current_tkn->type == token_varieble) {
            // adding the parameter to the arguments list
            list_add(parameters, type, current_tkn->attr.str);
            parameters->last->can_be_null = can_be_null;

            // <param>
            get_tkn();
            value = param(fun_id, parameters);
        }
    }

    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool param(dynstr_t *fun_id, list_t *parameters) {
    bool value = false;

    keywords type;
    bool can_be_null = false;
    // rule: <param> -> eps
    if(current_tkn->type == token_parentheses_right) {
        value = true;
    // rule: <param> -> , TYP VAR_ID <param>
    } else if(current_tkn->type == token_comma) {
        value = true;
        // TYP
        get_tkn();
        if(value && (current_tkn->attr.keyword == keyword_int ||
                     current_tkn->attr.keyword == keyword_float ||
                     current_tkn->attr.keyword == keyword_string)) {
            
            type = current_tkn->attr.keyword;
            if(current_tkn->type == token_keyword_w_null) {
                can_be_null = true;
            }
            
            // VAR_ID
            get_tkn();
            if(current_tkn->type == token_varieble) {
                // save the parameter to the list
                list_add(parameters, type, current_tkn->attr.str);
                parameters->last->can_be_null = can_be_null;

                // <param>
                get_tkn();
                value = param(fun_id, parameters);
            }
        }
    }

    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool prikaz_fce() {
    bool value = false;
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

    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool prikaz() {
    bool value = false;
    // rule: <prikaz> -> RETURN <vyraz> ;
    if(current_tkn->attr.keyword == keyword_return) {
        value = true;
        // <vyraz>
        get_tkn();
        if(value && !vyraz(false, *current_tkn, current_frame)) {
            value = false; 
        }
        // ;
        if(value && current_tkn->type != token_semicol) {
            value = false;
        }

        temp_var_counter++;
    // rule: <prikaz> -> ID ( <vol_parametry> ) ;
    } else if(current_tkn->type == token_identifier) {
        dynstr_t *id = current_tkn->attr.str;
        if(st_search(tree, id) == NULL) {
            error_handle(current_tkn->line, func_def_error);
            abort();
        }
        list_t *parameters = malloc(sizeof(list_t));
        if(parameters == NULL) {
            error_handle(0, compiler_error);
            abort();
        }
        list_init(parameters);

        value = true;
        // (
        get_tkn();
        if(value && current_tkn->type != token_parentheses_left) {
            value = false;
        }
        // <vol_parametry>
        get_tkn();
        if(value && !vol_parametry(parameters)) {
            value = false;
        }

        // check if we got the right number of parameters
        list_node_t *temp = parameters->first;
        unsigned int num_of_params = 0;
        while(temp != NULL) {
            num_of_params++;
            temp = temp->next;
        }
        if(st_search(tree, id)->params != num_of_params) {
            error_handle(current_tkn->line, func_arr_or_ret_error);
            abort();
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
        if(value && !vyraz(false, *current_tkn, current_frame)) {
            value = false; 
        }
        // )
        if(value && current_tkn->type != token_parentheses_right) {
            value = false;
        }

        temp_var_counter++;
    
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
        if(value && !vyraz(false, *current_tkn, current_frame)) {
            value = false; 
        }
        // )
        if(value && current_tkn->type != token_parentheses_right) {
            value = false;
        }

        temp_var_counter++;

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
        token_t prev_tok = *current_tkn;

        value = true;
        // something else
        get_tkn();
        if(value && current_tkn->type != token_assign) {
            vyraz(true, prev_tok, current_frame);
            // ;
            if(value && current_tkn->type != token_semicol) {
                value = false;
            }
            temp_var_counter++;
        // =
        } else {
            if(st_search(current_frame, id) == NULL) {
                add_node(&current_frame, id, 0, NULL, 0, keyword_null, true);
            }

            // <vyraz>
            get_tkn();
            if(value && current_tkn->type == token_identifier) {
                value = prikaz();
            } else {
                value = vyraz(false, *current_tkn, current_frame);
                // ;
                if(value && current_tkn->type != token_semicol) {
                    value = false;
                }
            }
            temp_var_counter++;
        }
    // checks expressions
    } else {
        value = vyraz(false, *current_tkn, current_frame);
        // check for ;
        if(value && current_tkn->type != token_semicol) {
            value = false;
        }
        temp_var_counter++;
    }

    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool else_rule() {
    bool value = false;
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

    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool vol_parametry(list_t *parameters) {
    bool value = false;
    // rule: <vol_parametry> -> eps
    if(current_tkn->type == token_parentheses_right) {
        value = true;
    // rule: <vol_parametry> -> VAR_ID <vol_param>
    } else if(current_tkn->type == token_varieble) {
        // VAR_ID
        if(st_search(current_frame, current_tkn->attr.str) != NULL) {
            list_add(parameters, keyword_void, current_tkn->attr.str);
        } else {
            error_handle(current_tkn->line, func_arr_or_ret_error);
            abort();
        }

        // <vol_param>
        get_tkn();
        value = vol_param(parameters);
    //rule: <vol_parametry> -> <vyraz> <vol_param>
    } else {
        value = true;
        // <vyraz>
        // no get_tkn(), since the token in if -^ is also a first token from <vyraz>
        if(value && !vyraz(false, *current_tkn, current_frame)) {
            value = false;
        }
        // adding the parameter to a list
        if(value) {
            // id is NULL, since the parameter is <vyraz>, not VAR_ID
            list_add(parameters, keyword_void, NULL);
        }
        temp_var_counter++;

        // <vol_param>
        if(value && !vol_param(parameters)) {
            value = false;
        }
    }

    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool vol_param(list_t *parameters) {
    bool value = false;
    // rule: <vol_param> -> eps
    if(current_tkn->type == token_parentheses_right) {
        value = true;
    // rule: <vol_param> -> , <vol_par>
    } else if(current_tkn->type == token_comma) {
        get_tkn();
        value = vol_par(parameters);
    }

    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool vol_par(list_t *parameters) {
    bool value = false;
    // rule: <vol_par> -> VAR_ID <vol_param>
    if(current_tkn->type == token_varieble) {
        // VAR_ID
        if(st_search(current_frame, current_tkn->attr.str) != NULL) {
            list_add(parameters, keyword_void, current_tkn->attr.str);
        } else {
            error_handle(current_tkn->line, func_arr_or_ret_error);
            abort();
        }

        // <vol_param>
        get_tkn();
        value = vol_param(parameters);
    // rule: <vol_par> -> <vyraz> <vol_param>
    } else { 
        value = true;
        // <vyraz>
        // no get_tkn(), since the token in if -^ is also a first token from <vyraz>
        if(value && !vyraz(false, *current_tkn, current_frame)) {
            value = false;
        }
        // adding the parameter to a list
        if(value) {
            // id is NULL, since the parameter is <vyraz> not, VAR_ID
            list_add(parameters, keyword_void, NULL);
        }
        temp_var_counter++;

        // <vol_param>
        if(value && !vol_param(parameters)) {
            value = false;
        }
    }

    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool konec() {
    bool value = false;
    if(current_tkn->type == token_none) {
        value = true;
    }

    if(!value) {
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    return value;
}

bool vyraz(bool second_tkn, token_t prev_tok, sym_table *frame) {
    if(second_tkn) {
        return expr(prev_tok, current_tkn, frame);
    } else {
        return expr(*current_tkn, NULL, frame);
    }
}

// TODOs na probrání na schůzce:
// $a + 5 jako parametr nezpracuje výraz -> předat to celé na zpracování výrazu? Nebo je to vůbec legal?

// TODO write funkce
