#include <stdlib.h>
#include <stdio.h>
#include "dynstr.h"

int main() {
    printf("--- [DYNSTR TESTS] ---\n");

    dynstr_t *string = malloc(sizeof(dynstr_t));
    if(string == NULL) {
        printf("Malloc failed.\n");
        return 1;
    }

    // test initialization
    printf("\n[dynstr init test]\n");
    int return_val = dynstr_init(string);
    printf("String: '%s'\n", string->array);
    printf("Length: %lu\n", string->length);
    printf("Allocated: %lu\n", string->allocated);
    printf("Returned: %d\n", return_val);

    // test adding characters
    printf("\n[dynstr add char test]\n");
    printf("Adding some characters:\n");
    dynstr_add_char(string, 'h');
    dynstr_add_char(string, 'e');
    dynstr_add_char(string, 'l');
    dynstr_add_char(string, 'l');
    dynstr_add_char(string, 'o');
    printf("String: '%s'\n", string->array);
    printf("Length: %lu\n", string->length);
    printf("Allocated: %lu\n", string->allocated);

    printf("\nFilling the free space before reallocation:\n");
    while(string->length != string->allocated) {
        dynstr_add_char(string, 'X');
    }
    printf("String: '%s'\n", string->array);
    printf("Length: %lu\n", string->length);
    printf("Allocated: %lu\n", string->allocated);

    printf("\nAdding one more character:\n");
    return_val = dynstr_add_char(string, '+');
    printf("String: '%s'\n", string->array);
    printf("Length: %lu\n", string->length);
    printf("Allocated: %lu\n", string->allocated);
    printf("Returned: %d\n", return_val);

    // test compare
    printf("\n[dynstr compare test]\n");
    printf("Compare with different string\n");
    return_val = dynstr_compare(string, "somerandomstring");
    printf("Returned: %d\n", return_val);
    printf("Compare with same string\n");
    return_val = dynstr_compare(string, "helloXXXXXXXXXX+");
    printf("Returned: %d\n", return_val);

    // test clear
    printf("\n[dynstr clear test]\n");
    dynstr_clear(string);
    printf("String: '%s'\n", string->array);
    printf("Length: %lu\n", string->length);
    printf("Allocated: %lu\n", string->allocated);

    // test delete
    printf("\n[dynstr clear test]\n");
    printf("First adding some characters\n");
    dynstr_add_char(string, '1');
    dynstr_add_char(string, '2');
    printf("String: '%s'\n", string->array);

    printf("And deleting the dynstr\n");
    dynstr_delete(string);
    printf("Length: %lu\n", string->length);
    printf("Allocated: %lu\n", string->allocated);

    free(string);

    return 0;
}
