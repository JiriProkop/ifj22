#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynstr.h"
#include "error.h"
#include "scanner.h"

dynstr_t *string_innit() {
    dynstr_t *str = malloc(sizeof(dynstr_t));
    if (str == NULL || !dynstr_init(str)) {
        free(str);
        return NULL;
    }
    return str;
}

FILE *input;

bool get_token(token_t *tok) {
    input = stdin;
    int c;
    unsigned tmp = 0; // for hex to dec and octal to dec conversions
    static unsigned state = begin_s;
    static unsigned line_c = 1;
    tok->type = token_none;
    tok->line = 0; // jen pro kontrolu u testu

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
                    } else if (c == '>') { // za timto uz nesmi byt zadne bile znaky, jen 1 '\n'
                        state = exit_s;    // TODO
                    } else {
                        error_handle(line_c, compiler_error);
                        return false;
                    }
                    tok->line = line_c;
                    ungetc(c, input);
                } else if (c == '/') {
                    state = division_s;
                } else if (c == EOF) {
                    return true;
                } else if (c == '"') {
                    state = string_start_s;
                    tok->type = token_string;
                    tok->line = line_c;
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
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
                    tok->attr.keyword = keyword_else;
                } else if (!strcmp(tok->attr.str->array, "float")) {
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
                    tok->attr.keyword = keyword_float;
                } else if (!strcmp(tok->attr.str->array, "function")) {
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
                    tok->attr.keyword = keyword_function;
                } else if (!strcmp(tok->attr.str->array, "if")) {
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
                    tok->attr.keyword = keyword_if;
                } else if (!strcmp(tok->attr.str->array, "int")) {
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
                    tok->attr.keyword = keyword_int;
                } else if (!strcmp(tok->attr.str->array, "null")) {
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
                    tok->attr.keyword = keyword_null;
                } else if (!strcmp(tok->attr.str->array, "return")) {
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
                    tok->attr.keyword = keyword_return;
                } else if (!strcmp(tok->attr.str->array, "string")) {
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
                    tok->attr.keyword = keyword_string;
                } else if (!strcmp(tok->attr.str->array, "void")) {
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
                    tok->attr.keyword = keyword_void;
                } else if (!strcmp(tok->attr.str->array, "while")) {
                    dynstr_delete(tok->attr.str);
                    free(tok->attr.str);
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
        } // end of switch
    }
    return true;
}

// TODO kontrola jestli je v kazdem ifu v start_c line_c++