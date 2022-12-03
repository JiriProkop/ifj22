/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Expression parser tests.
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#include "../src/error.h"
#include "../src/expr.h"
#include "../src/parser.h"
#include "../src/scanner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_EXPR_COUNT 1 //17

int main() {
    token_t tok;
    token_t tok1;
    current_tkn = malloc(sizeof(token_t));
    for (int i = 0; i < TEST_EXPR_COUNT; i++) {
        get_token(&tok);
        if (i >= 12 && i < 14) {
            get_token(&tok1);
            if (expr(tok, &tok1) == true) {
                printf("Correct!\n");
            } else {
                printf("False!\n");
            }
        } else {
            if (expr(tok, NULL) == true) {
                printf("Correct!\n");
            } else {
                printf("False!\n");
            }
        }
    }
    return ret;
}