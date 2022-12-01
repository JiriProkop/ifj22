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
void gen_function_def_end(dynstr_t *id);

#endif