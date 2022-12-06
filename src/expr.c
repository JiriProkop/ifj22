#include "expr.h"
#include "error.h"
#include "expr_codegen.h"
#include "ll.h"
#include "parser.h"
#include "scanner.h"
#include "stack.h"
#include "symtable.h"
#include <stdbool.h>
#include <stdio.h>

#define PREC_TABLE_SIZE 8

/**
 * @enum precedence expression parser table position
 */
typedef enum {
    pos_plus,
    pos_mul,
    pos_cmp_type,
    pos_cpm_val,
    pos_left_par,
    pos_right_par,
    pos_val,
    pos_dollar,
} prec_tab_pos;

/**
 * Function allocates new token and pushes it to given stack.
 *
 * @param pstk pointer to initialized stack
 * @param tok token which will be pushed to stack
 * @return Returns false if error was encountered, true otherwise.
 */
bool allocpush_token_stack(stack *pstk, token_t tok) {
    token_t *tmp = malloc(sizeof(token_t));
    if (tmp == NULL) {
        error_handle(0, compiler_error);
        stack_dispose_all(pstk);
        return false;
    } else {
        *tmp = tok;
        stack_push(pstk, tmp);
    }
    return true;
}

/**
 * Function finds right line or column number(in precedent table) for given token.
 *
 * @param colrow pointer to unsigned int
 * @param tok pointer to token
 * @return Returns false if error was encountered, true otherwise.
 */
bool get_colrow(unsigned *colrow, token_t *tok) {
    if (tok->type == token_plus || tok->type == token_minus || tok->type == token_dot) {
        *colrow = pos_plus;
    } else if (tok->type == token_multiply || tok->type == token_division) {
        *colrow = pos_mul;
    } else if (tok->type == token_compare || tok->type == token_compare_neg) {
        *colrow = pos_cmp_type;
    } else if (tok->type == token_greater || tok->type == token_greater_equal || tok->type == token_lower || tok->type == token_lower_equal) {
        *colrow = pos_cpm_val;
    } else if (tok->type == token_parentheses_left) {
        *colrow = pos_left_par;
    } else if (tok->type == token_parentheses_right) {
        *colrow = pos_right_par;
    } else if (tok->type == token_string || tok->type == token_float || tok->type == token_integer || tok->type == token_varieble) {
        *colrow = pos_val;
    } else if (tok->type == token_expr_dollar || tok->type == token_semicol || tok->type == token_comma) {
        *colrow = pos_dollar;
    } else {
        error_handle(tok->line, syntax_error);
        return false;
    }
    return true;
}

/**
 * Function implements 'reduction' operation of precedent expression parsing.
 *
 * @param pstk pointer to initialized stack
 * @return Returns false if error was encountered, true otherwise.
 */
bool reduction(stack *pstk, exprll **ll) {
    unsigned op_cnt = tokens_to_shift(pstk);
    token_t op = {.type = token_expr_e};
    int rule;
    if (op_cnt == 1) {
        token_t *tmp = stack_top_terminal(pstk);
        if (tmp->type == token_integer || tmp->type == token_float || tmp->type == token_string || tmp->type == token_varieble) {
            rule = erule_val;
        } else {
            error_handle(tmp->line, syntax_error);
            return false;
        }
        token_t *tok = stack_save_pop(pstk);
        if (!exprll_add(ll, rule, tok)) {
            expll_dispose(*ll);
            stack_dispose_all(pstk);
            abort();
        }
        stack_pop(pstk);
        if (!allocpush_token_stack(pstk, op)) {
            return false;
        }
    } else if (op_cnt == 3) { // all other operations
        token_t *tok1 = stack_save_pop(pstk);
        token_t *tok2 = stack_save_pop(pstk);
        token_t *tok3 = stack_save_pop(pstk);
        stack_pop(pstk); // popping '<'
        bool to_push = true;
        switch (tok2->type) {
            case token_plus:
                rule = erule_plus;
                break;
            case token_minus:
                rule = erule_minus;
                break;
            case token_division:
                rule = erule_div;
                break;
            case token_multiply:
                rule = erule_mul;
                break;
            case token_dot:
                rule = erule_cat;
                break;
            case token_compare:
                rule = erule_comp;
                break;
            case token_compare_neg:
                rule = erule_comp_neg;
                break;
            case token_greater:
                rule = erule_greater;
                break;
            case token_greater_equal:
                rule = erule_greater_equal;
                break;
            case token_lower:
                rule = erule_lower;
                break;
            case token_lower_equal:
                rule = erule_lower_equal;
                break;
            case token_expr_e:
                rule = erule_brackets;
                to_push = false;
                break;
            default:
                error_handle(tok1->line, syntax_error);
                return false;
        }
        if (!allocpush_token_stack(pstk, op)) {
            return false;
        }
        if (to_push && !exprll_add(ll, rule, NULL)) {
            expll_dispose(*ll);
            stack_dispose_all(pstk);
            abort();
        }
        free(tok1);
        free(tok2);
        free(tok3);
    }

    return true;
}

