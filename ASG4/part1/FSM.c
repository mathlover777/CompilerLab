#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
// Arithmetic operators: +, -, *, /, %, ++, --
// Assignment operators: =, +=, -=, *=, /=
// Relational operators: <,>, <=, >=, ==
// Special symbols: ; ( ) ,(comma) [ ] { }

char Identifier_End[16];

void initialize(){
	Identifier_End[0]='+';
	Identifier_End[1]='-';
	Identifier_End[2]='*';
	Identifier_End[3]='/';
	Identifier_End[4]='%';
	Identifier_End[5]='=';
	Identifier_End[6]='<';
	Identifier_End[7]='>';
	Identifier_End[8]='(';
	Identifier_End[9]=')';
	Identifier_End[10]=',';
	Identifier_End[11]='[';
	Identifier_End[12]=']';
	Identifier_End[13]='{';
	Identifier_End[14]='}';
	Identifier_End[15]=' ';	
	return;
}
int isEnd(char x,char *SYMBOL,int size){
	for(int i=0;i<size;i++){
		if(x==SYMBOL[i]){
			return 1;
		}
	}
	return 0;
}
int isAlpha(char x){
	int n=(int)x;
	if(((n>=65)&&(n<=90))||((n>=97)&&(n<=122))){
		return 1;
	}
	return 0;
}
int isDigit(char x){
	int n=(int)x;
	if((n>=48)&&(n<=57)){
		return 1;
	}
	return 0;
}
int isEOL(char x){
	if(x=='\0'){
		return 1;
	}
	return 0;
}
int isUnderScore(char x){
	if(x=='_'){
		return 1;
	}
	return 0;
}

#define STRINGSIZE 1000
#define FSMCOUNT 7
#define BUFFERSIZE 1000
#define MAXVARSIZE 100
int ENDOFFILE=0;
/****************** Global Arrays and Attributes ********************/
int Begin[FSMCOUNT];
int End[FSMCOUNT];
attrType Attrib[FSMCOUNT];
int Code[FSMCOUNT];

/********************************************************************/


void getString(const char *source,char *dest,int i,int j){
	// dest is already assumed to be created
	int x=0;
	for(int k=i;k<=j;k++){
		if(source[k]=='\0'){
			printf("\nERROR ... source is null in between !!!");
			return;
		}
		dest[x]=source[k];
		x++;
	}
	dest[x]='\0';
	return;
}
void getFloat(const char *source,float *dest,int i,int j){
	int floatflag=0;
	int dotPos=j+1;
	int sign=1;
	if(source[i]=='+'){
		i++;
	}
	if(source[i]=='-'){
		i++;
		sign=-1;
	}
	float integer=0.0;
	float frac=0.0;
	int k=i;
	while(1){
		if(k==j+1){
			break;
		}
		if(source[k]=='.'){
			break;
		}
		integer=integer*10+((int)source[k]-48);
		k++;
	}
	if(k==j+1){
		*dest=(integer)*(sign);
		return;
	}
	if(k==j&&source[k]=='.'){
		printf("\nError in expresssion !");
		*dest=0.0;
		return;
	}
	k++;
	float power=10;
	while(k<=j){
		frac=frac+(((int)source[k]-48)/power);
		power=power*10;
		k++;
	}
	*dest=(integer+frac)*(sign);
	return;

}
void getInt(const char *source,int *dest,int i,int j){
	int sum=0;
	int sign=0;
	if(source[i]=='+'){
		i++;
	}
	if(source[i]=='-'){
		i++;
		sign=1;
	}
	for(int k=i;k<=j;k++){
		if(source[k]=='\0'){
			printf("\nERROR ... source is null in between !!!");
			return;
		}
		sum=sum*10+(source[k]-48);
	}
	if(sign==1){
		sum=sum*(-1);
	}
	*dest=sum;
	return;
}
/*****************FILE read write functions**************************/
char b;
void removeCarraigeReturn(char *line){
	int i=0;
	while((int)line[i]!=13){
		// printf("%c\n",line[i]);
		if(line[i]=='\0'){
			printf("\n7777 End detected...\n");
			scanf("%c",&b);
			return;
		}
		i++;
	}
	line[i]='\0';
	return;
}
void readLine(FILE *fp,char *line){
	// memory for line must be already allocated
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			if(x==EOF){
				ENDOFFILE=1;
			}
			line[i]='\0';
			if((int)line[i-1]==13){
				line[i-1]='\0';// taking care of carriage return....os sensitive
			}
			return;
			// break;
		}else{
			line[i]=x;
		}
		x=getc(fp);
		i++;
	}
	// printf("Carriage return call\n");
	// removeCarraigeReturn(line);
	// printf("Carriage return end\n");
	return;
}
void clearTabs(char *line){
	// removes spaces and tabs from line
	// like "   \t  bbbb   \t\t  rrrr \t  ggg\t  "--->"bbbb rrrr ggg"
	int i,j;
	// convert all tabs into spaces
	// printf("\n*****************BEFORE TAB REMOVAL : '%s'",line);
	i=j=0;
	while(line[i]!='\0'){
		if(line[i]=='\t'||line[i]==9||line[i]==11){
			// printf("\ntab detected !!");
			line[i]=' ';
		}
		i++;
	}
	// printf("\n****************AFTER TAB REMOVAL : '%s'",line);
	// all tabs are replaced by spaces,,,now we will remove redundant spaces[inplace]
	i=j=0;
	while(line[i]==' '){
		i++;
	}
	while(1){
		if(line[i]=='\0'){
			break;
		}
		if(line[i]!=' '){
			line[j]=line[i];
			j++;
			i++;
			continue;
		}
		line[j]=' ';
		i++;
		j++;
		while(line[i]==' '){
			i++;
		}
	}
	if(j-1>=0){
		if(line[j-1]==' '){
			line[j-1]='\0';
		}
	}
	line[j]='\0';
	// printf("\n****************FINAL TAB REMOVAL : '%s'",line);
	return;
}
void resetfile(const char *x){
	FILE *f=fopen(x,"w");
	fclose(f);
	return;
}

