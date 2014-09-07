#include <stdlib.h>
#include <stdio.h>
#include "parser.c"


int main(){
	initialize_All();
	checkParserData();
	createParser();
	showParser();
	parse();
	freeMemory();
	return 0;
}