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

#define TEST_EXPR_COUNT 1 // 17

int main() {
    token_t tok;
    token_t tok1;
    printf(".IFJcode22\nDEFVAR GF@$left_result\nDEFVAR GF@$right_result\nDEFVAR GF@$type1\nDEFVAR GF@$type2\nDEFVAR GF@$type\n");
    printf("DEFVAR GF@$tmp\nDEFVAR GF@$val1\nDEFVAR GF@$val2\nDEFVAR GF@$cond1\nDEFVAR GF@$cond2\n");
    printf("CREATEFRAME\nPUSHFRAME\n");
    printf("DEFVAR LF@fuj\nDEFVAR LF@tajbl\n");
    printf("MOVE LF@fuj int@2\nMOVE LF@tajbl int@5\n");
    current_tkn = malloc(sizeof(token_t));
    for (int i = 0; i < TEST_EXPR_COUNT; i++) {
        get_token(&tok);
        if (i >= 12 && i < 14) {
            get_token(&tok1);
            expr(tok, &tok1);
        } else {
            expr(tok, NULL);
        }
    }
    printf("WRITE GF@$left_result\nLABEL %%TYPE_CASTING\nWRITE string@END\nPOPFRAME\n");
    return 0;
}