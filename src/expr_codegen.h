/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief The implementation of a expression code generator.
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#ifndef _EXPR_CODEGEN_H
#define _EXPR_CODEGEN_H

#include "stack.h"
#include "symtable.h"


void gen_expression(exprll *ll);

#endif