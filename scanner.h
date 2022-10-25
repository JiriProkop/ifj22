/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief scanner implementation.
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#ifndef _SCANNER_H
#define _SCANNER_H

/**
 * @enum
 */
typedef enum {
    token_none,
    token_division,
    // TODO
}
token_type;

/**
 * @struct token struct.
 */
typedef struct token {
    token_type type;
    int id; // id zalezi na implementaci bin. stromu
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
    block_cmnt_inside_s,
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
// TODO build-in funkce - mozna, zalezi jak bude fungovat kotrola func. redefinition

/**
 * Finds next token in STDIN.
 * 
 * @return Returns token_t that was found. If none was(EOF) then token.type = toke_none
 */
token_t get_token();

#endif //_SCANNER_H