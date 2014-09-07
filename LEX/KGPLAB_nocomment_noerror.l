%{
	int a=0;
	int state=0;
%}
INT ([0-9])+
%%
ADD {printf("<ADD>");
state=1;}
MMUL {printf("<MMUL>");
state=2;}
CMUL {printf("<CMUL>");
state=3;}
DEF {printf("<DEF>");
state=4;}
INIT {printf("<INIT>");
state=5;}
PRINT {printf("<PRINT>");}

[0-9]+ {printf("<INT>");}
[a-zA-Z0-9]+ {printf("<ID>");}

(",") {printf("<COMMA>");}
(";") {printf("<ENDS>");state=-1;}
(":") {printf("<ENDR>");}
("[") {printf("<SQBKTO>");}
("]") {printf("<SQBKTC>");}
(" "|\t)* ;
(\n) {if(state!=0){
		printf("\n");
		state=0;
		}
}
%%
int main(){
	//(("+"|"-")[0-9]+)|
	yylex();
	printf("\nHERE ");

	return 0;
}
