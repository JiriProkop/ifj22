/**
 * Project: compiler of IFJ22 programming language
 *
 * @file expr_codegen.c
 * @brief The implementation of a expression code generator.
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#include "expr_codegen.h"
#include "dynstr.h"
#include "error.h"
#include "expr.h"
#include "ll.h"
#include "parser.h"
#include "stack.h"
#include "symtable.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * Tells where to save result of expression.
 *
 * @return Returns true, if result should be stored in right_result. False otherwise.
 */
bool gen_expr_different_place(exprll *rule_node, exprll *ll) {
    if (rule_node->next->next->next != NULL && rule_node->next->next->next->rule == erule_val && rule_node->next->next->next->ptok == NULL && !exprll_prev_value(rule_node, ll)) {
        return true;
    }
    return false;
}

/**
 * Checks if varieble is defined, if not calls abort().
 *
 */
void check_existance(token_t *tok, unsigned *num) {
    // TODO
    /*
        je promenna ve stromu? (current_frame)
        if(st_search(current_frame, tok.attr.str.array) == NULL) {
                        error_handle(tok2.line, undefined_id)
                        abort();
        } else {
                        printf("TYPE GF@$type LF@%s\n");
                        printf("JUMPIFNEQ IS_DEF_%u GF@$type string@\n", *num);
                        printf("EXIT int@5\n");
                        printf("LABEL IS_DEF_%u\n", *num);
        }
    */
}

/**
 * Does given operation with operands left_result and (value or varieble).
 *
 */
void arith_varval(token_t *tok2, bool save_to_right, unsigned *num, const char *operation) {
    // tok1 represents result of smaller expression
    if (tok2->type == token_varieble) {
        check_existance(tok2, num);
        // tmp = var
        printf("MOVE GF@$tmp LF@%s\n", tok2->attr.str->array);
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 LF@%s\n", tok2->attr.str->array);
        // is left_result int or float?
        printf("EQ GF@$cond1 GF@$type1 string@int\n");
        printf("EQ GF@$cond2 GF@$type1 string@float\n");
        printf("OR GF@$cond1 GF@$cond1 GF@$cond2\n");
        printf("JUMPIFNEQ %s_BAD_%u GF@$cond1 bool@true\n", operation, *num);
        // is varieble int or float?
        printf("EQ GF@$cond1 GF@$type2 string@int\n");
        printf("EQ GF@$cond2 GF@$type2 string@float\n");
        printf("OR GF@$cond1 GF@$cond1 GF@$cond2\n");
        printf("JUMPIFNEQ %s_BAD_%u GF@$cond1 bool@true\n", operation, *num);
        // if(type1 == int)
        printf("JUMPIFNEQ %s_FLOAT_%u GF@$type1 string@int\n", operation, *num);
        if (!strcmp(operation, "DIV")) {
            printf("PUSHS bool@false\n");
            printf("PUSHS string@float\n");
            printf("PUSHS LF@%s\n", tok2->attr.str->array);
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS LF@%s\n", tok2->attr.str->array);
        } else {
            printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@int\n", operation, *num);
        }
        //		type2 is float, need to convert left_result to float
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);
        // else -> type1 is float
        printf("LABEL %s_FLOAT_%u\n", operation, *num);
        printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@float\n", operation, *num);
        // 		type2 is int, need to convert it to float
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS LF@%s\n", tok2->attr.str->array);
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS LF@%s\n", tok2->attr.str->array);
        printf("JUMP %s_DONE_%u\n", operation, *num);

        printf("LABEL %s_BAD_%u\n", operation, *num);
        printf("EXIT int@7\n");

        printf("LABEL %s_DONE_%u\n", operation, *num);
        if (!strcmp(operation, "DIV")) {
            printf("JUMPIFEQ %s_BAD_%u LF@%s float@%a\n", operation, *num, tok2->attr.str->array, 0.0);
        }
        if (save_to_right) {
            printf("%s GF@$right_result GF@$left_result LF@%s\n", operation, tok2->attr.str->array);
        } else {
            printf("%s GF@$left_result GF@$left_result LF@%s\n", operation, tok2->attr.str->array);
        }
        printf("MOVE LF@%s GF@$tmp\n", tok2->attr.str->array);
        (*num)++;
    } else { // tok2 is a value and is stored in var1
        if (tok2->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok2->attr.doub);
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
            printf("PUSHS bool@false\n");
            printf("PUSHS string@float\n");
            printf("PUSHS GF@$val1\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val1\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        } // tok2 type is already checked
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 GF@$val1\n");
        // is left_result int or float?
        printf("EQ GF@$cond1 GF@$type1 string@int\n");
        printf("EQ GF@$cond2 GF@$type1 string@float\n");
        printf("OR GF@$cond1 GF@$cond1 GF@$cond2\n");
        printf("JUMPIFNEQ %s_BAD_%u GF@$cond1 bool@true\n", operation, *num);
        // if(type1 == int)
        printf("JUMPIFNEQ %s_FLOAT_%u GF@$type1 string@int\n", operation, *num);
        if (!strcmp(operation, "DIV")) {
            printf("PUSHS bool@false\n");
            printf("PUSHS string@float\n");
            printf("PUSHS GF@$val1\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val1\n");
        } else {
            printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@int\n", operation, *num);
        }
        //		type2 is float, need to convert left_result to float
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);
        // else -> type1 is float
        printf("LABEL %s_FLOAT_%u\n", operation, *num);
        printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@float\n", operation, *num);
        // 		type2 is int, need to convert it to float
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);

        printf("LABEL %s_BAD_%u\n", operation, *num);
        printf("EXIT int@7\n");

        printf("LABEL %s_DONE_%u\n", operation, *num);
        if (!strcmp(operation, "DIV")) {
            printf("JUMPIFEQ %s_BAD_%u GF@val1 float@%a\n", operation, *num, 0.0);
        }
        if (save_to_right) {
            printf("%s GF@$right_result GF@$left_result GF@$val1\n", operation);
        } else {
            printf("%s GF@$left_result GF@$left_result GF@$val1\n", operation);
        }
        (*num)++;
    }
}

