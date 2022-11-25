/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Bottom up dxpression parser.
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#ifndef _EXPR_H
#define _EXPR_H

typedef enum {
	erule_int,				// <val>	-> integer
	erule_float,			// <val>	-> float
	erule_string,			// <val>	-> string
	erule_id,				// <val>	-> id
    erule_val,				// <exp>	-> <val>
	erule_plus,				// <exp>	-> <exp> + <exp>
	erule_minus,			// <exp>	-> <exp> - <exp>
	erule_div,				// <exp>	-> <exp> / <exp>
	erule_mul,				// <exp>	-> <exp> * <exp>
	erule_cat,				// <exp>	-> <exp> . <exp>
	erule_comp,				// <exp>	-> <exp> === <exp>
	erule_comp_neg,			// <exp>	-> <exp> !== <exp>
	erule_lower,			// <exp>	-> <exp> < <exp>
	erule_lower_equal,		// <exp>	-> <exp> <= <exp>
	erule_greater,			// <exp>	-> <exp> > <exp>
	erule_greater_equal,	// <exp>	-> <exp> >= <exp>
	erule_brackets,			// <exp>	-> ( <exp> )
}
expr_rules;

#endif