/////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "dfanfa.c"
#include "fileio.c"
#include "generic.c"

int main(int argc,char **argv)
{
	system("reset");
	if(argv[1]==NULL){
		printf("\nEXIT no input file..\n");
		return 0;
	}
	const char *SYMBOLTABLE = "symbol_table_3.out";
	const char *OUTFILE="a4_3.out";
	// const char *SOURCE="input.c";
	initialize_All(SYMBOLTABLE,OUTFILE);
	printf("\nHERE\n");
	compileWithDFA(argv[1],SYMBOLTABLE,OUTFILE);
	return 0;
}


