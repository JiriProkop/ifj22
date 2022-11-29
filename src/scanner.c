#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynstr.h"
#include "error.h"
#include "scanner.h"

#define ACCURACY (0.00005)

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
    variable_s,
    identifier_s,
    integer_s,
    float_s,
    expo_start_s,
    expo_end_s,
    greater_s,
    lower_s,
    assign_s,
    compare_neg_s,
    exit_s,
} state;

/**
 * Converts some characters into 3 digit decimal escape sequence such as \032 for codegen
 *
 * @param tok pointer to allocated token
 * @param c character to be converted - ASCII values 000-032, 035, 092 only
 */
void convert_to_escape(token_t* tok, char c) {
    dynstr_add_char(tok->attr.str, '\\');

    dynstr_add_char(tok->attr.str, (c / 100) % 10);
    dynstr_add_char(tok->attr.str, (c / 10) % 10);
    dynstr_add_char(tok->attr.str, c % 10);
}

/**
 * Converts integer writen as char to integer. ex. '6' -> 6
 *
 * @param str pointer to char array of numbers ending with '\0'
 * @param ptr pointer to int, where the converted integer is returned
 * @return Returns false if error occured, true otherwise
 */
bool str_to_num(const char str[], int *ptr) {
    *ptr = atoi(str);
    if (*ptr == 0) {
        for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] != '0') {
                return false;
            }
        }
    }
    return true;
}

FILE *input;

