/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief The implementation of a code generator.
 *
 * @author Patrik Čerbák <xcerba00@stud.fit.vutbr.cz>
 */

#ifndef _GENERATOR_H
#define _GENERATOR_H

#include "dynstr.h"
#include "ll.h"
#include "symtable.h"

/**
 * Function for printing the IFJcode22 header.
*/
void gen_header();

/**
 * Function for printing a function definition.
*/
void gen_function_def(dynstr_t *id, list_t *parameters);

/**
 * @brief Function for printing the ending of a function definition.
 * 
 * @param id Function identifier
 */
void gen_function_def_end(dynstr_t *id, sym_table *gen_tree);

/**
 * @brief Function for printing function call 
 * 
 * @param id Function identifier 
 * @param parameters Function parameters
 * @param tree Parser tree
 */
void gen_function_call(dynstr_t *id, list_t* parameters, sym_table *tree);

/**
 * @brief Function for printing the call of function 
 * 
 * @param id Function identifier
 * @param parameters Function parametrs
 * @param tree Parser tree
 */

/**
 * @brief Function for filling variable, takes value from stack 
 * 
 * @param variable Variable to be filled 
 */
void gen_fill_variable(dynstr_t *variable);

/**
 * @brief Function for printing start of if
 * 
 */
void gen_if_start();

/**
 * @brief Function for printing start of else
 * 
 */
void gen_if_start_else();

/**
 * @brief Function for printing end of if and else;
 * 
 */
void gen_if_end();




void gen_write(list_t *parameters);
void gen_return(dynstr_t *id_function, sym_table *gen_tree, bool exit);


#endif