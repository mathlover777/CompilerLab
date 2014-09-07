%{
#include <stdio.h>
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 int yywrap()
{
        return 1;
}
%}
%union
{
	char sval[100];
};
%token prog

%start prog

%%
prog:
	INT_CONST prog
	{
		printf("\nINT = {%s}",id.sval);
	}
	|
	FLOAT_CONST prog
	{
		printf("\nFLOAT = {%s}",id.sval);
	}
	|
	;

%%
int main(void) {
	extern int yydebug;
	yydebug = 1;
	yyparse();
	printf("\n");
	return 0;
}
