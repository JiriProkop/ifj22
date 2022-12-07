/**
 * Project: compiler of IFJ22 programming language
 *
 * @file parser.h
 * @brief The implementation of a parser.
 *
 * @author Patrik Čerbák <xcerba00@stud.fit.vutbr.cz>
 */

#include <stdbool.h>
#include "scanner.h"
#include "symtable.h"
#include "dynstr.h"

#ifndef _PARSER_H
#define _PARSER_H

/**
 * A global variable used for the current token.
*/
extern token_t *current_tkn;

/**
 * A global variable used for temporary variables names.
*/
extern unsigned int temp_var_counter;

/**
 * A global variable for the tree of current frame.
*/
extern sym_table *current_frame;

/**
 * Function frees everything allocated in parser and exits program.
*/
void abort();

/**
 * Function for geting data into the global token.
*/
void get_tkn();

/**
 * Function for freeing the gloal token.
 * @warning Should be only run at the end of the program or when it encounters a error.
*/
void free_tkn();

/**
 * Function for adding a node to the tree.
 * 
 * @param tree A tree to add the note to.
 * @param id A identifier of the function/variable.
 * @param is_function A bool value - true if it is a function, false if it is a variable.
 * @param parameters A list of the function parameters.
 * @param params A number of function parametres.
 * @param type Either a return type of the function or a type of a variable.
 * @param can_be_null True if the type is nullable.
 * @param defined True only for parameters, false for other variables.
*/
void add_node(sym_table **tree, dynstr_t *id, bool is_function, list_t *parameters,
              unsigned int params, keywords type, bool can_be_null, bool defined);

/**
 * Function for converting the parameters list to the function subtree.
 * 
 * @param list The parameters list.
 * @param subtree The subtree of the function.
 * @return Returns the number of items in the list.
*/
unsigned int convert_list_to_subtree(list_t *list, sym_table **subtree);

/**
 * Function for adding the prebuilt functions to the symtable tree.
*/
void add_prebuilt();

/**
 * A function for the <start> rule.
 * 
 * @return True if it was correct.
*/
bool start();

/**
 * A function for the <program> rule.
 * 
 * @return True if it was correct.
*/
bool program();

/**
 * A function for the <definice> rule.
 * 
 * @return True if it was correct.
*/
bool definice();

/**
 * A function for the <parametry> rule.
 * 
 * @param fun_id Identifier of the function.
 * @param parameters A list with the parameters.
 * @return True if it was correct.
*/
bool parametry(dynstr_t *fun_id, list_t *parameters);

/**
 * A function for the <param> rule.
 * 
 * @param fun_id Identifier of the function.
 * @param parameters A list with the parameters.
 * @return True if it was correct.
*/
bool param(dynstr_t *fun_id, list_t *parameters);

/**
 * A function for the <prikaz_fce> rule.
 * 
 * @param current_function_id Identifier of the function we are in.
 * @return True if it was correct.
*/
bool prikaz_fce(dynstr_t *current_function_id);

/**
 * A function for the <prikaz> rule.
 * 
 * @param current_function_id Identifier of the function we are in, if we are in the "main" function,
 *                            then it should be NULL.
 * @return True if it was correct.
*/
bool prikaz(dynstr_t *current_function_id);

/**
 * A function for the <else> rule.
 * 
 * @param current_function_id Identifier of the function we are in.
 * @return True if it was correct.
*/
bool else_rule(dynstr_t *current_function_id);

/**
 * A function for the <vol_parametry> rule.
 * 
 * @param parameters The list of the call parameters.
 * @return True if it was correct.
*/
bool vol_parametry(list_t *parameters);

/**
 * A function for the <vol_param> rule.
 * 
 * @param parameters The list of the call parameters.
 * @return True if it was correct.
*/
bool vol_param(list_t *parameters);

/**
 * A function for the <vol_par> rule.
 * 
 * @param parameters The list of the call parameters.
 * @return True if it was correct.
*/
bool vol_par(list_t *parameters);

/**
 * A function for the <konec> rule.
 * 
 * @return True if it was correct.
*/
bool konec();

/**
 * A function for the <vyraz> rule.
 * 
 * @param second_tkn True if there are two tokens for the calling of expr()
 * @param prev_tok If second_tok is true, than it is a previous token, otherwise it is undefined.
 * @return True if it was correct.
*/
bool vyraz(bool second_tkn, token_t prev_tok);

#endif