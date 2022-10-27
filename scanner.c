#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "scanner.h"

FILE *input;

bool get_token(token_t *tok) {
    input = stdin;
    int c;
    static unsigned state = begin_s;
    static unsigned line_c = 1;
    tok->type = token_none;
    tok->line = 0; // jen pro kontrolu u testu

    while (1) {
        c = getc(input);
        if (c == EOF)
            return true;

        switch (state) {
            case begin_s: {
                char prolog1[] = "<?php";
                char prolog2[] = "declare(strict_types=1);";
                bool white_space = false;
                for (unsigned i = 0; i < (unsigned)strlen(prolog1); i++) {
                    if (c != prolog1[i]) {
                        error_handle(line_c, syntax_error);
                        return false;
                    }
                    c = getc(input);
                }
                while (isspace(c) || c == '/') {
                    if (c == '\n')
                        line_c++;
                    white_space = true;

                    if (c == '/') // line comment counts as 1 whitespace
                    {
                        if ((c = getc(input)) == '/') {
                            while (1) {
                                c = getc(input);
                                if (c == '\n') {
                                    line_c++;
                                    break;
                                } else if (c == EOF) {
                                    error_handle(line_c, syntax_error);
                                    return false;
                                }
                            }
                            c = getc(input);
                            break;
                        } else if (c == '*') {
                            while (1) {
                                c = getc(input);
                                if (c == EOF) {
                                    error_handle(line_c, syntax_error);
                                    return false;
                                } else if (c == '*') {
                                    c = getc(input);
                                    if (c == '/') {
                                        break;
                                    }
                                }
								if (c == '\n') {
									line_c++;
								}
                            }
                        } else {
                            error_handle(line_c, syntax_error);
                            return false;
                        }
                    }
                    c = getc(input);
                }
                if (!white_space) {
                    error_handle(line_c, syntax_error);
                    return false;
                }
                for (unsigned i = 0; i < (unsigned)strlen(prolog2); i++) {
                    if (c != prolog2[i]) {
                        error_handle(line_c, syntax_error);
                        return false;
                    }
                    c = getc(input);
                }
                ungetc(c, input);
                state = start_s;
                break;
            }
            case start_s:
                if (c == '\n') {
                    line_c++;
                    printf("line count: %u'\n", line_c);
                } else if (isspace(c)) {
                    break;
                } else if (c == '$') {
                    state = identifier_s;
                    tok->type = token_identifier;
                    tok->line = line_c;
                    // get_string(); // TODO
                }
                //  else if (isdigit(c)) {
                //     state = integer_s;
                //     continue;
                // }
                else if (c == '/') {
                    state = division_s;
                } else if (c == EOF) {
                    return true;
                } else if (c == '"') {
                    state = string_start_s;
                }
                break;
            case division_s: {
                if (c == '/') {
                    state = line_cmnt_s;
                } else if (c == '*') {
                    state = block_cmnt_start_s;
                } else {
                    state = start_s;
                    ungetc(c, input); // c can be another token ex. 1/5
                    tok->type = token_division;
                    tok->line = line_c;
                    return true;
                }
                break;
            }
            case line_cmnt_s: {
                while (1) {
                    if (c == '\n') {
                        line_c++;
                        state = start_s;
                        break;
                    } else if (c == EOF) {
                        return true;
                    }
                    c = getc(input);
                }
                break;
            }
            case block_cmnt_start_s: {
                while (1) {
                    if (c == '\n') {
                        line_c++;
                    } else if (c == EOF) {
                        return true;
                    } else if (c == '*') {
                        state = block_cmnt_inside_s;
                        break;
                    }
                    c = getc(input);
                }
                break;
            }
            case block_cmnt_inside_s: {
                if (c == '/') {
                    state = start_s;
                } else {
                    state = block_cmnt_start_s;
                }
                break;
            }
                // '/' done
        } // end of switch
    }
    return true;
}

// int main() {

//     token_t tok = get_token();
//     printf("Tok type1 je %d'\n'", tok.type);
//     tok = get_token();
//     printf("Tok type2 je %d'\n'", tok.type);
//     return ret;
// }