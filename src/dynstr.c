/**
 * Project: compiler of IFJ22 programming language
 *
 * @file dynstr.c
 * @brief Dynamic string implementation.
 *
 * @author Patrik Čerbák xcerba00 <xcerba00@stud.fit.vutbr.cz>
 */

#include <stdlib.h>
#include "dynstr.h"
#include "parser.h"
#include "error.h"

void dynstr_init(dynstr_t *string) {
    string->array = malloc(sizeof(char) * STR_DEFAULT_LENGTH);
    if (string->array == NULL) {
        error_handle(0, compiler_error);
        abort();
    }
    string->array[0] = '\0';
    string->length = 0;
    string->allocated = STR_DEFAULT_LENGTH - 1; // minus one, since the last char is for \0
}

void dynstr_clear(dynstr_t *string) {
    string->array[0] = '\0';
    string->length = 0;
}

void dynstr_delete(dynstr_t *string) {
    if(string == NULL || string->array == NULL){
        return;
    }
    free(string->array);
    string->length = 0;
    string->allocated = 0;
}

void dynstr_add_char(dynstr_t *string, char character) {
    if (string->length < string->allocated) {
        string->array[string->length] = character;
        string->array[string->length + 1] = '\0';
    } else {
        // add more space (the +1 is fot the \0 symbol)
        string->array = realloc(string->array, sizeof(char) * (string->allocated + STR_DEFAULT_LENGTH + 1));
        if (string->array == NULL) {
            error_handle(0, compiler_error);
        	abort();
        }
        string->allocated = string->allocated + STR_DEFAULT_LENGTH;

        string->array[string->length] = character;
        string->array[string->length + 1] = '\0';
    }

    string->length++;
}

void dynstr_add_string(dynstr_t *string, char *chars) {
    unsigned long i = 0;
    while (chars[i] != '\0') {
        dynstr_add_char(string, chars[i]);
        i++;
    }
}

int dynstr_compare(dynstr_t *string, char *compare_str) {
    for (unsigned long i = 0; i <= string->length; i++) {
        if (string->array[i] != compare_str[i]) {
            return 0;
        }
    }
    return 1;
}

dynstr_t *string_innit() {
    dynstr_t *str = malloc(sizeof(dynstr_t));
    dynstr_init(str);
    if (str == NULL) {
        free(str);
        return NULL;
    }
    return str;
}

void string_free(dynstr_t *attr) {
    dynstr_delete(attr);
    free(attr);
}

int dynstrcmp(dynstr_t *strplus, dynstr_t *strminus){
    unsigned i = 0;
    while(i <= strplus->length){
        if (strplus->array[i] != strminus->array[i]){
            return (unsigned)strplus->array[i] - (unsigned)strminus->array[i];
        }
        i++;
    }
    return 0;
}