/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief error handler
 *
 * @author Jiří Prokop xproko47 <xproko47@stud.fit.vutbr.cz>
 */

#ifndef _ERROR_H
#define _ERROR_H

/**
 * A global variable used for returning errnumber.
*/
extern int ret;

/**
 * @enum
 */
typedef enum {
    lex_analysis_err = 1, // chybna struktura aktualniho lexemu
    syntax_error,
    func_def_error,        // undefined function or redefinition of one
    func_arr_or_ret_error, // wrong argument type or argument count, wrong return type
    undefied_identifier_error,
    ret_expr_cnt_error, // wrong count of expressions in return
    expr_type_error,    // wrong type in expression (arithmetic, relation or string)
    other_semantic_error,
    compiler_error = 99 // internal compiler error such as malloc error
} error_types;

/**
 * Writes error message to stderr.
 *
 * @param line Line on which error eccured
 * @param error_type Type of error defined in error_types
 */
void error_handle(unsigned line, unsigned error_type);

#endif // end of _ERROR_H