/**
 * Concatenate with operands left_result and (value or varieble).
 *
 */
void cat_varval(token_t *tok2, bool save_to_right, unsigned *num) {
    if (tok2->type == token_varieble) {
        check_existance(tok2, num);
        printf("MOVE GF@$tmp LF@%s\n", tok2->attr.str->array);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS LF@%s\n", tok2->attr.str->array);
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS LF@%s\n", tok2->attr.str->array);

        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        if (save_to_right) {
            printf("CONCAT GF@$right_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        } else {
            printf("CONCAT GF@$left_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        }
        printf("MOVE LF@%s GF@$tmp\n", tok2->attr.str->array);
    } else {
        if (tok2->type == token_string) {
            printf("MOVE GF@$val1 string@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
            printf("PUSHS bool@false\n");
            printf("PUSHS string@string\n");
            printf("PUSHS GF@$val1\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val1\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        if (save_to_right) {
            printf("CONCAT GF@$right_result GF@$left_result GF@$val1\n");
        } else {
            printf("CONCAT GF@$left_result GF@$left_result GF@$val1\n");
        }
    }
}

/**
 * Compares operands left_result and (value or varieble).
 *
 */
void cmp_varval(token_t *tok2, bool save_to_right, unsigned *num) {
    if (tok2->type == token_varieble) {
        check_existance(tok2, num);
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 LF@%s\n", tok2->attr.str->array);
        // neither operand can be bool
        printf("JUMPIFNEQ CMP_NOTBOOL_%u GF@$type1 string@bool\n", *num);
        printf("JUMPIFNEQ CMP_NOTBOOL_%u GF@$type2 string@bool\n", *num);
        printf("EXIT int@7\n");
        printf("LABEL CMP_NOTBOOL_%u\n", *num);

        printf("JUMPIFEQ CMP_SAMET_%u GF@$type1 GF@$type2\n", *num);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@false\n");
        } else {
            printf("MOVE GF@$left_result bool@false\n");
        }
        printf("JUMP CMP_DONE_%u\n", *num);
        printf("LABEL CMP_SAMET_%u\n", *num);
        printf("JUMPIFEQ CMP_TRUE_%u GF@$left_result LF@%s\n", *num, tok2->attr.str->array);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@false\n");
        } else {
            printf("MOVE GF@$left_result bool@false\n");
        }
        printf("JUMP CMP_DONE_%u\n", *num);
        printf("LABEL CMP_TRUE_%u\n", *num);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@true\n");
        } else {
            printf("MOVE GF@$left_result bool@true\n");
        }
        printf("LABEL CMP_DONE_%u\n", *num);
    } else {
        if (tok2->type == token_string) {
            printf("MOVE GF@$val1 string@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok2->attr.doub);
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }

        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 GF@$val1\n");
        // neither operand can be bool
        printf("JUMPIFNEQ CMP_NOTBOOL_%u GF@$type1 string@bool\n", *num);
        printf("JUMPIFNEQ CMP_NOTBOOL_%u GF@$type2 string@bool\n", *num);
        printf("EXIT int@7\n");
        printf("LABEL CMP_NOTBOOL_%u\n", *num);

        printf("JUMPIFEQ CMP_SAMET_%u GF@$type1 GF@$type2\n", *num);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@false\n");
        } else {
            printf("MOVE GF@$left_result bool@false\n");
        }
        printf("JUMP CMP_DONE_%u\n", *num);
        printf("LABEL CMP_SAMET_%u\n", *num);
        printf("JUMPIFEQ CMP_TRUE_%u GF@$left_result GF@$val1\n", *num);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@false\n");
        } else {
            printf("MOVE GF@$left_result bool@false\n");
        }
        printf("JUMP CMP_DONE_%u\n", *num);
        printf("LABEL CMP_TRUE_%u\n", *num);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@true\n");
        } else {
            printf("MOVE GF@$left_result bool@true\n");
        }
        printf("LABEL CMP_DONE_%u\n", *num);
    }
}

/**
 * Operation < with operands left_result and (value or varieble).
 *
 */
