%{
#include <stdio.h>
#include <string.h>
void yyerror(const char *str)
{
        //fprintf(stderr,"error: %s\n",str);
}
 int yywrap()
{
        return 1;
}
%}
%union
{
	char sval[100];
	int dummy;
}
%token <sval> INT_CONST
%token <sval> FLOAT_CONST
%token INT FLOAT
%type <dummy> A B M1 M2
%type <sval> C D
%start prog

%%
prog:
	
	C M1 A prog
	|
	{
		printf("\nNEXT = %d",$<dummy>-1);
	}
	D M2 B prog
	|
	;
A:
	INT_CONST
	{
		printf("in A--> INT_CONST : inherited = %d,[type] = {%s}",$<dummy>0,$<sval>-1);
	}
	;
B:	
	FLOAT_CONST
	{	
		printf("in A--> FLOAT_CONST : inherited = %d [type] = {%s}",$<dummy>0,$<sval>-1);
	}
	;
M1:
{
	printf("\nUSING MARKER !!\n");
	printf("type = {%s}",$<sval>0);
	$$=5;
}
;
M2:
{
	printf("\nUSING MARKER !!\n");
	printf("type = {%s}",$<sval>0);
	$$=6;
}
C:
INT
{
	strcpy($$,"int");
}
;
D:
FLOAT
{
	strcpy($$,"float");
}
;

%%
int main(void) {
	//extern int yydebug;
	//yydebug = 1;
	yyparse();
	//printf("\n");
	return 0;
}

