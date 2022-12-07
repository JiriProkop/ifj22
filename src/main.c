/**
 * Project: compiler of IFJ22 programming language
 *
 * @file main.c
 * @brief The main file, that calls the parser
 * 
 * @author Patrik Čerbák <xcerba00@stud.fit.vutbr.cz>
 */

#include "scanner.h"
#include "parser.h"
#include "generator.h"
#include "error.h"
#include "expr.h"
#include "symtable.h"
#include "dynstr.h"
#include "stack.h"
#include "ll.h"
#include "expr_codegen.h"

int main() {
    start(); // call the parser
    abort(); // clean and exit
    return ret;
}