void less_varval(token_t *tok2, bool save_to_right, unsigned *num) {
    if (tok2->type == token_varieble) {
        check_existance(tok2, num);
        printf("MOVE GF@$tmp LF@%s\n", tok2->attr.str->array);
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 LF@%s\n", tok2->attr.str->array);
        // neither can be bool
        printf("JUMPIFNEQ LESS_NOTBOOL_%u string@bool GF@$type1\n", *num);
        printf("JUMPIFNEQ LESS_NOTBOOL_%u string@bool GF@$type2\n", *num);
        printf("EXIT int@7\n");
        printf("LABEL LESS_NOTBOOL_%u\n", *num);
        // if(1st op == int)
        printf("JUMPIFNEQ LESS_NOTINT_%u GF@$type1 string@int\n", *num);
        printf("JUMPIFNEQ LESS_INNOTINT_%u GF@$type2 string@int\n", *num);
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        } else {
            printf("LT GF@$left_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        printf("LABEL LESS_INNOTINT%u\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        } else {
            printf("LT GF@$left_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        printf("LABEL LESS_NOTINT_%u\n", *num);
        // if(1st op == float)
        printf("JUMPIFNEQ LESS_NOTFLOAT_%u GF@$type1 string@float\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS LF@%s\n", tok2->attr.str->array);
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS LF@%s\n", tok2->attr.str->array);
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        } else {
            printf("LT GF@$left_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        // if(1st op == string)
        printf("JUMPIFNEQ LESS_NOTSTR_%u GF@$type1 string@string\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS LF@%s\n", tok2->attr.str->array);
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS LF@%s\n", tok2->attr.str->array);
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        } else {
            printf("LT GF@$left_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        // else
        printf("JUMPIFNEQ LESS_TOBOOL_%u GF@$type2 string@string\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        } else {
            printf("LT GF@$left_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        printf("LABEL LESS_TOBOOL_%u\n", *num);
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$left_result\n");

        printf("PUSHS LF@%s\n", tok2->attr.str->array);
        printf("CALL %%cast_bool\n");
        printf("POPS LF@%s\n", tok2->attr.str->array);
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        } else {
            printf("LT GF@$left_result GF@$left_result LF@%s\n", tok2->attr.str->array);
        }
        printf("LABEL LESS_DONE_%u\n", *num);
        printf("MOVE LF@%s GF@$tmp\n", tok2->attr.str->array);
    } else {
        if (tok2->type == token_string) {
            printf("MOVE GF@$val1 string@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok2->attr.doub);
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }

        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 GF@$val1\n");
        // neither can be bool
        printf("JUMPIFNEQ LESS_NOTBOOL_%u string@bool GF@$type1\n", *num);
        printf("JUMPIFNEQ LESS_NOTBOOL_%u string@bool GF@$type2\n", *num);
        printf("EXIT int@7\n");
        printf("LABEL LESS_NOTBOOL_%u\n", *num);
        // if(1st op == int)
        printf("JUMPIFNEQ LESS_NOTINT_%u GF@$type1 string@int\n", *num);
        printf("JUMPIFNEQ LESS_INNOTINT_%u GF@$type2 string@int\n", *num);
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$val1\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$val1\n");
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        printf("LABEL LESS_INNOTINT%u\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$val1\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$val1\n");
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        printf("LABEL LESS_NOTINT_%u\n", *num);
        // if(1st op == float)
        printf("JUMPIFNEQ LESS_NOTFLOAT_%u GF@$type1 string@float\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$val1\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$val1\n");
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        // if(1st op == string)
        printf("JUMPIFNEQ LESS_NOTSTR_%u GF@$type1 string@string\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$val1\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$val1\n");
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        // else - type1 is nil@nil
        printf("JUMPIFNEQ LESS_TOBOOL_%u GF@$type2 string@string\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$val1\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$val1\n");
        }
        printf("JUMP LESS_DONE_%u\n", *num);
        printf("LABEL LESS_TOBOOL_%u\n", *num);
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$left_result\n");

        printf("PUSHS GF@$val1\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$val1\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$val1\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$val1\n");
        }
        printf("LABEL LESS_DONE_%u\n", *num);
    }
}

/**
 * Operation <= with operands left_result and (value or varieble).
 *
 */
void lesseq_varval(token_t *tok2, bool save_to_right, unsigned *num) {
    if (tok2->type == token_varieble) {
        check_existance(tok2, num);
        printf("MOVE GF@$tmp LF@%s\n", tok2->attr.str->array);
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 LF@%s\n", tok2->attr.str->array);
        // neither can be bool
        printf("JUMPIFNEQ LESSEQ_NOTBOOL_%u string@bool GF@$type1\n", *num);
        printf("JUMPIFNEQ LESSEQ_NOTBOOL_%u string@bool GF@$type2\n", *num);
        printf("EXIT int@7\n");
        printf("LABEL LESSEQ_NOTBOOL_%u\n", *num);
        // if(1st op == int)
        printf("JUMPIFNEQ LESSEQ_NOTINT_%u GF@$type1 string@int\n", *num);
        printf("JUMPIFNEQ LESSEQ_INNOTINT_%u GF@$type2 string@int\n", *num);

        printf("LT GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
        printf("EQ GF@$cond2 GF@$left_result LF@%s\n", tok2->attr.str->array);
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        printf("LABEL LESSEQ_INNOTINT%u\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");

        printf("LT GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
        printf("EQ GF@$cond2 GF@$left_result LF@%s\n", tok2->attr.str->array);
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        printf("LABEL LESSEQ_NOTINT_%u\n", *num);
        // if(1st op == float)
        printf("JUMPIFNEQ LESSEQ_NOTFLOAT_%u GF@$type1 string@float\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS LF@%s\n", tok2->attr.str->array);
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS LF@%s\n", tok2->attr.str->array);

        printf("LT GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
        printf("EQ GF@$cond2 GF@$left_result LF@%s\n", tok2->attr.str->array);
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        // if(1st op == string)
        printf("JUMPIFNEQ LESSEQ_NOTSTR_%u GF@$type1 string@string\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS LF@%s\n", tok2->attr.str->array);
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS LF@%s\n", tok2->attr.str->array);

        printf("LT GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
        printf("EQ GF@$cond2 GF@$left_result LF@%s\n", tok2->attr.str->array);
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        // else
        printf("JUMPIFNEQ LESSEQ_TOBOOL_%u GF@$type2 string@string\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");

        printf("LT GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
        printf("EQ GF@$cond2 GF@$left_result LF@%s\n", tok2->attr.str->array);
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        printf("LABEL LESSEQ_TOBOOL_%u\n", *num);
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$left_result\n");

        printf("PUSHS LF@%s\n", tok2->attr.str->array);
        printf("CALL %%cast_bool\n");
        printf("POPS LF@%s\n", tok2->attr.str->array);

        printf("LT GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
        printf("EQ GF@$cond2 GF@$left_result LF@%s\n", tok2->attr.str->array);
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("LABEL LESSEQ_DONE_%u\n", *num);
        printf("MOVE LF@%s GF@$tmp\n", tok2->attr.str->array);
    } else {
        if (tok2->type == token_string) {
            printf("MOVE GF@$val1 string@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok2->attr.doub);
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }

        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 GF@$val1\n");
        // neither can be bool
        printf("JUMPIFNEQ LESSEQ_NOTBOOL_%u string@bool GF@$type1\n", *num);
        printf("JUMPIFNEQ LESSEQ_NOTBOOL_%u string@bool GF@$type2\n", *num);
        printf("EXIT int@7\n");
        printf("LABEL LESSEQ_NOTBOOL_%u\n", *num);
        // if(1st op == int)
        printf("JUMPIFNEQ LESSEQ_NOTINT_%u GF@$type1 string@int\n", *num);
        printf("JUMPIFNEQ LESSEQ_INNOTINT_%u GF@$type2 string@int\n", *num);

        printf("LT GF@$cond1 GF@$left_result GF@$val1\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$val1\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        printf("LABEL LESSEQ_INNOTINT%u\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");

        printf("LT GF@$cond1 GF@$left_result GF@$val1\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$val1\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        printf("LABEL LESSEQ_NOTINT_%u\n", *num);
        // if(1st op == float)
        printf("JUMPIFNEQ LESSEQ_NOTFLOAT_%u GF@$type1 string@float\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");

        printf("LT GF@$cond1 GF@$left_result GF@$val1\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$val1\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        // if(1st op == string)
        printf("JUMPIFNEQ LESSEQ_NOTSTR_%u GF@$type1 string@string\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");

        printf("LT GF@$cond1 GF@$left_result GF@$val1\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$val1\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        // else
        printf("JUMPIFNEQ LESSEQ_TOBOOL_%u GF@$type2 string@string\n", *num);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");

        printf("LT GF@$cond1 GF@$left_result GF@$val1\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$val1\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", *num);
        printf("LABEL LESSEQ_TOBOOL_%u\n", *num);
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$left_result\n");

        printf("PUSHS GF@$val1\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$val1\n");

        printf("LT GF@$cond1 GF@$left_result GF@$val1\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$val1\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("LABEL LESSEQ_DONE_%u\n", *num);
    }
}

/**
 * Generates code for operations +, -, *, /.
 *
 */
void arithmetic_check(token_t *tok1, token_t *tok2, bool save_to_right, unsigned *num, const char *operation) {
    if (tok1 == NULL && tok2 == NULL) {
        // both represent results of smaller expressions
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 GF@$right_result\n");
        // is left_result int or float?
        printf("EQ GF@$cond1 GF@$type1 string@int\n");
        printf("EQ GF@$cond2 GF@$type1 string@float\n");
        printf("OR GF@$cond1 GF@$cond1 GF@$cond2\n");
        printf("JUMPIFNEQ %s_BAD_%u GF@$cond1 bool@true\n", operation, *num);
        // is varieble int or float?
        printf("EQ GF@$cond1 GF@$type2 string@int\n");
        printf("EQ GF@$cond2 GF@$type2 string@float\n");
        printf("OR GF@$cond1 GF@$cond1 GF@$cond2\n");
        printf("JUMPIFNEQ %s_BAD_%u GF@$cond1 bool@true\n", operation, *num);
        // if(type1 == int)
        printf("JUMPIFNEQ %s_FLOAT_%u GF@$type1 string@int\n", operation, *num);
        if (!strcmp(operation, "DIV")) {
            printf("PUSHS bool@false\n");
            printf("PUSHS string@float\n");
            printf("PUSHS GF@$right_result\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$right_result\n");
        } else {
            printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@int\n", operation, *num);
        }
        //		type2 is float, need to convert left_result to float
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);
        // else -> type1 is float
        printf("LABEL %s_FLOAT_%u\n", operation, *num);
        printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@float\n", operation, *num);
        // 		type2 is int, need to convert it to float
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$right_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$right_result\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);

        printf("LABEL %s_BAD_%u\n", operation, *num);
        printf("EXIT int@7 \n");

        printf("LABEL %s_DONE_%u\n", operation, *num);
        if (!strcmp(operation, "DIV")) {
            printf("JUMPIFEQ %s_BAD_%u GF@$right_result float@%a\n", operation, *num, 0.0);
        }
        if (save_to_right) {
            printf("%s GF@$right_result GF@$left_result GF@$right_result\n", operation);
        } else {
            printf("%s GF@$left_result GF@$left_result GF@$right_result\n", operation);
        }
        (*num)++;
    } else if (tok1 == NULL) {
        arith_varval(tok2, save_to_right, num, operation);
    } else if (tok2 == NULL) {
        // tok2 represents result of smaller expression
        arith_varval(tok1, save_to_right, num, operation);
    } else {
        // both toks can be value or varieble
        if (tok1->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok1->attr.integer);
        } else if (tok1->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok1->attr.doub);
        } else if (tok1->type == token_varieble) {
            check_existance(tok1, num);
            printf("TYPE GF@$type LF@%s\n", tok1->attr.str->array);
            printf("JUMPIFEQ %s_TOVAR1_%u GF@$type STRING@int\n", operation, *num);
            printf("JUMPIFEQ %s_TOVAR1_%u GF@$type STRING@float\n", operation, *num);
            printf("EXIT int@7\n");
            printf("LABEL %s_TOVAR1_%u\n", operation, *num);
            printf("MOVE GF@$val1 LF@%s\n", tok1->attr.str->array);
        } else if (tok1->type == token_keyword && tok1->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
            printf("PUSHS bool@false\n");
            printf("PUSHS string@int\n");
            printf("PUSHS GF@$val1\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val1\n");
        } else {
            error_handle(tok1->line, expr_type_error);
            abort();
        }
        if (tok2->type == token_integer) {
            printf("MOVE GF@$val2 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val2 float@%a\n", (double)tok2->attr.doub);
        } else if (tok2->type == token_varieble) {
            check_existance(tok2, num);
            printf("TYPE GF@$type LF@%s\n", tok2->attr.str->array);
            printf("JUMPIFEQ %s_TOVAR2_%u GF@$type STRING@int\n", operation, *num);
            printf("JUMPIFEQ %s_TOVAR2_%u GF@$type STRING@float\n", operation, *num);
            printf("EXIT int@7\n");
            printf("LABEL %s_TOVAR2_%u\n", operation, *num);
            printf("MOVE GF@$val2 LF@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val2 nil@nil\n");
            printf("PUSHS bool@false\n");
            printf("PUSHS string@int\n");
            printf("PUSHS GF@$val2\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val2\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }
        printf("TYPE GF@$type1 GF@$val1\n");
        printf("TYPE GF@$type2 GF@$val2\n");
        // if(type1 == int)
        printf("JUMPIFNEQ %s_FLOAT_%u GF@$type1 string@int\n", operation, *num);
        if (!strcmp(operation, "DIV")) {
            printf("PUSHS bool@false\n");
            printf("PUSHS string@float\n");
            printf("PUSHS GF@$val2\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val2\n");
        } else {
            printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@int\n", operation, *num);
        }
        //		type2 is float, need to convert val1 to float
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);
        // else -> type1 is float
        printf("LABEL %s_FLOAT_%u\n", operation, *num);
        printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@float\n", operation, *num);
        // 		type2 is int, need to convert it to float
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val2\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val2\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);

        printf("LABEL %s_BAD_%u\n", operation, *num);
        printf("EXIT int@7\n");

        printf("LABEL %s_DONE_%u\n", operation, *num);
        if (!strcmp(operation, "DIV")) {
            printf("JUMPIFEQ %s_BAD_%u GF@val2 float@%a\n", operation, *num, 0.0);
        }
        if (save_to_right) {
            printf("%s GF@$right_result GF@$val1 GF@$val2\n", operation);
        } else {
            printf("%s GF@$left_result GF@$val1 GF@$val2\n", operation);
        }
        (*num)++;
    }
}

// TODO DEFVAR right and left result + type on the start of codegen or something
// TODO DEFVAR GF@$val1 a GF@$val2
// TODO DEFVAR GF@$type GF@$type1 GF@$type2, GF@$tmp, GF@$cond1, GF@$cond2
/**
 * Generates code for operation plus.
 *
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - 1 place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_plus(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_plus = 0;
    arithmetic_check(tok1, tok2, save_to_right, &unique_num_plus, "ADD");
}

/**
 * Generates code for operation minus.
 *
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - 1 place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_minus(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_minus = 0;
    arithmetic_check(tok1, tok2, save_to_right, &unique_num_minus, "SUB");
}

/**
 * Generates code for operation multiply.
 *
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - 1 place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_mul(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_mul = 0;
    arithmetic_check(tok1, tok2, save_to_right, &unique_num_mul, "MUL");
}

/**
 * Generates code for operation divide.
 *
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - 1 place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_div(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_div = 0;
    arithmetic_check(tok1, tok2, save_to_right, &unique_num_div, "DIV");
}

/**
 * Generates code for operation concatenate.
 *
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - 1 place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_cat(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_cat = 0;
    if (tok1 == NULL && tok2 == NULL) {
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");

        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$right_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$right_result\n");
        if (save_to_right) {
            printf("CONCAT GF@$right_result GF@$left_result GF@$right_result\n");
        } else {
            printf("CONCAT GF@$left_result GF@$left_result GF@$right_result\n");
        }
    } else if (tok1 == NULL) {
        cat_varval(tok2, save_to_right, &unique_num_cat);
    } else if (tok2 == NULL) {
        cat_varval(tok1, save_to_right, &unique_num_cat);
    } else {
        if (tok1->type == token_string) {
            printf("MOVE GF@$val1 string@%s\n", tok1->attr.str->array);
        } else if (tok1->type == token_varieble) {
            check_existance(tok1, &unique_num_cat);
            printf("PUSHS bool@false\n");
            printf("PUSHS string@string\n");
            printf("PUSHS LF@%s\n", tok1->attr.str->array);
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val1\n");
        } else if (tok1->type == token_keyword && tok1->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
            printf("PUSHS bool@false\n");
            printf("PUSHS string@string\n");
            printf("PUSHS GF@$val1\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val1\n");
        } else {
            error_handle(tok1->line, expr_type_error);
            abort();
        }

        if (tok2->type == token_string) {
            printf("MOVE GF@$val2 string@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_varieble) {
            check_existance(tok2, &unique_num_cat);
            printf("PUSHS bool@false\n");
            printf("PUSHS string@string\n");
            printf("PUSHS LF@%s\n", tok2->attr.str->array);
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val2\n");
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val2 nil@nil\n");
            printf("PUSHS bool@false\n");
            printf("PUSHS string@string\n");
            printf("PUSHS GF@$val2\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val2\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }

        if (save_to_right) {
            printf("CONCAT GF@$right_result GF@$val1 GF@$val2\n");
        } else {
            printf("CONCAT GF@$left_result GF@$val1 GF@$val2\n");
        }
    }
    unique_num_cat++;
}

/**
 * Generates code for operation compare(===).
 *
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - 1 place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_cmp(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_cmp = 0;
    if (tok1 == NULL && tok2 == NULL) {
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 GF@$right_result\n");
        // neither operand can be bool
        printf("JUMPIFNEQ CMP_NOTBOOL_%u GF@$type1 string@bool\n", unique_num_cmp);
        printf("JUMPIFNEQ CMP_NOTBOOL_%u GF@$type2 string@bool\n", unique_num_cmp);
        printf("EXIT int@7\n");
        printf("LABEL CMP_NOTBOOL_%u\n", unique_num_cmp);

        printf("JUMPIFEQ CMP_SAMET_%u GF@$type1 GF@$type2\n", unique_num_cmp);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@false\n");
        } else {
            printf("MOVE GF@$left_result bool@false\n");
        }
        printf("JUMP CMP_DONE_%u\n", unique_num_cmp);
        printf("LABEL CMP_SAMET_%u\n", unique_num_cmp);
        printf("JUMPIFEQ CMP_TRUE_%u GF@$left_result GF@$right_result\n", unique_num_cmp);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@false\n");
        } else {
            printf("MOVE GF@$left_result bool@false\n");
        }
        printf("JUMP CMP_DONE_%u\n", unique_num_cmp);
        printf("LABEL CMP_TRUE_%u\n", unique_num_cmp);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@true\n");
        } else {
            printf("MOVE GF@$left_result bool@true\n");
        }
        printf("LABEL CMP_DONE_%u\n", unique_num_cmp);
    } else if (tok1 == NULL) {
        cmp_varval(tok2, save_to_right, &unique_num_cmp);
    } else if (tok2 == NULL) {
        cmp_varval(tok1, save_to_right, &unique_num_cmp);
    } else {
        if (tok1->type == token_string) {
            printf("MOVE GF@$val1 string@%s\n", tok1->attr.str->array);
        } else if (tok1->type == token_varieble) {
            check_existance(tok1, &unique_num_cmp);
            printf("MOVE GF@$val1 LF@%s\n", tok1->attr.str->array);
        } else if (tok1->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok1->attr.integer);
        } else if (tok1->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok1->attr.doub);
        } else if (tok1->type == token_keyword && tok1->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
        } else {
            error_handle(tok1->line, expr_type_error);
            abort();
        }

        if (tok2->type == token_string) {
            printf("MOVE GF@$val2 string@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_varieble) {
            check_existance(tok2, &unique_num_cmp);
            printf("MOVE GF@$val2 LF@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_integer) {
            printf("MOVE GF@$val2 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val2 float@%a\n", (double)tok2->attr.doub);
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val2 nil@nil\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }

        printf("TYPE GF@$type1 GF@$val1\n");
        printf("TYPE GF@$type2 GF@$val2\n");
        // neither operand can be bool
        printf("JUMPIFNEQ CMP_NOTBOOL_%u GF@$type1 string@bool\n", unique_num_cmp);
        printf("JUMPIFNEQ CMP_NOTBOOL_%u GF@$type2 string@bool\n", unique_num_cmp);
        printf("EXIT int@7\n");
        printf("LABEL CMP_NOTBOOL_%u\n", unique_num_cmp);

        printf("JUMPIFEQ CMP_SAMET_%u GF@$type1 GF@$type2\n", unique_num_cmp);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@false\n");
        } else {
            printf("MOVE GF@$left_result bool@false\n");
        }
        printf("JUMP CMP_DONE_%u\n", unique_num_cmp);
        printf("LABEL CMP_SAMET_%u\n", unique_num_cmp);
        printf("JUMPIFEQ CMP_TRUE_%u GF@$val1 GF@$val2\n", unique_num_cmp);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@false\n");
        } else {
            printf("MOVE GF@$left_result bool@false\n");
        }
        printf("JUMP CMP_DONE_%u\n", unique_num_cmp);
        printf("LABEL CMP_TRUE_%u\n", unique_num_cmp);
        if (save_to_right) {
            printf("MOVE GF@$right_result bool@true\n");
        } else {
            printf("MOVE GF@$left_result bool@true\n");
        }
        printf("LABEL CMP_DONE_%u\n", unique_num_cmp);
    }
    unique_num_cmp++;
}

/**
 * Generates code for operation lower <.
 *
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - 1 place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_less(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_less = 0;
    if (tok1 == NULL && tok2 == NULL) {
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 GF@$right_result\n");
        // neither can be bool
        printf("JUMPIFNEQ LESS_NOTBOOL_%u string@bool GF@$type1\n", unique_num_less);
        printf("JUMPIFNEQ LESS_NOTBOOL_%u string@bool GF@$type2\n", unique_num_less);
        printf("EXIT int@7\n");
        printf("LABEL LESS_NOTBOOL_%u\n", unique_num_less);
        // if(1st op == int)
        printf("JUMPIFNEQ LESS_NOTINT_%u GF@$type1 string@int\n", unique_num_less);
        printf("JUMPIFNEQ LESS_INNOTINT_%u GF@$type2 string@int\n", unique_num_less);
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$right_result\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$right_result\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        printf("LABEL LESS_INNOTINT%u\n", unique_num_less);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$right_result\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$right_result\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        printf("LABEL LESS_NOTINT_%u\n", unique_num_less);
        // if(1st op == float)
        printf("JUMPIFNEQ LESS_NOTFLOAT_%u GF@$type1 string@float\n", unique_num_less);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$right_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$right_result\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$right_result\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$right_result\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        // if(1st op == string)
        printf("JUMPIFNEQ LESS_NOTSTR_%u GF@$type1 string@string\n", unique_num_less);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$right_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$right_result\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$right_result\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$right_result\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        // else
        printf("JUMPIFNEQ LESS_TOBOOL_%u GF@$type2 string@string\n", unique_num_less);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$right_result\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$right_result\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        printf("LABEL LESS_TOBOOL_%u\n", unique_num_less);
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$left_result\n");

        printf("PUSHS GF@$right_result\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$right_result\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$left_result GF@$right_result\n");
        } else {
            printf("LT GF@$left_result GF@$left_result GF@$right_result\n");
        }
        printf("LABEL LESS_DONE_%u\n", unique_num_less);
    } else if (tok1 == NULL) {
        less_varval(tok2, save_to_right, &unique_num_less);
    } else if (tok2 == NULL) {
        if (save_to_right) {
			printf("NOT GF@$right_result GF@$right_result\n");
            printf("EQ GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
            printf("NOT GF@$cond1 GF@$cond1\n");
			printf("AND GF@$right_result GF@$right_result GF@$cond1\n");
        } else {
			printf("NOT GF@$left_result GF@$left_result\n");
			printf("EQ GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
            printf("NOT GF@$cond1 GF@$cond1\n");
			printf("AND GF@$left_result GF@$left_result GF@$cond1\n");
        }
        less_varval(tok1, save_to_right, &unique_num_less);
    } else {
        if (tok1->type == token_string) {
            printf("MOVE GF@$val1 string@%s\n", tok1->attr.str->array);
        } else if (tok1->type == token_varieble) {
            check_existance(tok1, &unique_num_less);
            printf("MOVE GF@$val1 LF@%s\n", tok1->attr.str->array);
        } else if (tok1->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok1->attr.integer);
        } else if (tok1->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok1->attr.doub);
        } else if (tok1->type == token_keyword && tok1->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
        } else {
            error_handle(tok1->line, expr_type_error);
            abort();
        }

        if (tok2->type == token_string) {
            printf("MOVE GF@$val2 string@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_varieble) {
            check_existance(tok2, &unique_num_less);
            printf("MOVE GF@$val2 LF@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_integer) {
            printf("MOVE GF@$val2 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val2 float@%a\n", (double)tok2->attr.doub);
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val2 nil@nil\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }

        printf("TYPE GF@$type1 GF@$val1\n");
        printf("TYPE GF@$type2 GF@$val2\n");
        // neither can be bool
        printf("JUMPIFNEQ LESS_NOTBOOL_%u string@bool GF@$type1\n", unique_num_less);
        printf("JUMPIFNEQ LESS_NOTBOOL_%u string@bool GF@$type2\n", unique_num_less);
        printf("EXIT int@7\n");
        printf("LABEL LESS_NOTBOOL_%u\n", unique_num_less);
        // if(1st op == int)
        printf("JUMPIFNEQ LESS_NOTINT_%u GF@$type1 string@int\n", unique_num_less);
        printf("JUMPIFNEQ LESS_INNOTINT_%u GF@$type2 string@int\n", unique_num_less);
        if (save_to_right) {
            printf("LT GF@$right_result GF@$val1 GF@$val2\n");
        } else {
            printf("LT GF@$left_result GF@$val1 GF@$val2\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        printf("LABEL LESS_INNOTINT%u\n", unique_num_less);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$val1 GF@$val2\n");
        } else {
            printf("LT GF@$left_result GF@$val1 GF@$val2\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        printf("LABEL LESS_NOTINT_%u\n", unique_num_less);
        // if(1st op == float)
        printf("JUMPIFNEQ LESS_NOTFLOAT_%u GF@$type1 string@float\n", unique_num_less);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val2\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val2\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$val1 GF@$val2\n");
        } else {
            printf("LT GF@$left_result GF@$val1 GF@$val2\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        // if(1st op == string)
        printf("JUMPIFNEQ LESS_NOTSTR_%u GF@$type1 string@string\n", unique_num_less);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$val2\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val2\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$val1 GF@$val2\n");
        } else {
            printf("LT GF@$left_result GF@$val1 GF@$val2\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        // else
        printf("JUMPIFNEQ LESS_TOBOOL_%u GF@$type2 string@string\n", unique_num_less);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$val1 GF@$val2\n");
        } else {
            printf("LT GF@$left_result GF@$val1 GF@$val2\n");
        }
        printf("JUMP LESS_DONE_%u\n", unique_num_less);
        printf("LABEL LESS_TOBOOL_%u\n", unique_num_less);
        printf("PUSHS GF@$val1\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$val1\n");

        printf("PUSHS GF@$val2\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$val2\n");
        if (save_to_right) {
            printf("LT GF@$right_result GF@$val1 GF@$val2\n");
        } else {
            printf("LT GF@$left_result GF@$val1 GF@$val2\n");
        }
        printf("LABEL LESS_DONE_%u\n", unique_num_less);
    }
    unique_num_less++;
}

/**
 * Generates code for operation lower-equal <=.
 *
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - 1 place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_lesseq(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_lesseq = 0;
    if (tok1 == NULL && tok2 == NULL) {
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 GF@$right_result\n");
        // neither can be bool
        printf("JUMPIFNEQ LESSEQ_NOTBOOL_%u string@bool GF@$type1\n", unique_num_lesseq);
        printf("JUMPIFNEQ LESSEQ_NOTBOOL_%u string@bool GF@$type2\n", unique_num_lesseq);
        printf("EXIT int@7\n");
        printf("LABEL LESSEQ_NOTBOOL_%u\n", unique_num_lesseq);
        // if(1st op == int)
        printf("JUMPIFNEQ LESSEQ_NOTINT_%u GF@$type1 string@int\n", unique_num_lesseq);
        printf("JUMPIFNEQ LESSEQ_INNOTINT_%u GF@$type2 string@int\n", unique_num_lesseq);

        printf("LT GF@$cond1 GF@$left_result GF@$right_result\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$right_result\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        printf("LABEL LESSEQ_INNOTINT%u\n", unique_num_lesseq);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");

        printf("LT GF@$cond1 GF@$left_result GF@$right_result\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$right_result\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        printf("LABEL LESSEQ_NOTINT_%u\n", unique_num_lesseq);
        // if(1st op == float)
        printf("JUMPIFNEQ LESSEQ_NOTFLOAT_%u GF@$type1 string@float\n", unique_num_lesseq);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$right_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$right_result\n");

        printf("LT GF@$cond1 GF@$left_result GF@$right_result\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$right_result\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        // if(1st op == string)
        printf("JUMPIFNEQ LESSEQ_NOTSTR_%u GF@$type1 string@string\n", unique_num_lesseq);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$right_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$right_result\n");

        printf("LT GF@$cond1 GF@$left_result GF@$right_result\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$right_result\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        // else
        printf("JUMPIFNEQ LESSEQ_TOBOOL_%u GF@$type2 string@string\n", unique_num_lesseq);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");

        printf("LT GF@$cond1 GF@$left_result GF@$right_result\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$right_result\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        printf("LABEL LESSEQ_TOBOOL_%u\n", unique_num_lesseq);
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$left_result\n");

        printf("PUSHS GF@$right_result\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$right_result\n");

        printf("LT GF@$cond1 GF@$left_result GF@$right_result\n");
        printf("EQ GF@$cond2 GF@$left_result GF@$right_result\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("LABEL LESSEQ_DONE_%u\n", unique_num_lesseq);
    } else if (tok1 == NULL) {
        lesseq_varval(tok2, save_to_right, &unique_num_lesseq);
    } else if (tok2 == NULL) {
        lesseq_varval(tok1, save_to_right, &unique_num_lesseq);
        if (save_to_right) {
			printf("NOT GF@$right_result GF@$right_result\n");
            printf("EQ GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
			printf("OR GF@$right_result GF@$right_result GF@$cond1\n");
        } else {
			printf("NOT GF@$left_result GF@$left_result\n");
			printf("EQ GF@$cond1 GF@$left_result LF@%s\n", tok2->attr.str->array);
			printf("OR GF@$left_result GF@$left_result GF@$cond1\n");
        }
    } else {
        if (tok1->type == token_string) {
            printf("MOVE GF@$val1 string@%s\n", tok1->attr.str->array);
        } else if (tok1->type == token_varieble) {
            check_existance(tok1, &unique_num_lesseq);
            printf("MOVE GF@$val1 LF@%s\n", tok1->attr.str->array);
        } else if (tok1->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok1->attr.integer);
        } else if (tok1->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok1->attr.doub);
        } else if (tok1->type == token_keyword && tok1->attr.keyword == keyword_null) {
            printf("MOVE GF@$val1 nil@nil\n");
        } else {
            error_handle(tok1->line, expr_type_error);
            abort();
        }

        if (tok2->type == token_string) {
            printf("MOVE GF@$val2 string@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_varieble) {
            check_existance(tok2, &unique_num_lesseq);
            printf("MOVE GF@$val2 LF@%s\n", tok2->attr.str->array);
        } else if (tok2->type == token_integer) {
            printf("MOVE GF@$val2 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val2 float@%a\n", (double)tok2->attr.doub);
        } else if (tok2->type == token_keyword && tok2->attr.keyword == keyword_null) {
            printf("MOVE GF@$val2 nil@nil\n");
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }

        printf("TYPE GF@$type1 GF@$val1\n");
        printf("TYPE GF@$type2 GF@$val2\n");
        // neither can be bool
        printf("JUMPIFNEQ LESSEQ_NOTBOOL_%u string@bool GF@$type1\n", unique_num_lesseq);
        printf("JUMPIFNEQ LESSEQ_NOTBOOL_%u string@bool GF@$type2\n", unique_num_lesseq);
        printf("EXIT int@7\n");
        printf("LABEL LESSEQ_NOTBOOL_%u\n", unique_num_lesseq);
        // if(1st op == int)
        printf("JUMPIFNEQ LESSEQ_NOTINT_%u GF@$type1 string@int\n", unique_num_lesseq);
        printf("JUMPIFNEQ LESSEQ_INNOTINT_%u GF@$type2 string@int\n", unique_num_lesseq);

        printf("LT GF@$cond1 GF@$val1 GF@$val2\n");
        printf("EQ GF@$cond2 GF@$val1 GF@$val2\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        printf("LABEL LESSEQ_INNOTINT%u\n", unique_num_lesseq);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");

        printf("LT GF@$cond1 GF@$val1 GF@$val2\n");
        printf("EQ GF@$cond2 GF@$val1 GF@$val2\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        printf("LABEL LESSEQ_NOTINT_%u\n", unique_num_lesseq);
        // if(1st op == float)
        printf("JUMPIFNEQ LESSEQ_NOTFLOAT_%u GF@$type1 string@float\n", unique_num_lesseq);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val2\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val2\n");

        printf("LT GF@$cond1 GF@$val1 GF@$val2\n");
        printf("EQ GF@$cond2 GF@$val1 GF@$val2\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        // if(1st op == string)
        printf("JUMPIFNEQ LESSEQ_NOTSTR_%u GF@$type1 string@string\n", unique_num_lesseq);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$val2\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val2\n");

        printf("LT GF@$cond1 GF@$val1 GF@$val2\n");
        printf("EQ GF@$cond2 GF@$val1 GF@$val2\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        // else
        printf("JUMPIFNEQ LESSEQ_TOBOOL_%u GF@$type2 string@string\n", unique_num_lesseq);
        printf("PUSHS bool@false\n");
        printf("PUSHS string@string\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");

        printf("LT GF@$cond1 GF@$val1 GF@$val2\n");
        printf("EQ GF@$cond2 GF@$val1 GF@$val2\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("JUMP LESSEQ_DONE_%u\n", unique_num_lesseq);
        printf("LABEL LESSEQ_TOBOOL_%u\n", unique_num_lesseq);
        printf("PUSHS GF@$val1\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$val1\n");

        printf("PUSHS GF@$val2\n");
        printf("CALL %%cast_bool\n");
        printf("POPS GF@$val2\n");

        printf("LT GF@$cond1 GF@$val1 GF@$val2\n");
        printf("EQ GF@$cond2 GF@$val1 GF@$val2\n");
        if (save_to_right) {
            printf("OR GF@$right_result GF@$cond1 GF@$cond2\n");
        } else {
            printf("OR GF@$left_result GF@$cond1 GF@$cond2\n");
        }
        printf("LABEL LESSEQ_DONE_%u\n", unique_num_lesseq);
    }
    unique_num_lesseq++;
}

void gen_expression(exprll *ll) {
    printf("MOVE GF@$left_result nil@nil\n");
    while (1) {
        exprll *rule_node = exprll_leftmost_rule(ll);
        if (rule_node == NULL) {
            // TODO left_result to temp_var_count
            return;
        }
        switch (rule_node->rule) {
            case erule_plus:
                gen_plus(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                break;
            case erule_minus:
                gen_minus(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                break;
            case erule_div:
                gen_div(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                break;
            case erule_mul:
                gen_mul(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                break;
            case erule_cat:
                gen_cat(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                break;
            case erule_comp:
                gen_cmp(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                break;
            case erule_comp_neg:
                gen_cmp(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                if (gen_expr_different_place(rule_node, ll)) {
                    printf("NOT GF@$right_result GF@$right_result\n");
                } else {
                    printf("NOT GF@$left_result GF@$left_result\n");
                }
                break;
            case erule_lower:
                gen_less(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                break;
            case erule_lower_equal:
                gen_lesseq(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                break;
            case erule_greater:
                gen_lesseq(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                if (gen_expr_different_place(rule_node, ll)) {
                    printf("NOT GF@$right_result GF@$right_result\n");
                } else {
                    printf("NOT GF@$left_result GF@$left_result\n");
                }
                break;
            case erule_greater_equal:
                gen_less(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                if (gen_expr_different_place(rule_node, ll)) {
                    printf("NOT GF@$right_result GF@$right_result\n");
                } else {
                    printf("NOT GF@$left_result GF@$left_result\n");
                }
                break;
            case erule_brackets:
                break;
        } // end of rule switch
        exprll_del_next(rule_node);
        exprll_del_next(rule_node);
        rule_node->rule = erule_val;
        rule_node->ptok = NULL;
    }
}