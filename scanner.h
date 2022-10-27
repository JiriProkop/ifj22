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
 * @enum
 */
typedef enum {
    token_none,
    token_division,
    token_identifier,
    // TODO
} token_type;

/**
 * @struct token struct.
 */
typedef struct token {
    token_type type;
    unsigned line;
} token_t;

/**
 * @enum scanner states
 */
typedef enum {
    begin_s,
    start_s,
    division_s,
    line_cmnt_s,
    block_cmnt_start_s,
    block_cmnt_inside_s, // odtud
    string_start_s,
    string_escape_s,
    string_hex1_s,
    string_hex2_s,
    string_oct1_s,
    string_oct2_s,
    string_end_s,
    variable_s,
    identifier_s,
    identifier_w_null_s,
    php_end_s,
    integer_s,
    float_s,
    expo_start_s,
    expo_signed_s,
    expo_end_s,
    greater_s,
    greater_equal_s,
    lower_s,
    lower_equal_s,
    php_start_s,
    assign_s,
    assign_to_comp_s,
    comp_types_s,
    neg_type_comp1_s,
    neg_type_comp2_s,
    neg_type_comp3_s,
} state;

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
    keyword_bool,
} keywords;

/**
 * @union token attribute
 */
typedef union {
    int integer; // ma by na 64 bit
    double doub; // taky na 64 bit
    keywords keyword;
    dynstr_t *string;
} token_att;

/**
 * Finds next token in STDIN. If EOF was encountered token_type is set to token_none
 *
 * @param tok pointer to allocated token
 * @return Returns false if error was encountered, true otherwise
 */
bool get_token(token_t *tok);

#endif //_SCANNER_H