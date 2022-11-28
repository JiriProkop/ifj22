#include <stdio.h>
#include "generator.h"
#include "dynstr.h"
#include "ll.h"

void gen_header() {
    printf(".IFJcode22\n");
}

void gen_function_def(dynstr_t *id, list_t *arguments) {
    printf("JUMP %s_end\n", id->array);
    printf("LABEL %s\n", id->array);
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    while(arguments->first != NULL) {

    }
}