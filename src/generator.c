#include <stdio.h>
#include "generator.h"
#include "dynstr.h"
#include "ll.h"

void gen_header() {
    printf(".IFJcode22\n");
}

void gen_function_def(dynstr_t *id) {
    printf("JUMP %s_end\n", id->array);
    printf("LABEL %s_start\n", id->array);
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
}

void gen_function_defend(dynstr_t *id) {
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL %s_end\n", id->array);
}