/********************************************************************/
int isIdentifier(const char *buffer,int lbegin,int *newbegin,attrType *a){
	char x;
	int state=0;
	int begin=lbegin;
	while(1){
		x=buffer[lbegin];
		switch(state){
			case 0:{
				if(isAlpha(x)==1){
					state=1;
					lbegin++;
					break;
				}
				if(isDigit(x)==1){
					state=3;
					break;
				}
				if(x=='_'){
					state=2;
					lbegin++;
					break;
				}
				state=3;
				break;
			}
			case 1:{
				if(isDigit(x)==1||isAlpha(x)==1||x=='_'){
					state=1;
					lbegin++;
					break;
				}
				state=4;
				break;
			}
			case 2:{
				if(isDigit(x)==1||isAlpha(x)==1||x=='_'){
					state=1;
					lbegin++;
					break;
				}
				state=3;
				break;
			}
			case 3:{
				*newbegin=begin;
				getString(buffer,a->string,begin,lbegin-1);
				return NOTOK;
			}
			case 4:{
				*newbegin=lbegin;
				getString(buffer,a->string,begin,lbegin-1);
				return IDNTIFIER;
			}
		}
	}
}
int isRelationalOp(const char *buffer,int lbegin,int *newbegin,attrType *a){
	int state=0;
	char x;
	int begin=lbegin;
	while(1){
		x=buffer[lbegin];
		switch(state){
			case 0:{
				if(x=='<'){
					state=1;
					lbegin++;
					break;
				}
				if(x=='='){
					state=2;
					lbegin++;
					break;
				}
				if(x=='>'){
					state=3;
					lbegin++;
					break;
				}
				state=7;
				break;
			}
			case 1:{
				if(x=='='){
					state=4;
					lbegin++;
					break;
				}
				state=8;
				break;
			}
			case 2:{
				if(x=='='){
					state=5;
					lbegin++;
					break;
				}
				state=7;
				break;
			}
			case 3:{
				if(x=='='){
					state=6;
					lbegin++;
					break;
				}
				state=10;
				break;
			}
			case 4:{
				*newbegin=lbegin;
				getString(buffer,a->string,begin,lbegin-1);
				return LESS_EQ;
			}
			case 5:{
				*newbegin=lbegin;
				getString(buffer,a->string,begin,lbegin-1);
				return EQ_EQ;	
			}
			case 6:{
				*newbegin=lbegin;
				getString(buffer,a->string,begin,lbegin-1);
				return GREAT_EQ;
			}
			case 7:{
				*newbegin=begin;
				getString(buffer,a->string,begin,lbegin-1);
				return NOTOK;
			}
			case 8:{
				*newbegin=lbegin;
				getString(buffer,a->string,begin,lbegin-1);				
				return LESSER;		
			}
			// case 9:{
			// 	*newbegin=lbegin;
			// 	getString(buffer,a->string,begin,lbegin-1);
			// 	return EQ;
			// }
			case 10:{
				*newbegin=lbegin;
				getString(buffer,a->string,begin,lbegin-1);
				return GREATER;
			}
		}
	}
}
int isArithmeticOp(const char *buffer,int lbegin,int *newbegin,attrType *a){
	int begin=lbegin;
	if(buffer[lbegin]=='\0'){
		*newbegin=lbegin;
		getString(buffer,a->string,begin,lbegin-1);
		return NOTOK;
	}
	if(buffer[lbegin]=='+'){
		lbegin++;
		if(buffer[lbegin]=='+'){
			lbegin++;
			// lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return PLUS_PLUS;
		}else{
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return PLUS;
		}
	}
	if(buffer[lbegin]=='-'){
		lbegin++;
		if(buffer[lbegin]=='-'){
			lbegin++;
			// lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return MINUS_MINUS;
		}else{
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return MINUS;
		}
	}
	if(buffer[lbegin]=='*'){
		lbegin++;
		*newbegin=lbegin;
		getString(buffer,a->string,begin,lbegin-1);
		return MULT;
	}
	if(buffer[lbegin]=='/'){
		lbegin++;
		*newbegin=lbegin;
		getString(buffer,a->string,begin,lbegin-1);
		return DIV;
	}
	if(buffer[lbegin]=='%'){
		lbegin++;
		*newbegin=lbegin;
		getString(buffer,a->string,begin,lbegin-1);
		return PERCENT;
	}
	*newbegin=begin;
	getString(buffer,a->string,begin,lbegin-1);
	return NOTOK;
}
int isFloatingPoint(const char *buffer,int lbegin,int *newbegin,attrType *a){
	int begin=lbegin;
	int state=0;
	char x;
	while(1){
		x=buffer[lbegin];
		switch(state){
			case 0:{
				if(x=='+'){
					state=1;
					lbegin++;
					break;
				}
				if(x=='-'){
					state=2;
					lbegin++;
					break;
				}
				if(x=='.'){
					state=5;
					lbegin++;
					break;
				}
				if(x>=49&&x<=57){
					state=3;
					lbegin++;
					break;
				}
				if(x==48){
					state=4;
					lbegin++;
					break;
				}
				state=8;
				break;
			}
			case 1:{
				if(x>=49&&x<=57){
					state=3;
					lbegin++;
					break;
				}
				if(x==48){
					state=4;
					lbegin++;
					break;
				}
				if(x=='.'){
					state=5;
					lbegin++;
					break;
				}
				state=8;
				break;
			}
			case 2:{
				if(x>=49&&x<=57){
					state=3;
					lbegin++;
					break;
				}
				if(x==48){
					state=4;
					lbegin++;
					break;
				}
				if(x=='.'){
					state=5;
					lbegin++;
					break;
				}
				state=8;
				break;
			}
			case 3:{
				if(x>=48&&x<=57){
					state=3;
					lbegin++;
					break;
				}
				if(x=='.'){
					state=5;
					lbegin++;
					break;
				}
				state=7;
				break;
			}
			case 4:{
				if(x=='.'){
					state=5;
					lbegin++;
					break;
				}
				state=7;
				break;
			}
			case 5:{
				if(x>=48&&x<=57){
					state=6;
					lbegin++;
					break;
				}
				state=8;
				break;
			}
			case 6:{
				if(x>=48&&x<=57){
					state=6;
					lbegin++;
					break;
				}
				state=7;
				break;
			}
			case 7:{
				*newbegin=lbegin;
				getFloat(buffer,&(a->real),begin,lbegin-1);
				return FLO_CONST;
				// break;
			}
			case 8:{
				*newbegin=begin;
				// getString(buffer,a->string,begin,lbegin-1);
				a->real=0.0;
				return NOTOK;
				// break;
			}
		}
	}
}
int isInteger(const char *buffer,int lbegin,int *newbegin,attrType *a){
	int begin=lbegin;
	int state=0;
	char x;
	while(1){
		x=buffer[lbegin];
		switch(state){
			case 0:{
				if(x=='+'){
					state=1;
					lbegin++;
					break;
				}
				if(x=='-'){
					state=2;
					lbegin++;
					break;
				}
				if(x>=49&&x<=57){
					state=3;
					lbegin++;
					break;
				}
				if(x==48){
					state=4;
					lbegin++;
					break;
				}
				state=6;
				break;
			}
			case 1:{
				if(x>=49&&x<=57){
					state=3;
					lbegin++;
					break;
				}
				if(x==48){
					state=4;
					lbegin++;
					break;
				}
				state=6;
				break;
			}
			case 2:{
				if(x>=49&&x<=57){
					state=3;
					lbegin++;
					break;
				}
				if(x==48){
					state=4;
					lbegin++;
					break;
				}		
				state=6;		
				break;
			}
			case 3:{
				if(x>=48&&x<=57){
					state=3;
					lbegin++;
					break;
				}
				state=5;
				break;
			}
			case 4:{
				state=5;
				break;
			}
			case 5:{
				*newbegin=lbegin;
				getInt(buffer,&(a->integer),begin,lbegin-1);
				return INT_CONST;// return integer
				// break;
			}
			case 6:{
				*newbegin=begin;
				// getString(buffer,a->string,begin,lbegin-1);
				a->integer=0;
				printf("here a->integer = %d\n",a->integer);
				return NOTOK;
				// break;
			}
		}
	}
}
int isSpecialSymbol(const char *buffer,int lbegin,int *newbegin,attrType *a){
	int begin=lbegin;
	char x;
	int state=0;
	if(buffer[lbegin]=='\0'){
		*newbegin=lbegin;
		getString(buffer,a->string,begin,lbegin-1);
		return NOTOK;
	}
	switch(buffer[lbegin]){
		case '(':{
			lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return L_PARAEN;
		}
		case ')':{
			lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return R_PARAEN;
		}
		case ',':{
			lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return COMMA;
		}
		case '[':{
			lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return L_SQUARE;
		}
		case ']':{
			lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return R_SQUARE;
		}
		case '{':{
			lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return L_CBRACE;
		}
		case '}':{
			lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return R_CBRACE;
		}
		case ';':{
			lbegin++;
			*newbegin=lbegin;
			getString(buffer,a->string,begin,lbegin-1);
			return SEMICOL;
		}
	}
	*newbegin=begin;
	getString(buffer,a->string,begin,lbegin-1);
	return NOTOK;
}
int isAssignmentOp(const char *buffer,int lbegin,int *newbegin,attrType *a){
	int begin=lbegin;
	char x;
	int state=0;
	while(1){
		x=buffer[lbegin];
		switch(state){
			case 0:{
				if(x=='-'){
					lbegin++;
					state=1;
					break;
				}
				if(x=='+'){
					lbegin++;
					state=2;
					break;
				}
				if(x=='='){
					state=3;
					lbegin++;
					break;
				}
				if(x=='/'){
					state=4;
					lbegin++;
					break;
				}
				if(x=='*'){
					state=5;
					lbegin++;
					break;
				}
				state=10;
				break;
			}
			case 1:{
				if(x=='='){
					state=6;
					lbegin++;
					break;
				}
				state=10;
				break;
			}
			case 2:{
				if(x=='='){
					state=7;
					lbegin++;
					break;
				}
				state=10;
				break;
			}
			case 3:{
				getString(buffer,a->string,begin,lbegin-1);
				*newbegin=lbegin;
				return EQ;
			}
			case 4:{
				if(x=='='){
					state=8;
					lbegin++;
					break;
				}
				state=10;
				break;
			}
			case 5:{
				if(x=='='){
					state=9;
					lbegin++;
					break;
				}
				state=10;
				break;
			}
			case 6:{
				getString(buffer,a->string,begin,lbegin-1);
				*newbegin=lbegin;
				return MINUS_EQ;
			}
			case 7:{
				getString(buffer,a->string,begin,lbegin-1);
				*newbegin=lbegin;
				return PLUS_EQ;
			}
			case 8:{
				getString(buffer,a->string,begin,lbegin-1);
				*newbegin=lbegin;
				return DIV_EQ;
			}
			case 9:{
				getString(buffer,a->string,begin,lbegin-1);
				*newbegin=lbegin;
				return MULT_EQ;
			}
			case 10:{
				getString(buffer,a->string,begin,lbegin-1);
				*newbegin=begin;
				return NOTOK;
			}
		}
	}
}
void initialize_symtab(const char *filename){
	// short sizeof int float double bool char signed unsigned for while do
	// return struct const void switch break case break goto long static union
	// default
	FILE *fp=fopen(filename,"w");
	fprintf(fp, "short 0\n");
	fprintf(fp, "sizeof 0\n");
	fprintf(fp, "int 0\n");
	fprintf(fp, "float 0\n");
	fprintf(fp, "double 0\n");
	fprintf(fp, "bool 0\n");
	fprintf(fp, "char 0\n");
	fprintf(fp, "signed 0\n");
	fprintf(fp, "unsigned 0\n");
	fprintf(fp, "for 0\n");
	fprintf(fp, "while 0\n");
	fprintf(fp, "do 0\n");
	fprintf(fp, "return 0\n");
	fprintf(fp, "struct 0\n");
	fprintf(fp, "const 0\n");
	fprintf(fp, "void 0\n");
	fprintf(fp, "switch 0\n");
	fprintf(fp, "break 0\n");
	fprintf(fp, "case 0\n");
	fprintf(fp, "continue 0\n");
	fprintf(fp, "goto 0\n");
	fprintf(fp, "long 0\n");
	fprintf(fp, "static 0\n");
	fprintf(fp, "union 0\n");
	fprintf(fp, "default 0\n");
	fclose(fp);
	return;
}
int getBestMatch(const int *end){
	int maxmatch,id;
	maxmatch=-2;
	for(int i=0;i<FSMCOUNT;i++){
		if(end[i]>maxmatch){
			maxmatch=end[i];
			id=i;
		}
	}
	return id;
}
int SYMTAB_END=0;
void read_symtab(FILE *fp,char *line){
	// memory for line must be already allocated
	// printf("here...in read symtab\n");
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			if(x==EOF){
				SYMTAB_END=1;
			}
			line[i]='\0';
			return;
		}else{
			line[i]=x;
		}
		x=getc(fp);
		i++;
	}
}
int isSame(const char *a,const char *b){
	// printf("here..\n");
	while(1){
		if((*a)!=(*b)){
			return 0;
		}
		if((*a)=='\0'){
			return 1;
		}
		// printf("a=%c b=%c\n",*a,*b );
		a++;
		b++;
	}
}
void writeline(const char *filename,const char *line){
	// printf("\n*************APPENDING : '%s'  ****\n",line);
	FILE *f=fopen(filename,"a");
	fprintf(f, "%s\n",line);
	fclose(f);
	return;
}
int search_symtab(const char *symtab,const char *query){
	SYMTAB_END=0;
	char symbol[MAXVARSIZE],var[MAXVARSIZE],marker;
	FILE *fp=fopen(symtab,"r");
	int i;
	while(1){
		read_symtab(fp,symbol);
		// printf("%s\n",symbol );
		if(symbol[0]=='\0'&&SYMTAB_END!=1){
			continue;
		}
		if(symbol[0]=='\0'){
			break;
		}
		i=0;
		while(symbol[i]!=' '){
			var[i]=symbol[i];
			i++;
		}
		var[i]='\0';
		// printf("%s\n",var );
		i++;
		marker=symbol[i];
		// printf("%c\n",marker );
		if(isSame(var,query)==1){
			// printf("\nkeyword detected ...\n");
			fclose(fp);
			return ((int)marker-48);		
		}
		if(SYMTAB_END==1){
			break;
		}
	}
	// printf("\nIts a keyword");
	fclose(fp);
	return -1;
}
int getKeywordId(const char *id){
	if(isSame(id,"SHORT")||isSame(id,"short")){
		return SHORT;
	}
	if(isSame(id,"INT")||isSame(id,"int")){
		return INT;
	}
	if(isSame(id,"FLOAT")||isSame(id,"float")){
		return FLOAT;
	}
	if(isSame(id,"DOUBLE")||isSame(id,"double")){
		return DOUBLE;
	}
	if(isSame(id,"BOOL")||isSame(id,"bool")){
		return BOOL;
	}
	if(isSame(id,"CHAR")||isSame(id,"char")){
		return CHAR;
	}
	if(isSame(id,"SIGNED")||isSame(id,"signed")){
		return SIGNED;
	}
	if(isSame(id,"UNSIGNED")||isSame(id,"unsigned")){
		return UNSIGNED;
	}
	if(isSame(id,"FOR")||isSame(id,"for")){
		return FOR;
	}
	if(isSame(id,"WHILE")||isSame(id,"while")){
		return WHILE;
	}
	if(isSame(id,"DO")||isSame(id,"do")){
		return DO;
	}
	if(isSame(id,"RETURN")||isSame(id,"return")){
		return RETURN;
	}
	if(isSame(id,"STRUCT")||isSame(id,"struct")){
		return STRUCT;
	}
	if(isSame(id,"CONST")||isSame(id,"const")){
		return CONST;
	}
	if(isSame(id,"VOID")||isSame(id,"void")){
		return VOID;
	}
	if(isSame(id,"SWITCH")||isSame(id,"switch")){
		return SWITCH;
	}
	if(isSame(id,"BREAK")||isSame(id,"break")){
		return BREAK;
	}
	if(isSame(id,"CASE")||isSame(id,"case")){
		return CASE;
	}
	if(isSame(id,"CONTINUE")||isSame(id,"continue")){
		return CONTINUE;
	}
	if(isSame(id,"GOTO")||isSame(id,"goto")){
		return GOTO;
	}
	if(isSame(id,"LONG")||isSame(id,"long")){
		return LONG;
	}
	if(isSame(id,"STATIC")||isSame(id,"static")){
		return STATIC;
	}
	if(isSame(id,"UNION")||isSame(id,"union")){
		return UNION;
	}
	if(isSame(id,"DEFAULT")||isSame(id,"default")){
		return DEFAULT;
	}
	return NOTOK;
}

