#include <stdio.h>
#include <stdlib.h>

#include "../src/scanner.h"
#include "../src/ll.h"



int ret = 0;

int main(){
    printf("--- [LINKED LIST TEST] ---\n");
    list *list;

    printf("\n[list_init test]\n");
    list_init(list);
    if(list = NULL){
        printf("initialization succesful \n");
    }
    return ret;
}