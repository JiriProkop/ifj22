#include <stdio.h>

#include "error.h"

extern int ret; // to be returned by main

void error_handle(unsigned line, unsigned error_type) {
    if (ret == 0) // just 1st error is returned by compilator
        ret = error_type;

    // but each error is printed
    switch (error_type) {
        case lex_analysis_err:
            fprintf(stderr, "Lexical analysis error on line: %u\n", line); // FIXME better error text
            break;
        case syntax_error:
            fprintf(stderr, "Syntax error on line: %u\n", line);
            break;
        case func_def_error:
            fprintf(stderr, "Undefined function or redefinition of one on line: %u\n", line);
            break;
        case func_arr_or_ret_error:
            fprintf(stderr, "Wrong function argument type and/or count or wrong return type on line: %u\n", line);
            break;
        case undefied_identifier_error:
            fprintf(stderr, "Undefined identifier on line: %u\n", line);
            break;
        case ret_expr_cnt_error:
            fprintf(stderr, "Wrong count of expressions in return (should be 1 or 0) on line: %u\n", line);
            break;
        case expr_type_error:
            fprintf(stderr, "Type compability error in arithmetic, string or relation exppression on line: %u \n", line);
            break;
        case other_semantic_error:
            fprintf(stderr, "Semantic error on line: %u'\n'", line);
            break;
        case compiler_error:
            fprintf(stderr, "Internal compiler error.'\n'");
            break;
    }
}