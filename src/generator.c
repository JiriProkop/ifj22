#include "generator.h"
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

// void gen_header() {
//     printf(".IFJcode22\n");
// }

// void gen_function_def(dynstr_t *id, list_t *parameters) {
//     printf("JUMP %s_end\n", id->array);
//     list_t *i = parameters->first;
//     while (i != NULL) {
//         printf("DEFVAR LF@%%s_%s", id->array, i->first->id->array);
//         i = i->first->next;
//     }
//     printf("LABEL %s_start\n", id->array);
//     printf("CREATEFRAME\n");
//     i = parameters->first;
//     while (i != NULL) {
//         printf("DEFVAR TF@s", i->first->id->array);
//         printf("MOVE TF@%s LF@%%s_%s", i->first->id->array, id->array, i->first->id->array);
//     }
//     printf("PUSHFRAME\n");
// }

// void gen_function_defend(dynstr_t *id) {
//     printf("POPFRAME\n");
//     printf("RETURN\n");
//     printf("LABEL %s_end\n", id->array);
// }

// void gen_function_call(dynstr_t *id, list_t *parameters) {
// }

/**
 * @brief Tells where to save result of expression.
 *
 * @return Returns true, if result should be stored in right_result. False otherwise.
 */
bool gen_expr_different_place(exprll *rule_node, exprll *ll) {
    if (rule_node->next->next->next != NULL && rule_node->next->next->next->rule == erule_val && rule_node->next->next->next->ptok == NULL && !exprll_prev_value(rule_node, ll)) {
        return true;
    }
    return false;
}

