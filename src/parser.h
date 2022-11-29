/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief The implementation of a parser.
 *
 * @author Patrik Čerbák <xcerba00@stud.fit.vutbr.cz>
 * @author TODO - dopište se tady
 */

#include <stdbool.h>
#include "scanner.h"
#include "symtable.h"

#ifndef _PARSER_H
#define _PARSER_H


/**
 * A global variable used for the current token.
*/
extern token_t *current_tkn;

/**
 * Function frees everything allocated in parser and aborts.
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
 * @param tree A tree to add the note to.
 * @param id A identifier of the function/variable.
 * @param is_function A bool value - true if it is a function, false if it is a variable.
 * @param parameters A list of the function parameters.
 * @param params A number of function parametres.
 * @param type Either a return type of the function or a type of a variable.
*/
void add_node(sym_table **tree, dynstr_t *id, bool is_function, list_t *parameters, unsigned int params, keywords type);

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
 * @return True if it was correct.
*/
bool parametry();

/**
 * A function for the <param> rule.
 * 
 * @return True if it was correct.
*/
bool param();

/**
 * A function for the <prikaz_fce> rule.
 * 
 * @return True if it was correct.
*/
bool prikaz_fce();

/**
 * A function for the <prikaz> rule.
 * 
 * @return True if it was correct.
*/
bool prikaz();

/**
 * A function for the <else> rule.
 * 
 * @return True if it was correct.
*/
bool else_rule();

/**
 * A function for the <vol_parametry> rule.
 * 
 * @return True if it was correct.
*/
bool vol_parametry();

/**
 * A function for the <vol_param> rule.
 * 
 * @return True if it was correct.
*/
bool vol_param();

/**
 * A function for the <vol_par> rule.
 * 
 * @return True if it was correct.
*/
bool vol_par();

/**
 * A function for the <konec> rule.
 * 
 * @return True if it was correct.
*/
bool konec();

/**
 * A function for the <vyraz> rule.
 * 
 * @return True if it was correct.
*/
bool vyraz();

#endif