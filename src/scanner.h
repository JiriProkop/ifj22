/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief scanner implementation.
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#ifndef _SCANNER_H
#define _SCANNER_H

#include "dynstr.h"
#include <stdbool.h>

/**
 * @enum token types
 */
typedef enum {
    token_none,
    token_division,
    token_string,
    token_varieble,
    token_identifier,
    token_keyword_w_null,
    token_integer,
    token_float,
    token_greater,
    token_greater_equal,
    token_lower,
    token_lower_equal,
    token_assign,
    token_compare,
    token_compare_neg,
    token_semicol,
    token_dot,
    token_colon,
    token_curly_left,
    token_curly_right,
    token_parentheses_left,
    token_parentheses_right,
    token_multiply,
	token_plus,
	token_minus,
	token_keyword,
	token_comma,
	// tokens for expr. parser stack
	token_expr_dollar,
	token_expr_shift,
	token_expr_e,
}
token_type;

/**
 * @enum IFJ22's keywords
 */
typedef enum {
    keyword_else,
    keyword_float,
    keyword_function,
    keyword_if,
    keyword_int,
    keyword_null,
    keyword_return,
    keyword_string,
    keyword_void,
    keyword_while,
} keywords;

/**
 * @union token attribute
 */
typedef union {
    int integer; // minimum size of 64 bits
    long double doub; // 64 bits should always be ok
    keywords keyword;
    dynstr_t *str;
} token_att;

/**
 * @struct token struct.
 */
typedef struct token {
    token_type type;
    unsigned line;
    token_att attr;
} token_t;

/**
 * Finds next token in STDIN. If EOF was encountered token_type is set to token_none
 *
 * @param tok pointer to allocated token
 * @return Returns false if error was encountered, true otherwise
 */
bool get_token(token_t *tok);

#endif //_SCANNER_H