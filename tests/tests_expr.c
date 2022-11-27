/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Expression parser tests.
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#include "../src/error.h"
#include "../src/expr.h"
#include "../src/scanner.h"
#include <stdbool.h>
#include <stdio.h>

#define TEST_EXPR_COUNT 4

int main() {
    token_t tok;
    for (int i = 0; i < TEST_EXPR_COUNT; i++) {
        get_token(&tok);
        if (expr(tok) == true) {
            printf("Correct!\n");
        } else {
            printf("False!\n");
        }
    }
    return ret;
}