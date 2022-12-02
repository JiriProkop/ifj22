/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Bottom up expression parser.
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#ifndef _EXPR_H
#define _EXPR_H

#include <stdbool.h>
#include "scanner.h"
#include "ll.h"

/**
 * @enum expression rules
 */
typedef enum {
    erule_val,				// 0:	<exp>	-> <val>
	erule_plus,				// 1:	<exp>	-> <exp> + <exp>
	erule_minus,			// 2:	<exp>	-> <exp> - <exp>
	erule_div,				// 3:	<exp>	-> <exp> / <exp>
	erule_mul,				// 4:	<exp>	-> <exp> * <exp>
	erule_cat,				// 5:	<exp>	-> <exp> . <exp>
	erule_comp,				// 6:	<exp>	-> <exp> === <exp>
	erule_comp_neg,			// 7:	<exp>	-> <exp> !== <exp>
	erule_lower,			// 8:	<exp>	-> <exp> < <exp>
	erule_lower_equal,		// 9:	<exp>	-> <exp> <= <exp>
	erule_greater,			// 10:	<exp>	-> <exp> > <exp>
	erule_greater_equal,	// 11:	<exp>	-> <exp> >= <exp>
	erule_brackets,			// 12:	<exp>	-> ( <exp> )
}
expr_rules;

/**
 * Function checks whether expression is valid based on precedent expression table defined in documentation.
 *
 * @param first_tok first token of expression
 * @param second_tok second token of expression, give NULL if 2nd token is unknown
 * @param symtam pointer to corresponding symtable (frame where given expressions is)
 * @return Returns false if invalid expression was found, true otherwise.
 */
bool expr(token_t first_tok, token_t* second_tok, sym_table* symtab);

#endif