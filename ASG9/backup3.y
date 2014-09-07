%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.c"
#include "symtab.c"
#include "code.c"
#include "string.c"
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 int yywrap()
{
        return 1;
}
int offset=0;
%}
%code requires{
	#include "structures.c"
}
%union
{
	char type[20];
	char id[20];
	Set S;
	int dummy;
	EXP_attr EXP;
	char number[20];
	STMT_attr STMT;
	BEXP_attr B;
	char OP[20];
}
	%token AND
	%token ASSIGN
	%token COLON
	%token COMMA
	%token DEF
	%token ELSE
	%token DIV
	%token END
	%token EQ
	%token <type> FLOAT
	%token GE
	%token GLOBAL
	%token GT
	%token IF
	%token <type> INT
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
	%token <id> ID
	%token <number> INT_CONST
	%token <number> FLOAT_CONST
	%token DOT
	%token <type>FORMAT
	%token <type> BOOLEAN
	%token <type> NULL_

%start prog
	%type <type> type
	%type <S> var varList
	%type <dummy> typeList
	%type <EXP> exp expO assignmentStmt readStmt printStmt
	%type <STMT> stmtList stmt stmtListO whileStmt
	%type <STMT> M4 M1 M2 M10
	%type <B> M9 bExp
	%type <OP> relOP
%left PLUS MINUS 
%left MULT DIV MOD 
%left DOT
%left AND OR 
%left NOT

%%
prog:
	GLOBAL declList stmtListO END
	{
		saveSymTab();
	}
	;
declList:
	/*empty*/
	|decl declList
	;
decl:
	DEF typeList END
	;
typeList:
	typeList varList COLON type SEMICOLON
	{
		setElt *temp=$2.head;
		while(temp!=NULL){
			insertSymTab(temp->varname,$4,&offset);
			temp=temp->next;
		}
		deleteSet(&($2));
	}
	|varList COLON type SEMICOLON
	{
		setElt *temp=$1.head;
		while(temp!=NULL){
			insertSymTab(temp->varname,$3,&offset);
			temp=temp->next;
		}
		deleteSet(&($1));
	}
	;
varList:
	var COMMA varList
	{
		insertSet(&($3),$1.head->varname);
		deleteSet(&($1));
		moveSet(&($$),&($3));
		//printf("\nvarList -> var COMMA varList");
		//printSet(&($$));
	}
	|var
	{
		initSet(&($$));
		insertSet(&($$),$1.head->varname);
		deleteSet(&($1));
		//printf("\nVARLIST --> var {%s}",$$.head->varname);
	}
	;
var:
	ID
	{
		//printf("\nID = {%s}",$1);
		initSet(&($$));
		insertSet(&($$),$1);
		//printf("\nSET = {%s}",$$.head->varname);
	}
	;
type:
	INT
	{
		//test_symtab();
		strcpy($$,$1);
		//printf("\nTYPE = %s",$$);
	}
	|FLOAT
	{
		strcpy($$,$1);
		//test_symtab();
		//printf("\nTYPE = %s",$$);
	}
	|BOOLEAN
	{
		strcpy($$,$1);
		//printf("\nTYPE = %s",$$);
	}
	|NULL_
	{
		strcpy($$,$1);
		//printf("\nTYPE = %s",$$);
	}
	;
stmtListO:
	/*empty*/
	{
		//initCode(&($$.C));
	}
	|stmtList{
		initCode(&($$.C));
		moveCode(&($$.C),&($1.C));
		printCode(&($$.C));
		dumpCode(&($$.C));
	}
	;
stmtList:
	stmtList M1 stmt
	{
		//printf("\nLABEL at stmtList1 = {%s}",$<STMT>2.next);
		initCode(&($$.C));
		//printf("\nGOING TO CONCAT!!");
		concatCode(&($$.C),&($1).C,&($3).C);
		char Line[100];
		sprintf(Line,"%s:",$<STMT>2.next);
		//printf("\nHERE GONE LINE = {%s}??\n",Line);
		addCode(&($$.C),Line);
		//printf("\nNO\n");
	}
	|M2 stmt
	{
		//printf("\nLABEL at stmtList = {%s}",$<STMT>1.next);
		initCode(&($$.C));
		moveCode(&($$.C),&($2.C));
		char Line[100];
		sprintf(Line,"%s:",$<STMT>1.next);
		//printf("\nHERE GONE 1 {%s}??\n",Line);
		addCode(&($$.C),Line);
		//printf("\nNO 1\n");	
	}
	;
