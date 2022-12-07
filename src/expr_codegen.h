/**
 * Project: compiler of IFJ22 programming language
 *
 * @file expr_codegen.h
 * @brief The implementation of a expression code generator.
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#ifndef _EXPR_CODEGEN_H
#define _EXPR_CODEGEN_H

#include "stack.h"
#include "symtable.h"

/**
 * Generates code for operation multiply.
 *
 * @param ll pointer to expression linked list with operations and operands.
 */
void gen_expression(exprll *ll);

#endif