#include "expr.h"
#include "error.h"
#include "parser.h"
#include "scanner.h"
#include "stack.h"
#include <stdbool.h>

#define PREC_TABLE_SIZE 8

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

bool stack_allocpush_token(stack *pstk, token_t tok) {
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
    } else if (tok->type == token_string || tok->type == token_float || tok->type == token_integer || tok->type == token_identifier) {
        *colrow = pos_val;
    } else if (tok->type == token_expr_dollar) {
        *colrow = pos_dollar;
    } else {
        // TODO chyba
        return false;
    }
    return true;
}

bool reduction(stack *pstk) {
    unsigned op_cnt = stack_op_cnt();
    if (op_cnt == 1) { // TODO sem. kontrola
        printf("%d", erule_id);
        stack_pop(pstk);
        stack_pop(pstk);
        token_t op = {.type = token_expr_e};
        if (!stack_allocpush_token(pstk, op)) {
            return false;
        }
    } else if(op_cnt == 3) { // all other operations
		
	}
}

bool expr(token_t first_tok) {
    char prec_table[PREC_TABLE_SIZE][PREC_TABLE_SIZE] = {
        //+	   *   ===   >=   (    )    i    $
        {'>', '<', '>', '>', '<', '>', '<', '>'}, // +, -, .
        {'>', '>', '>', '>', '<', '>', '<', '>'}, // *, /
        {'<', '<', '<', '<', '<', '>', '<', '>'}, // ===, !==
        {'<', '<', '>', '>', '<', '>', '<', '>'}, // >, >=, <, <=
        {'<', '<', '<', '<', '<', '=', '<', '0'}, // (
        {'>', '>', '>', '>', '0', '>', '0', '>'}, // )
        {'>', '>', '>', '>', '0', '>', '0', '>'}, // i
        {'<', '<', '<', '<', '<', '0', '<', '0'}, // $
    };
    stack stk;
    stack_init(&stk);
    token_t dolar = {.type = token_expr_dollar};
    if (!stack_allocpush_token(&stk, dolar)) {
        return false;
    }
    bool first_tok_read = false; // 1st token is given by parser
    bool done = false;
    unsigned row, col;
    token_t tok = first_tok;
    token_t op;
    char c;
    printf("Expr. parser pravy rozbor: ");
    while (!done) {
        token_t *top_term = stack_top_terminal(&stk);
        if (first_tok_read && !get_token(&tok) || !get_colrow(&row, top_term) || !get_colrow(&col, &tok)) {
            stack_dispose_all(&stk);
            return false;
        }
        first_tok_read = true;
        c = prec_table[row][col];
        switch (c) {
            case '<': // shift
                op.type = token_expr_shift;
                if (!stack_allocpush_token(&stk, op)) {
                    return false;
                }
            case '=':
                if (!stack_allocpush_token(&stk, tok)) {
                    return false;
                }
                break;
            case '>': // reduction

            default: // checkuj jestli neni konec, jinak chyba
        }
    }
}
// TODO implicitni konverze
// TODO semanticke kontroly