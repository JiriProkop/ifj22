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

dynstr_t *string_innit() {
    dynstr_t *str = malloc(sizeof(dynstr_t));
    if (str == NULL || !dynstr_init(str)) {
        free(str);
        return NULL;
    }
    return str;
}

void string_free(dynstr_t *attr) {
    dynstr_delete(attr);
    free(attr);
}

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
                if (c == '\n') {
                    line_c++;
                } else if (isspace(c)) {
                    break;
                } else if (c == '$') {
                    state = variable_s;
                    tok->type = token_varieble;
                    tok->line = line_c;
                    if ((tok->attr.str = string_innit()) == NULL) {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                } else if (isalpha(c) || c == '_') {
                    state = identifier_s;
                    tok->line = line_c;
                    tok->type = token_identifier;
                    if ((tok->attr.str = string_innit()) == NULL) {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                    ungetc(c, input);
                } else if (c == '?') {
                    c = getc(input);
                    if (isalpha(c) || c == '_') {
                        tok->type = token_identifier_w_null;
                        state = identifier_s;
                        if ((tok->attr.str = string_innit()) == NULL) {
                            error_handle(line_c, compiler_error);
                            return false;
                        }
                    } else if (c == '>') {
                        state = exit_s;
                    } else {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                    tok->line = line_c;
                    ungetc(c, input);
                } else if (c == '/') {
                    state = division_s;
                } else if (c == '"') {
                    state = string_start_s;
                    tok->type = token_string;
                    tok->line = line_c;
                    if ((tok->attr.str = string_innit()) == NULL) {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                } else if (isdigit(c)) {
                    state = integer_s;
                    tok->line = line_c;
                    ungetc(c, input);
                    if ((tok->attr.str = string_innit()) == NULL) {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
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
                    tok->line = line_c;
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
                break;
            }
            case string_start_s: {

                if (c == '"') {
                    state = start_s;
                    return true;
                } else if (c == '\n') {
                    line_c++;
                } else if (c == '\\') {
                    state = string_escape_s;
                } else if (c > 31) {
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
                    dynstr_add_char(tok->attr.str, '\n');
                    state = string_start_s;
                } else if (c == '"') {
                    dynstr_add_char(tok->attr.str, '"');
                    state = string_start_s;
                } else if (c == 't') {
                    dynstr_add_char(tok->attr.str, '\t');
                    state = string_start_s;
                } else if (c == '\\') {
                    dynstr_add_char(tok->attr.str, '\\');
                    state = string_start_s;
                } else if (c == '$') {
                    dynstr_add_char(tok->attr.str, '$');
                    state = string_start_s;
                } else {
                    dynstr_add_char(tok->attr.str, '\\');
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
                    error_handle(line_c, other_semantic_error);
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
                    error_handle(line_c, other_semantic_error);
                    return false;
                }
                if (tmp > CHAR_MAX) {
                    error_handle(line_c, other_semantic_error);
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
                    error_handle(line_c, other_semantic_error);
                    return false;
                }
                break;
            }
            case string_oct2_s: {
                if (c >= '0' && c <= '7') {
                    tmp += c - '0';
                    state = string_start_s;
                } else {
                    error_handle(line_c, other_semantic_error);
                    return false;
                }
                if (tmp > CHAR_MAX) {
                    error_handle(line_c, other_semantic_error);
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
                } else if (!strcmp(tok->attr.str->array, "float")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_float;
                } else if (!strcmp(tok->attr.str->array, "function")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_function;
                } else if (!strcmp(tok->attr.str->array, "if")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_if;
                } else if (!strcmp(tok->attr.str->array, "int")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_int;
                } else if (!strcmp(tok->attr.str->array, "null")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_null;
                } else if (!strcmp(tok->attr.str->array, "return")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_return;
                } else if (!strcmp(tok->attr.str->array, "string")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_string;
                } else if (!strcmp(tok->attr.str->array, "void")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_void;
                } else if (!strcmp(tok->attr.str->array, "while")) {
                    string_free(tok->attr.str);
                    tok->attr.keyword = keyword_while;
                }
                // some keyword cannot have ? before them
                if (tok->type == token_identifier_w_null) {
                    if ((tok->attr.keyword == keyword_else) ||
                        (tok->attr.keyword == keyword_function) ||
                        (tok->attr.keyword == keyword_if) ||
                        (tok->attr.keyword == keyword_null) ||
                        (tok->attr.keyword == keyword_return) ||
                        (tok->attr.keyword == keyword_void) ||
                        (tok->attr.keyword == keyword_while)) {
                        error_handle(line_c, expr_type_error);
                        return false;
                    }
                }
                return true;
            }
            case exit_s: {
                if (c != '\n' || (c = getc(input)) != EOF) {
                    error_handle(line_c, syntax_error);
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
                    error_handle(line_c, syntax_error);
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
                    error_handle(line_c, syntax_error);
                    return false;
                }
                state = expo_end_s;
                break;
            }
            case expo_end_s: {
                char s[128]; // should be more than enought
                unsigned counter = 0;
                while (isdigit(c)) {
                    s[counter++] = c;
                    c = getc(input);
                }
                ungetc(c, input);
                s[counter] = '\0';
                int exp;
                if (!str_to_num(s, &exp)) {
                    error_handle(line_c, syntax_error);
                    return false;
                }
                double holder;
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
                            tok->attr.doub /= pow(10, exp);
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
                    if (tok->attr.doub -round(tok->attr.doub) < ACCURACY) {
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
        } // end of switch
    }
    return true;
}

// TODO kontrola jestli je v kazdem ifu v start_c line_c++ a kazdy state return nebo break
// TODO stejne tak je potreba v konecnych stavech nastavovat stav na start