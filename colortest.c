#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void rprintf(const char *msg){
	printf("\033[31m%s\033[0m",msg);
	return;
}

int main(){

	    rprintf("\nTEXT \n");
	    printf("\nDEFAUT\n");
	    return 0;
}