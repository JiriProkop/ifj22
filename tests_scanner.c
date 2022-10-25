#include <stdlib.h>
#include <stdio.h>
#include "scanner.h"

int main() {
    printf("--- [SCANNER TESTS] ---\n");

    token_t tok = get_token();
    printf("Tok type1 je %d'\n'", tok.type);
    tok = get_token();
    printf("Tok type2 je %d'\n'", tok.type);

    return 0;
}
