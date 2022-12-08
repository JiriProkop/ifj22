/**
 * Project: compiler of IFJ22 programming language
 *
 * @file generator.h
 * @brief The implementation of a code generator.
 * 
 * @author Patrik Čerbák xcerba00 <xcerba00@stud.fit.vutbr.cz>
 * @author Štěpán Czajkowski xczajk01 <xczajk01@stud.fit.vutbr.cz>
 * @author Marek Chalupka xchalu18 <xchalu18@stud.fit.vut.cz>
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

/**
 * @brief Function for printing for moving value into variable. It takes value from stack 
 * 
 * @param variable Variable to move the value into 
 */
void gen_fill_variable(dynstr_t *variable);

/**
 * @brief Function for printing for while start
 * 
 */
void gen_while_start();

/**
 * @brief Function for printing while condition check 
 * 
 */
void gen_while_check_condition();

/**
 * @brief Function for printing  the end of condition 
 * 
 */
void gen_while_end();

/**
 * @brief Function for printing file closure
 * 
 */
void gen_closure();

/**
 * @brief Function for printing build in function WRITE
 * 
 * @param parameters to be written out 
 */
void gen_write(list_t *parameters);

/**
 * @brief Function for printing return 
 * 
 * @param id_function Id of the function for searching function_data
 * @param gen_tree Tree for checking if the function should return something 
 * @param exit true for return outside function
 */
void gen_return(dynstr_t *id_function, sym_table *gen_tree, bool exit);

/**
 * @brief Assigns a return value from function to variable.
 * 
 * @param variable A variable identifier.
*/
void gen_assign_value(dynstr_t *variable);

/**
 * @brief Define all the used variables in the tree.
 * 
 * @param tree The tree to look for variables in.
*/
void gen_define_used_vars(sym_table *tree);

#endif
