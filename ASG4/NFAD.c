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
	const char *SYMBOLTABLE = "Symboltable3.out";
	const char *OUTFILE="outfile3.out";
	// const char *SOURCE="input.c";
	initialize_All(SYMBOLTABLE,OUTFILE);
	printf("\nHERE\n");
	compileWithDFA(argv[1],SYMBOLTABLE,OUTFILE);
	return 0;
}
