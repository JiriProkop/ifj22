/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Dynamic string implementation.
 *
 * @author Patrik Čerbák xcerba00 <xcerba00@stud.fit.vutbr.cz>
 */

#ifndef _DYN_STR_H
#define _DYN_STR_H

#define STR_DEFAULT_LENGTH 16 // the default length of string to allocate

/**
 * @struct Dynamic string structure
 */
typedef struct {
    char *array;
    unsigned long length;    // length of the string
    unsigned long allocated; // length of the allocated space
} dynstr_t;

/**
 * Initialize the dynamic string.
 *
 * @param string Pointer to dynamic string with the dynstr_t type.
 */
void dynstr_init(dynstr_t *string);

/**
 * Clears the dynamic string.
 *
 * @param string Pointer to dynamic string with the dynstr_t type.
 */
void dynstr_clear(dynstr_t *string);

/**
 * Dynamic string destructor.
 *
 * @warning You still have to free the dynstr_t structure!
 * @param string Pointer to dynamic string with the dynstr_t type.
 */
void dynstr_delete(dynstr_t *string);

/**
 * Add a character to string.
 *
 * @brief Adds a character to the string, if the string is full, then allocate more space.
 *
 * @param string Pointer to dynamic string with the dynstr_t type.
 * @param character A character of the type char.
 */
void dynstr_add_char(dynstr_t *string, char character);

/**
 * Add a whole string to the dynstr.
 *
 * @brief Adds a whole string to the dynstr. It calls the function dynstr_add_char.
 *
 * @param string Pointer to dynamic string with the dynstr_t type.
 * @param chars A string to add.
 */
void dynstr_add_string(dynstr_t *string, char *chars);

/**
 * Compares dynamic string content with given string.
 *
 * @param string Pointer to dynamic string with the dynstr_t type.
 * @param compare_str String to compare the dynamic with.
 * @return Returns 1 if the strings are the same.
 */
int dynstr_compare(dynstr_t *string, char *compare_str);

/**
 * allocates and initialises struct dynstr_t
 *
 * @return Returns NULL if error occured, pointer to dynstr_t otherwise
 */
dynstr_t *string_innit();

/**
 * frees dynstr_t struct
 *
 * @param attr pointer to allocated dynstr_t
 */
void string_free(dynstr_t *attr);

/**
 * @brief strcmp fot dynstr 
 * 
 * @param strplus 
 * @param strminus 
 * @return int 0 if strinfs are the same 
 *        return > 0 if first different char is bigger in *strplus
 *        return < 0 if first different char is bigger in *strminus
 */
int dynstrcmp(dynstr_t *strplus, dynstr_t *strminus);

#endif