/**
 * Function checks if only '$E' is in stack.
 *
 * @param pstk pointer to initialized stack
 * @return Returns false if more, less or different tokens are found, true otherwise.
 */
void final_check(stack *pstk) {
    if (tokens_to_shift(pstk) != NO_SHIFT) {
        stack_dispose_all(pstk);
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
    stack_pop(pstk);
    stack_pop(pstk);
    if (!stack_is_empty(pstk)) {
        stack_dispose_all(pstk);
        error_handle(current_tkn->line, syntax_error);
        abort();
    }
}

bool expr(token_t first_tok, token_t *second_tok) {
    char prec_table[PREC_TABLE_SIZE][PREC_TABLE_SIZE] = {
        //+	   *   ===   >=   (    )    i    $
        {'>', '<', '>', '>', '<', '>', '<', '>'}, // +, -, .
        {'>', '>', '>', '>', '<', '>', '<', '>'}, // *, /
        {'<', '<', '>', '<', '<', '>', '<', '>'}, // ===, !==
        {'<', '<', '>', '>', '<', '>', '<', '>'}, // >, >=, <, <=
        {'<', '<', '<', '<', '<', '=', '<', '0'}, // (
        {'>', '>', '>', '>', '0', '>', '0', '>'}, // )
        {'>', '>', '>', '>', '0', '>', '0', '>'}, // i
        {'<', '<', '<', '<', '<', '0', '<', '0'}, // $
    };
    stack stk;
    stack_init(&stk);
    token_t tok = {.type = token_expr_dollar, .line = current_tkn->line};
    if (!allocpush_token_stack(&stk, tok)) {
        return false;
    }
    bool toread = false; // 1st token is given by parser
    bool set = false;
    bool second_read;
    if (second_tok) {
        second_read = false;
    } else {
        second_read = true;
    }
    unsigned row, col;
    unsigned par_left;      // '(' count
    unsigned par_right = 0; // ')' count
    if (first_tok.type == token_parentheses_left) {
        par_left = 1;
    } else {
        par_left = 0;
    }
    if (second_tok && second_tok->type == token_parentheses_right) {
        par_right = 1;
    } else {
        par_right = 0;
    }
    tok = first_tok;
    char c;
    exprll *ll;
    exprll_init(&ll);

    while (1) {
        token_t *top_term = stack_top_terminal(&stk);
        if (toread && !second_read) {
            toread = false;
            tok = *second_tok;
            second_read = true;
        }
        if ((toread && !get_token(&tok)) || !get_colrow(&row, top_term) || !get_colrow(&col, &tok)) {
            stack_dispose_all(&stk);
            return false;
        }
        if (toread && tok.type == token_parentheses_left) {
            par_left++;
        } else if (toread && tok.type == token_parentheses_right) {
            par_right++;
        }
        if ((tok.type == token_parentheses_right && par_right - par_left == 1) ||
            tok.type == token_comma) {
            // for if(expr) doesn't end with semicolon
            *current_tkn = tok;
            set = true;
            tok.type = token_semicol;
        }
        if (tok.type == token_semicol && top_term->type == token_expr_dollar) {
            if (!set) {
                *current_tkn = tok;
            }
            final_check(&stk);
            gen_expression(ll);
            return true;
        }
        c = prec_table[row][col];
        switch (c) {
            case '<': // shift
                stack_insert_shift(&stk);
                /* FALLTHROUGH */
            case '=':
                if (!allocpush_token_stack(&stk, tok)) {
                    return false;
                }
                toread = true;
                break;
            case '>': // reduction
                if (!reduction(&stk, &ll)) {
                    return false;
                }
                toread = false;
                break;

            default: // when c == 0
                stack_dispose_all(&stk);
                return false;
        }
    }
    return true;
}