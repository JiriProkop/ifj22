#include <ctype.h>
#include <stdio.h>

#include "error.h"
#include "scanner.h"

int ret = 0; // put this to main
FILE *input;

token_t get_token() {
    input = stdin;
    int c;
    static unsigned state = begin_s;
    static unsigned line_c = 1;
    token_t tok = {.type = token_none};

    while (1) {
        c = getc(input);
        switch (state) {
            case begin_s:
                if (c != '<') {
                    error_handle(line_c, syntax_error);
                    state = start_s;
                    break;
                }
                c = getc(input);
                if (c != '?') {
                    error_handle(line_c, syntax_error);
                    state = start_s;
                    break;
                }
                c = getc(input);
                if (c != 'p') {
                    error_handle(line_c, syntax_error);
                    state = start_s;
                    break;
                }
                c = getc(input);
                if (c != 'h') {
                    error_handle(line_c, syntax_error);
                    state = start_s;
                    break;
                }
                c = getc(input);
                if (c != 'p') {
                    error_handle(line_c, syntax_error);
                    state = start_s;
                    break;
                }
                state = start_s;
                break;
            case start_s:
                if (c == '\n') {
                    line_c++;
                } else if (isspace(c)) {
                    break;
                } else if (c == '/') {
                    state = division_s;
                } else if (c == EOF) {
                    return tok;
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
                    tok.type = token_division;
                    return tok;
                    // TODO vlozit do BS a do tokenu ulozit id
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
                        return tok;
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
                        return tok;
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
    return tok;
}

// int main() {

//     token_t tok = get_token();
//     printf("Tok type1 je %d'\n'", tok.type);
//     tok = get_token();
//     printf("Tok type2 je %d'\n'", tok.type);
//     return ret;
// }