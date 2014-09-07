%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "parser_test.h"
	int line=1;
%}
digit [0-9]
sign [-+]
space " "
newline "\n"
tab "\t"
gap {space}|{tab}
integer ({sign}({digit}+))|({digit}+)
float ({sign}({digit}*)"."({digit}+))|(({digit}*)"."({digit}+))
id [a-zA-Z0-9]+
%%
{integer} {
	strcpy(yylval.sval,yytext);
	return INT_CONST;
}
{float} {
	strcpy(yylval.sval,yytext);
	return FLOAT_CONST;
}
{gap} {
	//printf("wewe");
}
int {
	return INT;
}
float {
	return FLOAT;
}
. {
	printf("\nERROR : Unknown Symbol {%s} at line %d\n",yytext,line);
}
%%