stmt :	
	assignmentStmt SEMICOLON
	{
		initCode(&($$.C));
		moveCode(&($$.C),&($1.C));
		strcpy($$.type,$1.type);
	}
	|readStmt SEMICOLON
	{
		initCode(&($$.C));
		moveCode(&($$.C),&($1.C));
		strcpy($$.type,$1.type);
	}
	|printStmt SEMICOLON
	{	
		initCode(&($$.C));
		moveCode(&($$.C),&($1.C));
		strcpy($$.type,$1.type);
	}
	|M3 ifStmt
	|M4 whileStmt{
		//printf("\nINIT WHILE!!");
		initCode(&($$.C));
		//printf("\nWHILE NEXT = {%s},",$<STMT>1.next);
		//printf("\nWHILE HEAD =%p TAIL =%p",$$.C.head,$$.C.tail);
		moveCode(&($$.C),&($2.C));
	}
	;
assignmentStmt:
	ID ASSIGN expO
	{
		char idType[20];
		int typeR=getType($1,idType);
		if(typeR!=SUCCESS){
			printf("\nERROR ACCESSING UNDEFINED VARIABLE!!\n");
			exit(-1);
		}
		char dummyType[20];
		int compR=compareType(idType,$3.type,dummyType);
		//printf("\ncompR = %d",compR);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char Line[200];
		if(compR!=1){
			if(compR==10){
				printf("\nERROR CANT STORE FLOAT IN INT");
				exit(-1);
			}
			if(compR==20){
				sprintf(Line,"%s = (float) %s",$1,$3.ADDR);
			}
		}else{
			sprintf(Line,"%s = %s",$1,$3.ADDR);
		}
		initCode(&($$.C));
		moveCode(&($$.C),&($3.C));
		addCode(&($$.C),Line);
		//printf("\nASSIGNMENTSTMT ---> ");
		//printCode(&($$.C));
	}
	;
readStmt:
	READ FORMAT expO
	{
		initCode(&($$.C));
		char dummyType[20];
		int compR=compareType($2,$3.type,dummyType);
		if(compR!=1){
			printf("\nERROR FORMAT MISSMATCH IN READ FORMAT = {%s} EXPTYPE = {%s}!!",$2,$3.type);
			exit(-1);
		}
		strcpy($$.type,dummyType);
		char Line[200];
		moveCode(&($$.C),&($3).C);
		sprintf(Line,"param %s",$3.ADDR);
		addCode(&($$.C),Line);
		if(isEqual(dummyType,"int")){
			sprintf(Line,"call readInt",$3.ADDR);
		}else{
			sprintf(Line,"call readFloat",$3.ADDR);
		}
		addCode(&($$.C),Line);
		//printf("\nREADSTMT ---> ");
		//printCode(&($$.C));
	}
	;
printStmt:
	PRINT FORMAT expO
	{
		initCode(&($$.C));
		char dummyType[20];
		int compR=compareType($2,$3.type,dummyType);
		if(compR!=1){
			printf("\nERROR FORMAT MISSMATCH IN READ FORMAT = {%s} EXPTYPE = {%s}!!",$2,$3.type);
			exit(-1);
		}
		strcpy($$.type,dummyType);
		char Line[200];
		moveCode(&($$.C),&($3).C);
		sprintf(Line,"param %s",$3.ADDR);
		addCode(&($$.C),Line);
		if(isEqual(dummyType,"int")){
			sprintf(Line,"call printInt",$3.ADDR);
		}else{
			sprintf(Line,"call printFloat",$3.ADDR);
		}
		addCode(&($$.C),Line);
		//printf("\nREADSTMT ---> ");
		//printCode(&($$.C));
	}
	;
ifStmt:
	IF M5 bExp COLON M6 stmtList M7 elsePart END
	;