bool get_token(token_t *tok) {
    input = stdin;
    int c;
    int tmp = 0;
    static unsigned state = begin_s;
    static unsigned line_c = 1;
    tok->type = token_none;

    while (1) {
        c = getc(input);
        if (c == EOF)
            return true;

        switch (state) {
            case begin_s: {
                char prolog1[] = "<?php";
                char prolog2[] = "declare(strict_types=1);";
                bool white_space = false;
                for (unsigned i = 0; i < (unsigned)strlen(prolog1); i++) {
                    if (c != prolog1[i]) {
                        error_handle(line_c, syntax_error);
                        return false;
                    }
                    c = getc(input);
                }
                while (isspace(c) || c == '/') {
                    if (c == '\n')
                        line_c++;
                    white_space = true;

                    if (c == '/') // line comment counts as 1 whitespace
                    {
                        if ((c = getc(input)) == '/') {
                            while (1) {
                                c = getc(input);
                                if (c == '\n') {
                                    line_c++;
                                    break;
                                } else if (c == EOF) {
                                    error_handle(line_c, syntax_error);
                                    return false;
                                }
                            }
                            c = getc(input);
                            break;
                        } else if (c == '*') {
                            while (1) {
                                c = getc(input);
                                if (c == EOF) {
                                    error_handle(line_c, syntax_error);
                                    return false;
                                } else if (c == '*') {
                                    c = getc(input);
                                    if (c == '/') {
                                        break;
                                    }
                                }
                                if (c == '\n') {
                                    line_c++;
                                }
                            }
                        } else {
                            error_handle(line_c, syntax_error);
                            return false;
                        }
                    }
                    c = getc(input);
                }
                if (!white_space) {
                    error_handle(line_c, syntax_error);
                    return false;
                }
                for (unsigned i = 0; i < (unsigned)strlen(prolog2); i++) {
                    if (c != prolog2[i]) {
                        error_handle(line_c, syntax_error);
                        return false;
                    }
                    c = getc(input);
                }
                ungetc(c, input);
                state = start_s;
                break;
            }
            case start_s:
                tok->line = line_c;
                if (c == '\n') {
                    line_c++;
                } else if (isspace(c)) {
                    break;
                } else if (c == '$') {
                    state = variable_s;
                    tok->type = token_varieble;
                    if ((tok->attr.str = string_innit()) == NULL) {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                } else if (isalpha(c) || c == '_') {
                    state = identifier_s;
                    tok->type = token_identifier;
                    if ((tok->attr.str = string_innit()) == NULL) {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                    ungetc(c, input);
                } else if (c == '?') {
                    c = getc(input);
                    if (isalpha(c) || c == '_') {
                        tok->type = token_keyword_w_null;
                        state = identifier_s;
                        if ((tok->attr.str = string_innit()) == NULL) {
                            error_handle(line_c, compiler_error);
                            return false;
                        }
                        ungetc(c, input);
                    } else if (c == '>') {
                        state = exit_s;
                    } else {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                } else if (c == '/') {
                    state = division_s;
                } else if (c == '"') {
                    state = string_start_s;
                    tok->type = token_string;
                    if ((tok->attr.str = string_innit()) == NULL) {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                } else if (isdigit(c)) {
                    state = integer_s;
                    ungetc(c, input);
                    if ((tok->attr.str = string_innit()) == NULL) {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                } else if (c == '>') {
                    state = greater_s;
                } else if (c == '<') {
                    state = lower_s;
                } else if (c == '=') {
                    state = assign_s;
                } else if (c == '!') {
                    state = compare_neg_s;
                } else if (c == ';') {
                    tok->type = token_semicol;
                    return true;
                } else if (c == '.') {
                    tok->type = token_dot;
                    return true;
                } else if (c == ':') {
                    tok->type = token_colon;
                    return true;
                } else if (c == '{') {
                    tok->type = token_curly_left;
                    return true;
                } else if (c == '}') {
                    tok->type = token_curly_right;
                    return true;
                } else if (c == '(') {
                    tok->type = token_parentheses_left;
                    return true;
                } else if (c == ')') {
                    tok->type = token_parentheses_right;
                    return true;
                } else if (c == '*') {
                    tok->type = token_multiply;
                    return true;
                } else if (c == '+') {
                    tok->type = token_plus;
                    return true;
                } else if (c == '-') {
                    tok->type = token_minus;
                    return true;
                } else if (c == ',') {
                    tok->type = token_comma;
                    return true;
                }
                break;
            case division_s: {
                if (c == '/') {
                    state = line_cmnt_s;
                } else if (c == '*') {
                    state = block_cmnt_start_s;
                } else {
                    state = start_s;
                    ungetc(c, input); // c can be another token ex. 1/5
                    tok->type = token_division;
                    return true;
                }
                break;
            }
            case line_cmnt_s: {
                while (1) {
                    if (c == '\n') {
                        line_c++;
                        state = start_s;
                        break;
                    } else if (c == EOF) {
                        return true;
                    }
                    c = getc(input);
                }
                break;
            }
            case block_cmnt_start_s: {
                while (1) {
                    if (c == '\n') {
                        line_c++;
                    } else if (c == EOF) {
                        return true;
                    } else if (c == '*') {
                        state = block_cmnt_inside_s;
                        break;
                    }
                    c = getc(input);
                }
                break;
            }
            case block_cmnt_inside_s: {
                if (c == '/') {
                    state = start_s;
                } else {
                    state = block_cmnt_start_s;
                }
                if (c == '\n') {
                    line_c++;
                }
                break;
            }
            case string_start_s: {
                if (c == '"') {
                    state = start_s;
                    return true;
                } else if (c == '\n') {
                    error_handle(line_c, lex_analysis_err);
                    string_free(tok->attr.str);
                    return false;
                } else if (c == '\\') {
                    state = string_escape_s;
                } else if (c == ' ' || c == '#') {
                    convert_to_escape(tok, c);
                }else if (c > 31) {
                    dynstr_add_char(tok->attr.str, c);
                }
                break;
            }
            case string_escape_s: {
                tmp = 0;
                if (c == 'x') {
                    state = string_hex1_s;
                } else if (c >= '0' && c <= '7') {
                    tmp += 8 * 8 * (c - '0');
                    state = string_oct1_s;
                } else if (c == 'n') {
                    convert_to_escape(tok, c);
                    state = string_start_s;
                } else if (c == '"') {
                    dynstr_add_char(tok->attr.str, '"');
                    state = string_start_s;
                } else if (c == 't') {
                    convert_to_escape(tok, c);
                    state = string_start_s;
                } else if (c == '\\') {
                    convert_to_escape(tok, c);
                    state = string_start_s;
                } else if (c == '$') {
                    dynstr_add_char(tok->attr.str, '$');
                    state = string_start_s;
                } else {
                    convert_to_escape(tok, c);
                    ungetc(c, input);
                    state = string_start_s;
                }
                break;
            }
            case string_hex1_s: {
                if (c >= '0' && c <= '9') {
                    tmp += (c - '0') * 16;
                } else if (c >= 'A' && c <= 'F') {
                    tmp += (c - 'A' + 10) * 16;
                } else if (c >= 'a' && c <= 'f') {
                    tmp += (c - 'a' + 10) * 16;
                } else {
                    error_handle(line_c, lex_analysis_err);
                    string_free(tok->attr.str);
                    return false;
                }
                state = string_hex2_s;
                break;
            }
            case string_hex2_s: {
                if (c >= '0' && c <= '9') {
                    tmp += c - '0';
                } else if (c >= 'A' && c <= 'F') {
                    tmp += c - 'A' + 10;
                } else if (c >= 'a' && c <= 'f') {
                    tmp += c - 'a' + 10;
                } else {
                    error_handle(line_c, lex_analysis_err);
                    string_free(tok->attr.str);
                    return false;
                }
                if (tmp > CHAR_MAX) {
                    error_handle(line_c, lex_analysis_err);
                    string_free(tok->attr.str);
                    return false;
                }

                dynstr_add_char(tok->attr.str, tmp);
                state = string_start_s;
                break;
            }
            case string_oct1_s: {
                if (c >= '0' && c <= '7') {
                    tmp += 8 * (c - '0');
                    state = string_oct2_s;
                } else {
                    error_handle(line_c, lex_analysis_err);
                    string_free(tok->attr.str);
                    return false;
                }
                break;
            }
            case string_oct2_s: {
                if (c >= '0' && c <= '7') {
                    tmp += c - '0';
                    state = string_start_s;
                } else {
                    error_handle(line_c, lex_analysis_err);
                    string_free(tok->attr.str);
                    return false;
                }
                if (tmp > CHAR_MAX) {
                    error_handle(line_c, lex_analysis_err);
                    string_free(tok->attr.str);
                    return false;
                }
                dynstr_add_char(tok->attr.str, tmp);
                break;
            }
            case variable_s: {
                while (isalnum(c) || c == '_') {
                    dynstr_add_char(tok->attr.str, c);
                    c = getc(input);
                }
                ungetc(c, input);
                state = start_s;
                return true;
                break;
            }
            case identifier_s: {
                while (isalnum(c) || c == '_') {
                    dynstr_add_char(tok->attr.str, c);
                    c = getc(input);
                }
                ungetc(c, input);
                state = start_s;
                // check if identifier is not a keyword
                if (!strcmp(tok->attr.str->array, "else")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_else;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                } else if (!strcmp(tok->attr.str->array, "float")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_float;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                } else if (!strcmp(tok->attr.str->array, "function")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_function;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                } else if (!strcmp(tok->attr.str->array, "if")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_if;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                } else if (!strcmp(tok->attr.str->array, "int")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_int;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                } else if (!strcmp(tok->attr.str->array, "null")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_null;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                } else if (!strcmp(tok->attr.str->array, "return")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_return;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                } else if (!strcmp(tok->attr.str->array, "string")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_string;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                } else if (!strcmp(tok->attr.str->array, "void")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_void;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                } else if (!strcmp(tok->attr.str->array, "while")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_while;
                    if (tok->type != token_keyword_w_null) {
                        tok->type = token_keyword;
                    }
                }
                // some keyword cannot have '?' before them
                if (tok->type == token_keyword_w_null) {
                    if ((tok->attr.keyword == keyword_else) ||
                        (tok->attr.keyword == keyword_function) ||
                        (tok->attr.keyword == keyword_if) ||
                        (tok->attr.keyword == keyword_null) ||
                        (tok->attr.keyword == keyword_return) ||
                        (tok->attr.keyword == keyword_void) ||
                        (tok->attr.keyword == keyword_while)) {
                        error_handle(line_c, lex_analysis_err);
                        return false;
                    } else if (tok->attr.keyword != keyword_float &&
                               tok->attr.keyword != keyword_int &&
                               tok->attr.keyword != keyword_string) {
                        error_handle(line_c, lex_analysis_err);
                        return false;
                    }
                }
                return true;
            }
            case exit_s: {
                if (c != '\n' || (c = getc(input)) != EOF) {
                    error_handle(line_c, lex_analysis_err);
                    return false;
                }
                return true;
            }
            case integer_s: {
                tmp = 0;
                while (isdigit(c)) {
                    dynstr_add_char(tok->attr.str, c);
                    c = getc(input);
                }
                if (!str_to_num(tok->attr.str->array, &tmp)) {
                    error_handle(line_c, lex_analysis_err);
                    string_free(tok->attr.str);
                    return false;
                }
                if (c == '.') {
                    string_free(tok->attr.str);
                    tok->type = token_float;
                    tok->attr.doub = tmp;
                    state = float_s;
                    tok->attr.integer = tmp;
                } else if (tolower(c) == 'e') {
                    string_free(tok->attr.str);
                    tok->type = token_integer;
                    tok->attr.integer = tmp;
                    state = expo_start_s;
                } else {
                    ungetc(c, input);
                    tok->type = token_integer;
                    string_free(tok->attr.str);
                    tok->attr.integer = tmp;
                    state = start_s;
                    return true;
                }
                break;
            }
            case float_s: {
                unsigned counter = 1;
                while (isdigit(c)) {
                    tok->attr.doub += (c - '0') / pow(10, counter++);
                    c = getc(input);
                }
                if (tolower(c) == 'e') {
                    state = expo_start_s;
                } else {
                    ungetc(c, input);
                    state = start_s;
                    return true;
                }
                break;
            }
            case expo_start_s: {
                tmp = 1;
                if (c == '-') {
                    tmp = -1;
                } else if (isdigit(c)) {
                    ungetc(c, input);
                } else if (c != '+') {
                    error_handle(line_c, lex_analysis_err);
                    return false;
                }
                state = expo_end_s;
                break;
            }
            case expo_end_s: {
                char buffer[128]; // should be more than enought
                unsigned counter = 0;
                while (isdigit(c)) {
                    buffer[counter++] = c;
                    c = getc(input);
                }
                ungetc(c, input);
                buffer[counter] = '\0';
                int exp;
                if (!str_to_num(buffer, &exp)) {
                    error_handle(line_c, lex_analysis_err);
                    return false;
                }
                long double holder;
                if (tok->type == token_integer) {
                    if (tmp == 1) {
                        tok->attr.integer *= pow(10, exp);
                    } else if (tmp == -1) {
                        holder = tok->attr.integer;
                        holder /= pow(10, exp);
                        if (holder == round(holder)) {
                            tok->type = token_integer;
                            tok->attr.integer /= pow(10, exp);
                        } else {
                            tok->type = token_float;
                            tok->attr.doub = holder;
                        }
                    }
                    state = start_s;
                    return true;
                } else {
                    if (tmp == 1) {
                        tok->attr.doub *= pow(10, exp);
                    } else if (tmp == -1) {
                        tok->attr.doub /= pow(10, exp);
                    }
                    if (tok->attr.doub - round(tok->attr.doub) < ACCURACY) {
                        tmp = round(tok->attr.doub);
                        tok->attr.integer = tmp;
                        tok->type = token_integer;
                    } else {
                        tok->type = token_float;
                    }
                    state = start_s;
                    return true;
                }
                break;
            }
            case greater_s: {
                if (c == '=') {
                    tok->type = token_greater_equal;
                } else {
                    ungetc(c, input);
                    tok->type = token_greater;
                }
                state = start_s;
                return true;
                break;
            }
            case lower_s: {
                if (c == '=') {
                    tok->type = token_lower_equal;
                } else {
                    ungetc(c, input);
                    tok->type = token_lower;
                }
                state = start_s;
                return true;
                break;
            }
            case assign_s: {
                state = start_s;
                if (c != '=') {
                    ungetc(c, input);
                    tok->type = token_assign;
                } else if ((c = getc(input)) != '=') {
                    error_handle(line_c, lex_analysis_err);
                    return false;
                } else {
                    tok->type = token_compare;
                }
                return true;
                break;
            }
            case compare_neg_s: {
                state = start_s;
                if (c != '=' || (c = getc(input)) != '=') {
                    error_handle(line_c, lex_analysis_err);
                    return false;
                } else {
                    tok->type = token_compare_neg;
                    return true;
                }
                break;
            }
        } // end of switch
    }
    return true;
}