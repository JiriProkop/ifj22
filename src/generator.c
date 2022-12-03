#include "generator.h"
#include "dynstr.h"
#include "expr.h"
#include "ll.h"
#include "parser.h"
#include "stack.h"
#include "symtable.h"
#include <stdbool.h>
#include <stdio.h>

void gen_header() {
    printf(".IFJcode22\n");
}

void gen_function_def(dynstr_t *id, list_t *parameters) {
    printf("JUMP %s_end\n", id->array);
    list_t *i = parameters->first;
    while (i != NULL) {
        printf("DEFVAR LF@%%s_%s", id->array, i->first->id->array);
        i = i->first->next;
    }
    printf("LABEL %s_start\n", id->array);
    printf("CREATEFRAME\n");
    i = parameters->first;
    while (i != NULL) {
        printf("DEFVAR TF@s", i->first->id->array);
        printf("MOVE TF@%s LF@%%s_%s", i->first->id->array, id->array, i->first->id->array);
    }
    printf("PUSHFRAME\n");
}

void gen_function_defend(dynstr_t *id) {
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL %s_end\n", id->array);
}

void gen_function_call(dynstr_t *id, list_t *parameters) {
}

/**
 * @brief Tells where to save result of expression.
 * 
 * @return Returns true, if result should be stored in right_result. False otherwise.
 */
bool gen_expr_different_place(exprll *rule_node, exprll *ll) {
    if (rule_node->next->rule == 0 && rule_node->next->ptok == NULL && exprll_prev_value(rule_node, ll)) {
        return true;
    }
    return false;
}

void gen_expression(exprll *ll) {
    while (1) {
        exprll *rule_node = exprll_leftmost_rule(ll);
        if (rule_node == NULL) {
            return;
        }
        switch (rule_node->rule) {
            case erule_plus:
                gen_plus(rule_node->next->next->ptok, rule_node->next->ptok, gen_expr_different_place(rule_node, ll));
                break;
            case erule_minus:
                break;
            case erule_div:
                break;
            case erule_mul:
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
    }
}

// TODO check types
// TODO implicitni konverze
// TODO DEFVAR right and left result on the start of codegen or something
// TODO DEFVAR var1 and var2, type result on the start of codegen or something
/**
 * @brief Generates code for operation plus.
 * 
 * @param tok1 pointer to token - 2 places after operation rule.
 * @param tok2 pointer to token - place after operation rule.
 * @param save_to_right Bool - is true, if the result is to be stored in right_result(otherwise in left_result).
 */
void gen_plus(token_t *tok1, token_t *tok2, bool save_to_right) {
    if (tok1 == NULL && tok2 == NULL) {
        // both represent results of smaller expressions
        if (save_to_right) {
            printf("ADD GF@$right_result GF@$left_result GF@$right_result");
        } else {
            printf("ADD GF@$left_result GF@$left_result GF@$right_result");
        }
    } else if (tok1 == NULL) {
        // tok1 represents result of smaller expression
        // TODO is tok2 defined?
        if (save_to_right) {
            printf("ADD GF@$right_result GF@$left_result GF@$var1");
        } else {
            printf("ADD GF@$left_result GF@$left_result GF@$var1");
        }
    } else if (tok2 == NULL) {
        // tok2 represents result of smaller expression
        // TODO is tok1 defined?
        if (save_to_right) {
            printf("ADD GF@$right_result GF@$left_result GF@$var1");
        } else {
            printf("ADD GF@$left_result GF@$left_result GF@$var1");
        }
    } else {
        // both toks are values or variebles
        // TODO are both defined?
        if (save_to_right) {
            printf("ADD GF@$right_result GF@$var1 GF@$var2");
        } else {
            printf("ADD GF@$left_result GF@$var1 GF@$var2");
        }
    }
}

void gen_minus(token_t* tok1, token_t* tok2, bool save_to_right) {
	   if (tok1 == NULL && tok2 == NULL) {
        // both represent results of smaller expressions
        if (save_to_right) {
            //save the result to the GF@$right_result
        } else {
            //save the result to the GF@$left_result
        }
    } else if (tok1 == NULL) {
        // tok1 represents result of smaller expression(is in left_result)
        // TODO is tok2 defined?
        if (save_to_right) {
            
        } else {
            
        }
    } else if (tok2 == NULL) {
        // tok2 represents result of smaller expression
        // TODO is tok1 defined?
        if (save_to_right) {
            
        } else {
            
        }
    } else {
        // both toks are values or variebles
        // TODO are both defined?
        if (save_to_right) {
            
        } else {
            
        }
    }
}