elsePart:
	/*empty*/
	|ELSE M8 stmtList
	;
whileStmt:
	WHILE M9 bExp COLON M10 stmtList END
	{
		//printf("\nWHILE NEXT = {%s}",$<STMT>0.next);
		//printf("\nstmtNext = {%s}",$5.next);
		initCode(&($$.C));
		//whileStmt.Code=Label(stmtList.Next)||bExp.Code||
		//Label(bExp.True)||stmtList.Code||Gen(goto stmtList.Next)
		char Line[200];
		char begin[50];
		getNewLabel(begin);
		sprintf(Line,"%s:",begin);
		addCode(&($$.C),Line);
		appendCodeToLeft(&($$.C),&($3.C));
		char Label[50];
		sprintf(Label,"%s",$3.True);
		sprintf(Line,"%s:",Label);
		addCode(&($$.C),Line);
		appendCodeToLeft(&($$.C),&($6.C));
		sprintf(Line,"goto %s",begin);
		addCode(&($$.C),Line);
	}
	;
expO:
	/*empty*/
	{
		initCode(&($$.C));
		strcpy($$.ADDR,"null");
		strcpy($$.type,"null");
	}
	|exp
	{
		initCode(&($$.C));
		strcpy($$.ADDR,$1.ADDR);
		strcpy($$.type,$1.type);
		moveCode(&($$.C),&($1.C));
	}
	;
bExp:
	M11 bExp OR M12 bExp
	|M13 bExp AND M14 bExp
	|NOT M15 bExp
	|LEFT_PAREN M16 bExp RIGHT_PAREN
	|exp relOP exp
	{
		//printf("\nRELOP = {%s}",$2);
		printf("\nBEXP TRUE = {%s}",$<B>0.True);
		printf("\nBEXP FALSE = {%s}",$<B>0.False);
		initCode(&($$.C));
		concatCode(&($$.C),&($1.C),&($3.C));
		char Line[200];
		sprintf(Line,"if %s %s %s goto %s",$1.ADDR,$2,$3.ADDR,$<B>0.True);
		addCode(&($$.C),Line);
		sprintf(Line,"goto %s",$<B>0.False);
		addCode(&($$.C),Line);
		strcpy($$.True,$<B>0.True);
		strcpy($$.False,$<B>0.False);
	}
	;
	M9:{

		//printf("\nBFALSE = {%s}",$<STMT>-1.next);
		getNewLabel($$.True);
		strcpy($$.False,$<STMT>-1.next);
		printf("\nB.TRUE = {%s}",$$.True);
		printf("\nB.FALSE = {%s}",$$.False);
	}
	;
	M10:{
		getNewLabel($$.next);
		printf("\nSTMT NEXT = {%s}",$$.next);
	}
	;
M1:
	{
		getNewLabel($$.next);
		//printf("\nLABEL FROM M1 : {%s}",$$.next);
	}
	;
	M2:
	{
		getNewLabel($$.next);
		//printf("\nLABEL FROM M2 : {%s}",$$.next);
	}
	;
	M3:
	;
	M4:{
		printf("\nNEXT LABEL = {%s}",$<STMT>0.next);
		strcpy($$.next,$<STMT>0.next);
	}
	;
	M5:
	;
	M6:
	;
	M7:
	;
	M8:
	;

	
	M11:
	;
	M12:
	;
	M13:
	;
	M14:
	;
	M15:
	;
	M16:
	;
relOP:
	EQ
	{
		strcpy($$,"EQ");
	}
	|LE
	{
		strcpy($$,"LE");
	}
	|LT
	{
		strcpy($$,"LT");
	}
	|GE
	{
		strcpy($$,"GE");
	}
	|GT
	{
		strcpy($$,"GT");
	}
	|NE
	{
		strcpy($$,"NE");
	}
	;
