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


%token ID MULT PLUS PLEFT PRIGHT
%start E
%%
E:
	E PLUS T
	{
		//printf("\nE --> E + T");
	}
	| T
	{
		//printf("\nE --> T");
	}
	;
T:
	T MULT F
	{
		//printf("\nT --> T * F");
	}
	| F
	{
		//printf("\nT --> F");
	}
	;
F:
	PLEFT E PRIGHT
	{
		//printf("\nF --> ( E )");
	}
	| ID
	{
		//printf("\nF --> ID");
	}
	;
%%


int main(void) {
	extern int yydebug;
	yydebug = 1;
	yyparse();
	printf("\n");
	return 0;
}