/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Basic scanner tests.
 *
 * @author Patrik Čerbák xcerba00 <xcerba00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "../src/scanner.h"
#include "../src/dynstr.h"
#include "../src/error.h"

// define the types, so I can print them later
char *types[] = {
    "token_none",
    "token_division",
    "token_string",
    "token_varieble",
    "token_identifier",
    "token_keyword_w_null",
    "token_integer",
    "token_float",
    "token_greater",
    "token_greater_equal",
    "token_lower",
    "token_lower_equal",
    "token_assign",
    "token_compare",
    "token_compare_neg",
    "token_semicol",
    "token_dot",
    "token_colon",
    "token_curly_left",
    "token_curly_right",
    "token_parentheses_left",
    "token_parentheses_right",
    "token_multiply",
    "token_plus",
    "token_minus",
    "token_keyword"
};

// define the keywords
char *keywords_str[] = {
    "keyword_else",
    "keyword_float",
    "keyword_function",
    "keyword_if",
    "keyword_int",
    "keyword_null",
    "keyword_return",
    "keyword_string",
    "keyword_void",
    "keyword_while",
};

int main() {
    printf("--- [SCANNER TESTS] ---\n");

    // allocate the token_t structure before using
    token_t *tkn = malloc(sizeof(token_t));
    if(tkn == NULL) {
        error_handle(0, compiler_error);
    }

    int skipped = 0;
    int type = 0;
    int i = 0;
    while(skipped == 0 || type != 0) {
        get_token(tkn);
        type = tkn->type;
        if(skipped == 0 && type != 0) {
            skipped = 1;
        } else if(type == 0) {
            break;
        }
        printf("\n[token number %d]\n", i);
        printf("Type of token: %s\n", types[type]);
        printf("Token line: %d\n", tkn->line);
        if(type == token_integer) {
            printf("Token integer attribute: %d\n", tkn->attr.integer);
        } else if(type == token_float) {
            printf("Token double attribute: %Lf\n", tkn->attr.doub);
        } else if(type == token_string || type == token_identifier || type == token_varieble) {
            printf("Token string attribute: '%s'\n", tkn->attr.str->array);
            string_free(tkn->attr.str);
        } else if(type == token_keyword || type == token_keyword_w_null) {
            printf("Token keyword attribute: %s\n", keywords_str[tkn->attr.keyword]);
        }

        i++;
    }

    free(tkn); // free the token

    return ret;
}