exp:
	exp PLUS exp
	{
		//exp.addr=newTemp()
		//exp.code=exp1.code||exp2.code||Gen(exp1.addr OP exp2.addr)
		getNewTemp($$.ADDR);
		initCode(&($$.C));
		char type[20];
		int compR=compareType($1.type,$3.type,type);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char typeLine[200];
		char typeVar[20];
		typeLine[0]='\0';
		char line[200];
		if(compR!=1){
			// we need to do explicit type casting
			getNewTemp(typeVar);
			insertSymTab(typeVar,"float",&offset);
			insertSymTab($$.ADDR,"float",&offset);
			strcpy($$.type,"float");
			if(compR==10){
				sprintf(typeLine,"%s = (float) %s",typeVar,$1.ADDR);
				sprintf(line,"%s = %s + %s",$$.ADDR,typeVar,$3.ADDR);
			}
			if(compR==20){
				sprintf(typeLine,"%s = (float) %s",typeVar,$3.ADDR);
				sprintf(line,"%s = %s + %s",$$.ADDR,$1.ADDR,typeVar);
			}
		}else{
			insertSymTab($$.ADDR,$1.type,&offset);
			strcpy($$.type,$1.type);
			sprintf(line,"%s = %s + %s",$$.ADDR,$1.ADDR,$3.ADDR);
		}
		concatCode(&($$.C),&($1).C,&($3).C);
		if(compR!=1){
			addCode(&($$.C),typeLine);
		}
		addCode(&($$.C),line);
	}
	|exp MINUS exp
	{
		//exp.addr=newTemp()
		//exp.code=exp1.code||exp2.code||Gen(exp1.addr OP exp2.addr)
		getNewTemp($$.ADDR);
		char type[20];
		int compR=compareType($1.type,$3.type,type);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char typeLine[200];
		char typeVar[20];
		typeLine[0]='\0';
		char line[200];
		if(compR!=1){
			// we need to do explicit type casting
			getNewTemp(typeVar);
			insertSymTab(typeVar,"float",&offset);
			insertSymTab($$.ADDR,"float",&offset);
			strcpy($$.type,"float");
			if(compR==10){
				sprintf(typeLine,"%s = (float) %s",typeVar,$1.ADDR);
				sprintf(line,"%s = %s - %s",$$.ADDR,typeVar,$3.ADDR);
			}
			if(compR==20){
				sprintf(typeLine,"%s = (float) %s",typeVar,$3.ADDR);
				sprintf(line,"%s = %s - %s",$$.ADDR,$1.ADDR,typeVar);
			}
		}else{
			insertSymTab($$.ADDR,$1.type,&offset);
			strcpy($$.type,$1.type);
			sprintf(line,"%s = %s - %s",$$.ADDR,$1.ADDR,$3.ADDR);
		}
		concatCode(&($$.C),&($1).C,&($3).C);
		if(compR!=1){
			addCode(&($$.C),typeLine);
		}
		addCode(&($$.C),line);
	}
	|exp MULT exp
	{
		getNewTemp($$.ADDR);
		char type[20];
		int compR=compareType($1.type,$3.type,type);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char typeLine[200];
		char typeVar[20];
		typeLine[0]='\0';
		char line[200];
		if(compR!=1){
			// we need to do explicit type casting
			getNewTemp(typeVar);
			insertSymTab(typeVar,"float",&offset);
			insertSymTab($$.ADDR,"float",&offset);
			strcpy($$.type,"float");
			if(compR==10){
				sprintf(typeLine,"%s = (float) %s",typeVar,$1.ADDR);
				sprintf(line,"%s = %s * %s",$$.ADDR,typeVar,$3.ADDR);
			}
			if(compR==20){
				sprintf(typeLine,"%s = (float) %s",typeVar,$3.ADDR);
				sprintf(line,"%s = %s * %s",$$.ADDR,$1.ADDR,typeVar);
			}
		}else{
			insertSymTab($$.ADDR,$1.type,&offset);
			strcpy($$.type,$1.type);
			sprintf(line,"%s = %s * %s",$$.ADDR,$1.ADDR,$3.ADDR);
		}
		concatCode(&($$.C),&($1).C,&($3).C);
		if(compR!=1){
			addCode(&($$.C),typeLine);
		}
		addCode(&($$.C),line);
	}
	|exp DIV exp
	{
		getNewTemp($$.ADDR);
		char type[20];
		int compR=compareType($1.type,$3.type,type);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char typeLine[200];
		char typeVar[20];
		typeLine[0]='\0';
		char line[200];
		if(compR!=1){
			// we need to do explicit type casting
			getNewTemp(typeVar);
			insertSymTab(typeVar,"float",&offset);
			insertSymTab($$.ADDR,"float",&offset);
			strcpy($$.type,"float");
			if(compR==10){
				sprintf(typeLine,"%s = (float) %s",typeVar,$1.ADDR);
				sprintf(line,"%s = %s / %s",$$.ADDR,typeVar,$3.ADDR);
			}
			if(compR==20){
				sprintf(typeLine,"%s = (float) %s",typeVar,$3.ADDR);
				sprintf(line,"%s = %s / %s",$$.ADDR,$1.ADDR,typeVar);
			}
		}else{
			insertSymTab($$.ADDR,$1.type,&offset);
			strcpy($$.type,$1.type);
			sprintf(line,"%s = %s / %s",$$.ADDR,$1.ADDR,$3.ADDR);
		}
		concatCode(&($$.C),&($1).C,&($3).C);
		if(compR!=1){
			addCode(&($$.C),typeLine);
		}
		addCode(&($$.C),line);
	}
	|exp MOD exp
	{
		getNewTemp($$.ADDR);
		insertSymTab($$.ADDR,"int",&offset);
		char type[20];
		char line[200];
		if(isEqual($1.type,"int")==0||isEqual($3.type,"int")==0){
			printf("\nERROR MOD can be done with integer only !!");
			exit(-1);
		}
		concatCode(&($$.C),&($1).C,&($3).C);
		sprintf(line,"%s = %s MOD %s",$$.ADDR,$1.ADDR,$3.ADDR);
		addCode(&($$.C),line);
		strcpy($$.type,"int");
	}
	|exp DOT exp
	{
		getNewTemp($$.ADDR);
		insertSymTab($$.ADDR,"int",&offset);
		char type[20];
		char line[200];
		if(isEqual($1.type,"int")==0||isEqual($3.type,"int")==0){
			printf("\nERROR MOD can be done with integer only !!");
			exit(-1);
		}
		concatCode(&($$.C),&($1).C,&($3).C);
		sprintf(line,"%s = %s DOT %s",$$.ADDR,$1.ADDR,$3.ADDR);
		addCode(&($$.C),line);
		strcpy($$.type,"int");
	}
	|LEFT_PAREN exp RIGHT_PAREN
	{
		initCode(&($$.C));
		moveCode(&($$.C),&($2.C));
		strcpy($$.ADDR,$2.ADDR);
		strcpy($$.type,$2.type);
	}
	|ID
	{
		initCode(&($$.C));
		strcpy($$.ADDR,$1);
		if(getType($1,$$.type)!=SUCCESS){
			printf("\nERROR ACCESSING UNDEFINED VARIABLE!!\n");
			exit(-1);
		}
		//printf("\nID = {%s} TYPE = {%s}",$$.ADDR,$$.type);
	}
	|FLOAT_CONST
	{
		getNewTemp($$.ADDR);
		insertSymTab($$.ADDR,"float",&offset);
		char line[200];
		sprintf(line,"%s = %s",$$.ADDR,$1);
		//printf("\nLINE = {%s}",line);
		initCode(&($$.C));
		addCode(&($$.C),line);
		strcpy($$.type,"float");
		//printf("\nEXP TYPE = %s",$$.type);
		//printf("\nCODE = %s",(($$.C).head)->Line);
	}
	|INT_CONST
	{
		getNewTemp($$.ADDR);
		insertSymTab($$.ADDR,"int",&offset);
		char line[200];
		sprintf(line,"%s = %s",$$.ADDR,$1);
		//printf("\nLINE = {%s}",line);
		//printf("\nHERE 3\n");
		initCode(&($$.C));
		//printf("\nCODE INIT LINE = {%s}\n",line);
		addCode(&($$.C),line);
		//printf("\nCODE ADDED!!\n");
		strcpy($$.type,"int");
		//printf("\nTEMP = %s %s",$$.ADDR,$1);
	}
	;
%%
int main(void) {
	//extern int yydebug;
	//yydebug = 1;
	initSymTab();
	yyparse();
	printf("\n");
	return 0;
}

