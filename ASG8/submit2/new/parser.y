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
	char	*sval;
};

%token AND
%token ASSIGN
%token COLON
%token COMMA
%token DEF
%token ELSE
%token DIV
%token END
%token EQ
%token FLOAT
%token GE
%token GLOBAL
%token GT
%token IF
%token INT
%token LEFT_PAREN
%token LEFT_SQ_BKT
%token LE
%token LT
%token MINUS
%token MOD
%token MULT
%token NE
%token NOT
%token NUL
%token OR
%token PLUS
%token PRINT
%token PRODUCT
%token READ
%token RETURN
%token RIGHT_PAREN
%token RIGHT_SQ_BKT
%token SEMICOLON
%token WHILE
%token ID
%token INT_CONST
%token FLOAT_CONST
%token DOT
%token FORMAT

%start prog

%left PLUS MINUS 
%left MULT DIV MOD 
%left DOT
%left AND OR 
%left NOT

%%
prog:
	GLOBAL declList stmtListO END
	;
declList:
	/*empty*/
	|decl declList
	;
decl:
	DEF typeList END
	;
typeList :	typeList SEMICOLON varList COLON type
| typeList SEMICOLON typeDef
| varList COLON type
| typeDef
;
varList:
	var COMMA varList
	|var
	;
var:
	ID sizeListO
	;
sizeListO:
	/*empty*/
	|sizeList
	;
sizeList:
	sizeList LEFT_SQ_BKT INT_CONST RIGHT_SQ_BKT
	|LEFT_SQ_BKT INT_CONST RIGHT_SQ_BKT
	;
type:
	INT
	|FLOAT
	|NUL
	|ID
	;
typeDef:
	ID ASSIGN PRODUCT typeList END
	;
stmtListO:
	/*empty*/
	|stmtList
	;
stmtList :	stmtList SEMICOLON stmt
| stmt
;
stmt :	assignmentStmt
| readStmt
| printStmt
| ifStmt
| whileStmt
| returnStmt
;
assignmentStmt:
	dotId ASSIGN exp
	;
dotId:
	id
	|id DOT dotId
	;
readStmt:
	READ FORMAT exp
	;
printStmt:
	PRINT FORMAT exp
	;
ifStmt:
	IF bExp COLON stmtList elsePart END
	;
elsePart:
	/*empty*/
	|ELSE stmtList
	;
whileStmt:
	WHILE bExp COLON stmtList END
	;
returnStmt:
	RETURN expO
	;
expO:
	/*empty*/
	|exp
	;
id:
	ID indexListO
	;
indexListO:
	/*empty*/
	|indexList
	;
indexList:
	indexList LEFT_SQ_BKT exp RIGHT_SQ_BKT
	|LEFT_SQ_BKT exp RIGHT_SQ_BKT
	;
bExp:
	bExp OR bExp
	|bExp AND bExp
	|NOT bExp
	|LEFT_PAREN bExp RIGHT_PAREN
	|exp relOP exp
	;
relOP:
	EQ
	|LE
	|LT
	|GE
	|GT
	|NE
	;
exp:
	exp PLUS exp
	|exp MINUS exp
	|exp MULT exp
	|exp DIV exp
	|exp MOD exp
	|exp DOT exp
	|LEFT_PAREN exp RIGHT_PAREN
	|id
	|LEFT_PAREN ID COLON actParamListO RIGHT_PAREN
	|INT_CONST
	|FLOAT_CONST
	;
actParamListO:
	/*empty*/
	|actParamList
	;
actParamList:
	actParamList COMMA exp
	|exp
	;
%%
int main(void) {
	extern int yydebug;
	yydebug = 1;
	yyparse();
	printf("\n");
	return 0;
}

