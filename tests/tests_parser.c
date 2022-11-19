/**
 * Project: compiler of IFJ22 programming language
 *
 * @brief Basic parser tests.
 *
 * @author Patrik Čerbák xcerba00 <xcerba00@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/scanner.h"
#include "../src/parser.h"
#include "../src/error.h"

int ret = 0;
int main() {
    //printf("--- [SCANNER TESTS] ---\n");
    if(start()) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    free_tkn();
    return ret;
}
