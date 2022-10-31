#include <stdlib.h>
#include <stdio.h>
#include "../scanner.h"
#include "../error.h"

int ret = 0;
int main() {
    printf("--- [SCANNER TESTS] ---\n");

    token_t* tok = malloc(sizeof(token_t));
	get_token(tok);
    printf("Tok type1 je %d'\n'", tok->type);
    get_token(tok);
    printf("Tok type2 je %d'\n'", tok->type);
    free(tok);

    return ret;
}