void var_or_val(token_t *tok2, bool save_to_right, unsigned *num, const char *operation) {
    // tok1 represents result of smaller expression
    if (tok2->type == token_varieble) {
        // TODO is tok2 defined?
        // tmp = var
        printf("MOVE GF@$tmp GF@%s\n", tok2->attr.str->array);
        printf("TYPE GF@$type1 GF@$left_result\n");
        printf("TYPE GF@$type2 GF@%s\n", tok2->attr.str->array);
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
            printf("PUSHS bool@true\n");
            printf("PUSHS string@float\n");
            printf("PUSHS GF@%s\n", tok2->attr.str->array);
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@%s\n", tok2->attr.str->array);
        } else {
            printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@int\n", operation, *num);
        }
        //		type2 is float, need to convert left_result to float
        printf("PUSHS bool@true\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);
        // else -> type1 is float
        printf("LABEL %s_FLOAT_%u\n", operation, *num);
        printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@float\n", operation, *num);
        // 		type2 is int, need to convert it to float
        printf("PUSHS bool@true\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@%s\n", tok2->attr.str->array);
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@%s\n", tok2->attr.str->array);
        printf("JUMP %s_DONE_%u\n", operation, *num);

        printf("LABEL %s_BAD_%u\n", operation, *num);
        printf("EXIT int@7\n");

        printf("LABEL %s_DONE_%u\n", operation, *num);
        if (save_to_right) {
            printf("%s GF@$right_result GF@$left_result GF@%s\n", operation, tok2->attr.str->array);
        } else {
            printf("%s GF@$left_result GF@$left_result GF@%s\n", operation, tok2->attr.str->array);
        }
        printf("MOVE GF@%s GF@$tmp\n", tok2->attr.str->array);
        (*num)++;
    } else { // tok2 is a value and is stored in var1
        if (tok2->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok2->attr.doub);
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
            printf("PUSHS bool@true\n");
            printf("PUSHS string@float\n");
            printf("PUSHS GF@$val1\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val1\n");
        } else {
            printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@int\n", operation, *num);
        }
        //		type2 is float, need to convert left_result to float
        printf("PUSHS bool@true\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);
        // else -> type1 is float
        printf("LABEL %s_FLOAT_%u\n", operation, *num);
        printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@float\n", operation, *num);
        // 		type2 is int, need to convert it to float
        printf("PUSHS bool@true\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);

        printf("LABEL %s_BAD_%u\n", operation, *num);
        printf("EXIT int@7\n");

        printf("LABEL %s_DONE_%u\n", operation, *num);
        if (save_to_right) {
            printf("%s GF@$right_result GF@$left_result GF@$val1\n", operation);
        } else {
            printf("%s GF@$left_result GF@$left_result GF@$val1\n", operation);
        }
        (*num)++;
    }
}
// TODO variebles are in local frame
void plus_check(token_t *tok1, token_t *tok2, bool save_to_right, unsigned *num, const char *operation) {
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
            printf("PUSHS bool@true\n");
            printf("PUSHS string@float\n");
            printf("PUSHS GF@$right_result\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$right_result\n");
        } else {
            printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@int\n", operation, *num);
        }
        //		type2 is float, need to convert left_result to float
        printf("PUSHS bool@true\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$left_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$left_result\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);
        // else -> type1 is float
        printf("LABEL %s_FLOAT_%u\n", operation, *num);
        printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@float\n", operation, *num);
        // 		type2 is int, need to convert it to float
        printf("PUSHS bool@true\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$right_result\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$right_result\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);

        printf("LABEL %s_BAD_%u\n", operation, *num);
        printf("EXIT int@7 \n");

        printf("LABEL %s_DONE_%u\n", operation, *num);
        if (save_to_right) {
            printf("%s GF@$right_result GF@$left_result GF@$right_result\n", operation);
        } else {
            printf("%s GF@$left_result GF@$left_result GF@$right_result\n", operation);
        }
        (*num)++;
    } else if (tok1 == NULL) {
        var_or_val(tok2, save_to_right, num, operation);
    } else if (tok2 == NULL) {
        // tok2 represents result of smaller expression
        // TODO is tok1 defined?
        var_or_val(tok1, save_to_right, num, operation);
    } else {
        // both toks can be value or varieble
        // TODO are both defined(if variebles)?
        if (tok1->type == token_integer) {
            printf("MOVE GF@$val1 int@%d\n", tok1->attr.integer);
        } else if (tok1->type == token_float) {
            printf("MOVE GF@$val1 float@%a\n", (double)tok1->attr.doub);
        } else if (tok1->type == token_varieble) {
            printf("TYPE GF@$type GF@%s\n", tok1->attr.str->array);
            printf("JUMPIFEQ %s_TOVAR1_%u GF@$type STRING@int\n", operation, *num);
            printf("JUMPIFEQ %s_TOVAR1_%u GF@$type STRING@float\n", operation, *num);
            printf("EXIT int@7\n");
            printf("LABEL %s_TOVAR1_%u\n", operation, *num);
            printf("MOVE GF@$val1 GF@%s\n", tok1->attr.str->array);
        } else {
            error_handle(tok1->line, expr_type_error);
            abort();
        }
        if (tok2->type == token_integer) {
            printf("MOVE GF@$val2 int@%d\n", tok2->attr.integer);
        } else if (tok2->type == token_float) {
            printf("MOVE GF@$val2 float@%a\n", (double)tok2->attr.doub);
        } else if (tok2->type == token_varieble) {
            printf("TYPE GF@$type GF@%s\n", tok2->attr.str->array);
            printf("JUMPIFEQ %s_TOVAR2_%u GF@$type STRING@int\n", operation, *num);
            printf("JUMPIFEQ %s_TOVAR2_%u GF@$type STRING@float\n", operation, *num);
            printf("EXIT int@7\n");
            printf("LABEL %s_TOVAR2_%u\n", operation, *num);
            printf("MOVE GF@$val2 GF@%s\n", tok2->attr.str->array);
        } else {
            error_handle(tok2->line, expr_type_error);
            abort();
        }
        printf("TYPE GF@$type1 GF@$val1\n");
        printf("TYPE GF@$type2 GF@$val2\n");
        // if(type1 == int)
        printf("JUMPIFNEQ %s_FLOAT_%u GF@$type1 string@int\n", operation, *num);
		if (!strcmp(operation, "DIV")) {
            printf("PUSHS bool@true\n");
            printf("PUSHS string@float\n");
            printf("PUSHS GF@$val2\n");
            printf("CALL %%TYPE_CASTING\n");
            printf("POPS GF@$val2\n");
        } else {
            printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@int\n", operation, *num);
        }
        //		type2 is float, need to convert val1 to float
        printf("PUSHS bool@true\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val1\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val1\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);
        // else -> type1 is float
        printf("LABEL %s_FLOAT_%u\n", operation, *num);
        printf("JUMPIFEQ %s_DONE_%u GF@$type2 string@float\n", operation, *num);
        // 		type2 is int, need to convert it to float
        printf("PUSHS bool@true\n");
        printf("PUSHS string@float\n");
        printf("PUSHS GF@$val2\n");
        printf("CALL %%TYPE_CASTING\n");
        printf("POPS GF@$val2\n");
        printf("JUMP %s_DONE_%u\n", operation, *num);

        printf("LABEL %s_BAD_%u\n", operation, *num);
        printf("EXIT int@7\n");

        printf("LABEL %s_DONE_%u\n", operation, *num);
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
 * @brief Generates code for operation plus.
 *
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_plus(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_plus = 0;
    plus_check(tok1, tok2, save_to_right, &unique_num_plus, "ADD");
}

void gen_minus(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_minus = 0;
    plus_check(tok1, tok2, save_to_right, &unique_num_minus, "SUB");
}

void gen_mul(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_mul = 0;
    plus_check(tok1, tok2, save_to_right, &unique_num_mul, "MUL");
}

void gen_div(token_t *tok1, token_t *tok2, bool save_to_right) {
    static unsigned unique_num_div = 0;
    // TODO tok2 cannot be 0
    plus_check(tok1, tok2, save_to_right, &unique_num_div, "DIV");
}

void gen_expression(exprll *ll) {
    while (1) {
        exprll *rule_node = exprll_leftmost_rule(ll);
        if (rule_node == NULL) {
            // left_result to temp_var_count
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
                break;
            case erule_comp:
                break;
            case erule_comp_neg:
                break;
            case erule_lower:
                break;
            case erule_lower_equal:
                break;
            case erule_greater:
                break;
            case erule_greater_equal:
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