/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Dynamic string tests.
 *
 * @author Patrik Čerbák xcerba00 <xcerba00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include "../src/dynstr.h"
#include "../src/error.h"

int main() {
	printf("--- [DYNSTR TESTS] ---\n");

	// allocate the dynstr_t struct before using
	dynstr_t *string = malloc(sizeof(dynstr_t));
	if(string == NULL) {
		error_handle(0, compiler_error);
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

	// test add string
	printf("\n[dynstr add string test]\n");
	printf("Insert short string:\n");
	dynstr_add_string(string, "some_string");
	printf("String: '%s'\n", string->array);
	printf("Length: %lu\n", string->length);
	printf("Allocated: %lu\n", string->allocated);
	printf("Insert another string:\n");
	dynstr_add_string(string, "_some_another_longer_string");
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


    // test dynstrcmp
    printf("\n--- test dynstrcmp ---\n");
    dynstr_t *string1 = malloc(sizeof(dynstr_t));
    if(string == NULL) {
        error_handle(0, compiler_error);
    }
    int result = 1;
    printf("comparing the same string\n");
    dynstr_add_string(string, "some_string");
    dynstr_add_string(string1, "some_string");
    result = dynstrcmp(string,string1);
    printf("result: %d \n", result);

    printf("comparing first longer");
    dynstr_clear(string);
    dynstr_add_string(string, "some_string_here");
    result = dynstrcmp(string,string1);
    printf("result: %d \n", result);
    printf("comparing second longer");
    result = dynstrcmp(string1,string);
    printf("result: %d \n", result);

    dynstr_delete(string);
    string_free(string1);
    // free the dynstr_t structure
    free(string);
    return ret;
}