int insert_symtab(const char *symtab,const char *id){
	// if string is not present already then it adds that to symtab and returns Identifier
	// if string is a keyword then its respective id is returned 
	char line[100];
	int result=search_symtab(symtab,id);
	// printf("\nInserting '%s' is symtab\n",id);
	if(result==-1){
		// the token is an identifier and not available in symtab
		sprintf(line,"%s 1",id);
		// printf("SYMTAB >> '%s'\n",line );
		writeline(symtab,line);
		return IDNTIFIER;
	}
	if(result==1){
		return IDNTIFIER;
	}
	if(result==0){
		return getKeywordId(id);
	}
}
void addIdentifier_KeyWord(attrType *Attrib,const char *symtab,const char *outfile){
	printf("\ngoing Inserting '%s' is symtab\n",Attrib->string);
	int result=insert_symtab(symtab,Attrib->string);

	char token[100];
	if(result==IDNTIFIER){
		// means inserted it was a identifier
		sprintf(token,"%d %s",IDNTIFIER,Attrib->string);
		printf("outfile >> %s",token);
	}else{
		// its a key word
		result=getKeywordId(Attrib->string);
		sprintf(token,"%d %s",result,Attrib->string);
	}
	writeline(outfile,token);
	return;
}
void add_Symbol(attrType *Attrib,const char *symtab,const char *outfile,int code){
	char token[100];
	sprintf(token,"%d",code);
	writeline(outfile,token);
	return;
}
void add_Numerical(attrType *Attrib,const char *symtab,const char *outfile,int code){
	char token[100];
	if(code==FLO_CONST){
		sprintf(token,"%d %f",code,Attrib->real);
	}else{
		printf("\n$$$$$$$$$$$$$$$$Integer\n");
		sprintf(token,"%d %d",code,Attrib->integer);
	}
	printf("\nNumerical Token {%s}\n",token);
	writeline(outfile,token);
	return;
}
int lineToTokens(const char *line,const char *symtab,const char *outfile){
	int lbegin=0;
	int bestmatch;
	while(1){
		printf("HERE 1\n");
		Code[0]=isIdentifier(line,lbegin,&End[0],&Attrib[0]);
		printf("\n[0]From identifier = '%s'\n",Attrib[0].string);

		Code[1]=isRelationalOp(line,lbegin,&End[1],&Attrib[1]);
		printf("\n[1]From Relational Op ='%s'\n",Attrib[1].string);
		
		Code[2]=isArithmeticOp(line,lbegin,&End[2],&Attrib[2]);
		printf("\n[2]From Artithmetical Op ='%s'\n",Attrib[2].string);
		
		Code[3]=isInteger(line,lbegin,&End[3],&Attrib[3]);
		printf("\n[3]From Integer Point ='%d'\n",Attrib[3].integer);

		Code[4]=isFloatingPoint(line,lbegin,&End[4],&Attrib[4]);
		printf("\n[4]From Floating point ='%f'\n",Attrib[4].real);
		
		Code[5]=isSpecialSymbol(line,lbegin,&End[5],&Attrib[5]);
		printf("\n[5]From Special Symbol ='%s'\n",Attrib[5].string);
		
		Code[6]=isAssignmentOp(line,lbegin,&End[6],&Attrib[6]);
		printf("\n[6]From Assignment Op ='%s'\n",Attrib[6].string);
		


		bestmatch=getBestMatch(End);
		printf("\nBesMatch = %d\n",bestmatch);
		lbegin=End[bestmatch];
		switch(bestmatch){
			case 0:{
				if(Code[0]!=NOTOK){
					addIdentifier_KeyWord(&Attrib[0],symtab,outfile);
					lbegin=End[0];
					break;
				}else{
					printf("\nUnrecognised Lexeme !!!");
					return -1;
				}
			}
			case 1:{
				if(Code[1]!=NOTOK){
					add_Symbol(&Attrib[1],symtab,outfile,Code[1]);
					lbegin=End[1];
					break;
				}else{
					printf("\nUnrecognised Lexeme !!!");
					return -1;
				}
			}
			case 2:{
				if(Code[2]!=NOTOK){
					add_Symbol(&Attrib[2],symtab,outfile,Code[2]);
					lbegin=End[2];
					break;
				}else{
					printf("\nUnrecognised Lexeme !!!");
					return -1;
				}
			}
			case 3:{
				printf("\nGoing to insert integer\n");
				if(Code[3]!=NOTOK){
					add_Numerical(&Attrib[3],symtab,outfile,Code[3]);
					lbegin=End[3];
					break;
				}else{
					printf("\nUnrecognised Lexeme !!!");
					return -1;
				}
			}
			case 4:{
				if(Code[4]!=NOTOK){
					add_Numerical(&Attrib[4],symtab,outfile,Code[4]);
					lbegin=End[4];
					break;
				}else{
					printf("\nUnrecognised Lexeme !!!");
					return -1;
				}
			}
			case 5:{
				if(Code[5]!=NOTOK){
					add_Symbol(&Attrib[5],symtab,outfile,Code[5]);
					lbegin=End[5];
					break;
				}else{
					printf("\nUnrecognised Lexeme !!!");
					return -1;	
				}
			}
			case 6:{
				if(Code[6]!=NOTOK){
					add_Symbol(&Attrib[6],symtab,outfile,Code[6]);
					lbegin=End[6];
					break;
				}else{
					printf("\nUnrecognised Lexeme !!!");
					return -1;
				}
			}
		}
		if(line[lbegin]=='\0'){
			// we have reached the End of the current line
			break;
		}
		if(line[lbegin]==' '){
			lbegin++;
			continue;
		}
	}
}
void printChar(const char *line){
	printf("\nSTART\n");
	while(*line!='\0'){
		printf("%c ASCII = %d$\n",*line,(int)(*line) );
		line++;
	}
	printf("END\n");
	return;
}
void TokenCreator(const char *source,const char *symtab,const char *outfile){
	printf("\nFILENAME = '%s'\n",source);
	char *line=(char *)malloc(sizeof(char)*1000);
	char *debug=(char *)malloc(sizeof(char)*1000);
	// char debug[1000];
	FILE *s=fopen(source,"r");
	ENDOFFILE=0;
	while(1){
		line[0]='\0';
		readLine(s,line);
		// printChar(line);
		// printf("%s",line);
		// printf("\n **LINE = { %s }*** \n",line);
		printf("clearing tabs\n");
		clearTabs(line);
		printf("tabs cleared\n");
		sprintf(debug,"\n**********LINE = [%s]**************\n",line);
		printf("%s",debug);
		// printChar(debug);
		// printf("\n%s\n",line );
		if(line[0]=='\0'&&ENDOFFILE==1){
			break;
		}
		if(line[0]=='\0'){
			continue;
		}
		lineToTokens(line,symtab,outfile);
		if(ENDOFFILE==1){
			break;
		}
	}
	fclose(s);
}
void initialize_All(const char *symtab,const char *outfile){
	initialize_symtab(symtab);
	resetfile(outfile);
	ENDOFFILE=0;
	for(int i=0;i<FSMCOUNT;i++){
		Begin[i]=0;
		End[i]=0;
		Code[i]=0;
	}
	Attrib[0].string=(char *)malloc(sizeof(char)*100);
	Attrib[1].string=(char *)malloc(sizeof(char)*100);
	Attrib[2].string=(char *)malloc(sizeof(char)*100);
	// Attrib[3].string=(char *)malloc(sizeof(char)*100);
	Attrib[5].string=(char *)malloc(sizeof(char)*100);
	Attrib[6].string=(char *)malloc(sizeof(char)*100);
	// Attrib[6].string=(char *)malloc(sizeof(char)*100);
	// Attrib[0].string=(char *)malloc(sizeof(char)*100);
	return;
}
int main(int argc,char **argv){
	system("reset");
	// const char *SOURCE="input.c";
	if(argv[1]==NULL){
		printf("\nNo input file given ,... terminate..\n");
		return 0;
	}
	const char *symtab="symbol_table_1.out";
	const char  *outfile ="a4_1.out";
	initialize_All(symtab,outfile);

	TokenCreator(argv[1],symtab,outfile);
	printf("\n");
	return 0;
}
int getAllId(const char *id){
	if(isSame(id,"IDNTIFIER")==1){
		return IDNTIFIER;
	}
	if(isSame(id,"INT_CONST")==1){
		return INT_CONST;
	}
	if(isSame(id,"FLO_CONST")==1){
		return FLO_CONST;
	}
	if(isSame(id,"PLUS")==1){
		return PLUS;
	}
	if(isSame(id,"MINUS")==1){
		return MINUS;
	}
	if(isSame(id,"DIV")==1){
		return DIV;
	}
	if(isSame(id,"PERCENT")==1){
		return PERCENT;
	}
	if(isSame(id,"PLUS_PLUS")==1){
		return PLUS_PLUS;
	}
	if(isSame(id,"MINUS_MINUS")==1){
		return MINUS_MINUS;
	}
	if(isSame(id,"EQ")==1){
		return EQ;
	}
	if(isSame(id,"PLUS_EQ")==1){
		return PLUS_EQ;
	}
	if(isSame(id,"MINUS_EQ")==1){
		return MINUS_EQ;
	}
	if(isSame(id,"MULT_EQ")==1){
		return MULT_EQ;
	}
	if(isSame(id,"DIV_EQ")==1){
		return DIV_EQ;
	}
	if(isSame(id,"EQ_EQ")==1){
		return EQ_EQ;
	}
	if(isSame(id,"LESSER")==1){
		return LESSER;
	}
	if(isSame(id,"LESS_EQ")==1){
		return LESS_EQ;
	}
	if(isSame(id,"GREATER")==1){
		return GREATER;
	}
	if(isSame(id,"GREAT_EQ")==1){
		return GREAT_EQ;
	}
	if(isSame(id,"L_SQUARE")==1){
		return L_SQUARE;
	}
	if(isSame(id,"R_SQUARE")==1){
		return R_SQUARE;
	}
	if(isSame(id,"L_PARAEN")==1){
		return L_PARAEN;
	}
	if(isSame(id,"R_PARAEN")==1){
		return R_PARAEN;
	}
	if(isSame(id,"L_CBRACE")==1){
		return L_CBRACE;
	}
	if(isSame(id,"R_CBRACE")==1){
		return R_CBRACE;
	}
	if(isSame(id,"COMMA")==1){
		return COMMA;
	}
	if(isSame(id,"SEMICOL")==1){
		return SEMICOL;
	}
	if(isSame(id,"SHORT")){
		return SHORT;
	}
	if(isSame(id,"INT")){
		return INT;
	}
	if(isSame(id,"FLOAT")){
		return FLOAT;
	}
	if(isSame(id,"DOUBLE")){
		return DOUBLE;
	}
	if(isSame(id,"BOOL")){
		return BOOL;
	}
	if(isSame(id,"CHAR")){
		return CHAR;
	}
	if(isSame(id,"SIGNED")){
		return SIGNED;
	}
	if(isSame(id,"UNSIGNED")){
		return UNSIGNED;
	}
	if(isSame(id,"FOR")){
		return FOR;
	}
	if(isSame(id,"WHILE")){
		return WHILE;
	}
	if(isSame(id,"DO")){
		return DO;
	}
	if(isSame(id,"RETURN")){
		return RETURN;
	}
	if(isSame(id,"STRUCT")){
		return STRUCT;
	}
	if(isSame(id,"CONST")){
		return CONST;
	}
	if(isSame(id,"VOID")){
		return VOID;
	}
	if(isSame(id,"SWITCH")){
		return SWITCH;
	}
	if(isSame(id,"BREAK")){
		return BREAK;
	}
	if(isSame(id,"CASE")){
		return CASE;
	}
	if(isSame(id,"CONTINUE")){
		return CONTINUE;
	}
	if(isSame(id,"GOTO")){
		return GOTO;
	}
	if(isSame(id,"LONG")){
		return LONG;
	}
	if(isSame(id,"STATIC")){
		return STATIC;
	}
	if(isSame(id,"UNION")){
		return UNION;
	}
	if(isSame(id,"DEFAULT")){
		return DEFAULT;
	}
	if(isSame(id,"NOTOK")){
		return NOTOK;
	}
}



