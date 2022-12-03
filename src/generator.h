/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief The implementation of a code generator.
 *
 * @author Patrik Čerbák <xcerba00@stud.fit.vutbr.cz>
 */

#ifndef _GENERATOR_H
#define _GENERATOR_H

#include "stack.h"
#include "symtable.h"

/**
 * Function for printing the IFJcode22 header.
*/
void gen_header();

/**
 * Function for printing a function definition.
*/
void gen_function_def();

void gen_expression(exprll *ll);

#endif