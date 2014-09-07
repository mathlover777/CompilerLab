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
	%token RIGHT_BRACES
	%token LEFT_BRACES

%start prog
	%type <type> type
	%type <S> var varList
	%type <dummy> typeList
	%type <EXP> exp expO assignmentStmt readStmt printStmt
	%type <STMT> stmtList stmt stmtListO whileStmt
	%type <STMT> ifStmt elsePart
	%type <STMT> M4 M1 M2 M3 M7
	%type <B> M9 bExp M5 M12 M14 M15 M16
	%type <OP> relOP
%left PLUS MINUS 
%left MULT DIV MOD 
%left DOT


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
		initCode(&($$.C));
		moveCode(&($$.C),&($1.C));
		char Line[200];
		//sprintf(Line,"%s:",$1.next);
		//addCode(&($$.C),Line);
		appendCodeToLeft(&($$.C),&($3.C));
		sprintf(Line,"%s:",$3.next);
		addCode(&($$.C),Line);	
	}
	|M2 stmt
	{
		//printf("\nLABEL at stmtList = {%s}",$<STMT>1.next);
		initCode(&($$.C));
		moveCode(&($$.C),&($2.C));
		char Line[100];
		sprintf(Line,"%s:",$2.next);
		//printf("\nHERE GONE 1 {%s}??\n",Line);
		addCode(&($$.C),Line);
		//printf("\nNO 1\n");	
	}
	;
stmt :	
	assignmentStmt SEMICOLON
	{
		strcpy($$.next,$<STMT>0.next);
		initCode(&($$.C));
		moveCode(&($$.C),&($1.C));
		strcpy($$.type,$1.type);
	}
	|readStmt SEMICOLON
	{
		strcpy($$.next,$<STMT>0.next);
		initCode(&($$.C));
		moveCode(&($$.C),&($1.C));
		strcpy($$.type,$1.type);
	}
	|printStmt SEMICOLON
	{	
		strcpy($$.next,$<STMT>0.next);
		initCode(&($$.C));
		moveCode(&($$.C),&($1.C));
		strcpy($$.type,$1.type);
	}
	|M3 ifStmt{
		initCode(&($$.C));
		moveCode(&($$.C),&($2.C));
		//strcpy($$.next,$2.next);
	}
	|M4 whileStmt{
		strcpy($$.next,$2.next);
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
	IF M5 bExp COLON stmtList M7 elsePart END{
		//ifStmt.Code=bExp.Code||Label(bExp.True)||stmtList.Code||
		//gen(‘goto’ ifstmt.Next||Label(bExp.False)||elsePart.Code)
		printf("\nIN IFSTMT FALSE = {%s}",$3.False);
		printf("\nINIF READ ADDRESS = {%p}",&$$);
		initCode(&($$.C));
		moveCode(&($$.C),&($3.C));
		char Line[100];
		sprintf(Line,"%s:",$3.True);
		addCode(&($$.C),Line);
		appendCodeToLeft(&($$.C),&($5.C));
		sprintf(Line,"goto %s;",$<STMT>-1.next);
		addCode(&($$.C),Line);
		sprintf(Line,"%s:",$3.False);
		addCode(&($$.C),Line);
		appendCodeToLeft(&($$.C),&($7.C));
		//strcpy($$.next,)
	}
	;
elsePart:
	/*empty*/
	{
		initCode(&($$.C));
		//strcpy($$.next,"");
	}
	|ELSE stmtList
	{
		//stmtList.Next=elsePart.Next
		//elsePart.Code=stmtList.Code||Gen(‘goto’ elsePart.Next)
		initCode(&($$.C));
		moveCode(&($$.C),&($2.C));
		char Line[100];
		sprintf(Line,"goto %s",$<STMT>0.next);
		addCode(&($$.C),Line);
	}
	;
	M5:{
		getNewLabel($$.True);
		getNewLabel($$.False);
		printf("\nWE ARE HERE 333!!");
	}
	;
	M3:{
		strcpy($$.next,$<STMT>0.next);
	}
	;
	M7:{
		printf("\nIFNEXT = {%s}",$<STMT>-5.next);
		strcpy($$.next,$<STMT>-5.next);
	}
	;
whileStmt:
	WHILE M9 bExp COLON stmtList END
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
		appendCodeToLeft(&($$.C),&($5.C));
		sprintf(Line,"goto %s",begin);
		addCode(&($$.C),Line);
		strcpy($$.next,$3.False);
		printf("\nWHILE FALSE = {%s}",$$.next);
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
M1:{
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
M4:{
	printf("\nNEXT LABEL = {%s}",$<STMT>0.next);
	strcpy($$.next,$<STMT>0.next);
}
;
bExp:
	bExp OR M12 bExp
	{
		moveCode(&($$.C),&($1.C));
		char Line[200];
		sprintf(Line,"%s:",$1.False);
		addCode(&($$.C),Line);
		appendCodeToLeft(&($$.C),&($4.C));
		strcpy($$.True,$<B>0.True);
		//printf("\nWE HAVE ={%s}",$4.False);
		strcpy($<B>0.False,$4.False);
		strcpy($$.False,$4.False);
		//strcpy($$.False,"2434");
		//printf("\nWE HAVE ={%s}",$$.False);
		//printf("\nINBEXP WRITE ADDRESS = {%p}",&$$);
	}
	|bExp AND M14 bExp
	{
		initCode(&($$.C));
		//bExp.Code=bExp1.Code||Label(bExp1.True)||bExp2.Code
		moveCode(&($$.C),&($1.C));
		char Line[200];
		sprintf(Line,"%s:",$1.True);
		addCode(&($$.C),Line);
		appendCodeToLeft(&($$.C),&($4.C));
		strcpy($$.True,$4.True);
		//strcpy($$.False,$<B>0.False);
	}
	|NOT M15 bExp
	{
		initCode(&($$.C));
		moveCode(&($$.C),&($3.C));
		strcpy($$.True,$3.False);
		strcpy($$.False,$3.True);
	}
	|LEFT_BRACES M16 bExp RIGHT_BRACES
	{
		printf("\nHERE !!");
		initCode(&($$.C));
		moveCode(&($$.C),&($3.C));
		strcpy($$.True,$3.True);
		strcpy($$.False,$3.False);
	}
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
		printf("\nMAY BE ME TOO !!");
		strcpy($$.True,$<B>0.True);
		strcpy($$.False,$<B>0.False);
	}
	;
	M14:
	{
		//strcpy($$.True,$<B>-2.True);
		getNewLabel($$.True);
		strcpy($$.False,$<B>-2.False);
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

	M12:
	{
		strcpy($$.True,$<B>-2.True);
		//strcpy($$.False,$<B>-2.False);
		getNewLabel($$.False);
		printf("\nnew FALSE LABEL ={%s}",$$.False);
	}
	;
	M15:
	{
		strcpy($$.True,$<B>-1.False);
		strcpy($$.False,$<B>-1.True);
	}
	;
	M16:
	{
		strcpy($$.True,$<B>-1.True);
		strcpy($$.False,$<B>-1.False);
		printf("\nM16 : TRUE = {%s} FALSE = {%s}",$$.True,$$.False);
	}
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

