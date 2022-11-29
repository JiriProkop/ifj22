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
#include "../src/scanner.h"

/**
 * @enum expression rules
 */
typedef enum {
	erule_int,				// 0:	<val>	-> integer
	erule_float,			// 1:	<val>	-> float
	erule_string,			// 2:	<val>	-> string
	erule_id,				// 3:	<val>	-> var
    erule_val,				// 4:	<exp>	-> <val>
	erule_plus,				// 5:	<exp>	-> <exp> + <exp>
	erule_minus,			// 6:	<exp>	-> <exp> - <exp>
	erule_div,				// 7:	<exp>	-> <exp> / <exp>
	erule_mul,				// 8:	<exp>	-> <exp> * <exp>
	erule_cat,				// 9:	<exp>	-> <exp> . <exp>
	erule_comp,				// 10:	<exp>	-> <exp> === <exp>
	erule_comp_neg,			// 11:	<exp>	-> <exp> !== <exp>
	erule_lower,			// 12:	<exp>	-> <exp> < <exp>
	erule_lower_equal,		// 13:	<exp>	-> <exp> <= <exp>
	erule_greater,			// 14:	<exp>	-> <exp> > <exp>
	erule_greater_equal,	// 15:	<exp>	-> <exp> >= <exp>
	erule_brackets,			// 16:	<exp>	-> ( <exp> )
}
expr_rules;

/**
 * Function checks whether expression is valid based on expression rules defined in documentation.
 *
 * @param first_tok first token of expression
 * @return Returns false if invalid expression was found, true otherwise.
 */
bool expr(token_t first_tok);

#endif