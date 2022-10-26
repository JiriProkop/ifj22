#include <stdlib.h>

#include "dynstr.h"

int dynstr_init(dynstr_t *string) {
    string->array = malloc(sizeof(char) * STR_DEFAULT_LENGTH);
    if(string->array == NULL) {
        return 0;
    }
    string->array[0] = '\0';
    string->length = 0;
    string->allocated = STR_DEFAULT_LENGTH - 1; // minus one, since the last char is for \0
    return 1;
}

void dynstr_clear(dynstr_t *string) {
    string->array[0] = '\0';
    string->length = 0;
}

void dynstr_delete(dynstr_t *string) {
    free(string->array);
    string->length = 0;
    string->allocated = 0;
}

int dynstr_add_char(dynstr_t *string, char character) {
    if(string->length < string->allocated) {
        string->array[string->length] = character;
        string->array[string->length + 1] = '\0';
    } else {
        // add more space
        string->array = realloc(string->array, sizeof(char) * (string->length + STR_DEFAULT_LENGTH));
        if(string->array == NULL) {
            return 0;
        }
        string->allocated = string->allocated + STR_DEFAULT_LENGTH;

        string->array[string->length] = character;
        string->array[string->length + 1] = '\0';
    }

    string->length++;
    return 1;
}

int dynstr_compare(dynstr_t *string, char *compare_str) {
    for(unsigned long i = 0; i <= string->length; i++) {
        if(string->array[i] != compare_str[i]) {
            return 0;
        }
    }
    return 1;
}