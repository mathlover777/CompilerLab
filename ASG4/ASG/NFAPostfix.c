#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/****************Unimplemented******************/
struct NFA{
	int a;
};
typedef struct NFA NFA;
struct Token{
	int flag;// if flag is 1 means we have NFA
	// flag = 0 we have a symbol...union , concat or aster
	char op;
	NFA a;
};
typedef struct Token Token;
NFA charToNFA(char a){
	NFA b;
	return b; 
}
NFA unionNFA(NFA a,NFA b){
	return a;
	// return 0;
}
NFA aster(NFA a){
	return a;
}
NFA concat(NFA a,NFA b){
	return a;
}
/***********************************************/
struct node{
	char c;
	struct node *next;
};
typedef struct node node;

struct stack{
	node *head;
};
typedef struct stack stack;
struct queue{
	node *head;
	node *tail;
};
typedef struct queue queue;
void init_stack(stack *s){
	s->head=NULL;
	return;
}
void init_queue(queue *q){
	q->head=NULL;
	q->tail=NULL;
	return;
}
void stack_push(stack *s,char x){
	node *temp=s->head;
	s->head=(node *)malloc(sizeof(node));
	s->head->c=x;
	s->head->next=temp;
	return;
}
void enqueue(queue *q,char x){
	if(q->tail==NULL){
		q->head=(node *)malloc(sizeof(node));
		q->head->c=x;
		q->head->next=NULL;
		q->tail=q->head;
		return;
	}
	q->tail->next=(node *)malloc(sizeof(node));
	q->tail=q->tail->next;
	q->tail->c=x;
	q->tail->next=NULL;
	return;
}
char stack_top(stack *s){
	if(s->head==NULL){
		return '\0';
	}
	return s->head->c;
}
char queue_top(queue *q){
	if(q->head==NULL){
		return '\0';
	}
	return q->head->c;
}
void stack_pop(stack *s){
	if(s->head==NULL){
		return;
	}
	node *temp=s->head->next;
	free(s->head);
	s->head=temp;
	return;
}
void dequeue(queue *q){
	if(q->head==NULL){
		return;
	}
	if(q->head==q->tail){
		free(q->head);
		q->head=NULL;
		q->tail=NULL;
		return;
	}
	node *temp=q->head->next;
	free(q->head);
	q->head=temp;
	return;
}
int isStackEmpty(stack *s){
	if(s->head==NULL){
		return 1;
	}
	return 0;
}
int isQueueEmpty(queue *q){
	if(q->head==NULL){
		return 1;
	}
	return 0;
}
void queueToString(queue *q,char *a){
	char x;
	int i=0;
	while(1){
		if(isQueueEmpty(q)==1){
			break;
		}
		x=queue_top(q);
		dequeue(q);
		a[i]=x;
		i++;
	}
	a[i]='\0';
}
void copyString(const char *source,char *dest){
	while((*source)!='\0'){
		(*dest)=(*source);
		dest++;
		source++;
	}
	(*dest)='\0';
	return;
}
void replaceBraces(char *string){
	while((*string)!='\0'){
		switch(*string){
			case '[':{
				(*string)='(';
				break;
			}
			case ']':{
				(*string)=')';
				break;
			}
		}
		string++;
	}
	return;
}
void replaceRange(char *a,char *b){
	int i=0,j=0;
	char first,last;
	while(1){
		if(a[i]=='\0'){
			b[j]='\0';
			return;
		}
		if(a[i]=='-'){
			// we detected a range sign like <char1>-<charn> going to replace it like
			// (<char1>|<char2>|....|<charn-1>|<charn>)
			// currently <char1> is written and writehead is pointing at the next pos
			j--;
			first=a[i-1];
			last=a[i+1];
			b[j]='(';
			j++;
			while(first<last){
				b[j]=first;
				j++;
				b[j]='|';
				j++;
				first=(char)((int)first+1);
			}
			b[j]=first;
			j++;
			b[j]=')';
			j++;
			i++;
			i++;
		}else{
			b[j]=a[i];
			j++;
			i++;
		}
	}
}
int isOperator(char x){
	if(x=='+'||x=='*'||x=='|'){
		return 1;
	}
	return 0;
	// everything else is a valid symbol
}
int isSymbol(char x){
	if((isOperator(x)==0)&&x!='('&&x!=')'){
		return 1;
	}
	return 0;
}
void addPlus(const char *a,char *b){
	int i=0,j=0;
	if(a[i]=='\0'){
		b[i]='\0';
		return;
	}
	while(1){
		if(a[i+1]=='\0'){
			b[j]=a[i];
			j++;
			b[j]='\0';
			break;
		}
		// printf("%d %d %d\n",(a[i]=='*'),(isSymbol(a[i+1])==1||a[i+1]=='('),((a[i]=='*')&&(isSymbol(a[i+1])==1||a[i+1]=='('))   );
		// printf("a[i] = %c a[i+1]=%c",a[i],a[i+1] );
		// printf(" isSymbol(a[i+1])=%d\n",isSymbol(a[i+1]));
		if((  isSymbol(a[i])==1&&(isSymbol(a[i+1])==1||(a[i+1]=='('))  )||  (isSymbol(a[i])==1&&(a[i+1]=='(')) 
			|| (isSymbol(a[i+1])==1&&(a[i]==')')) || ((a[i]==')')&&(a[i+1]=='(')) || 
			((a[i]=='*')&&(isSymbol(a[i+1])==1||a[i+1]=='('))  ){
			printf("A[i]=%c\n",a[i] );
			b[j]=a[i];
			j++;
			b[j]='+';
			j++;
			i++;
			continue;
		}else{
			b[j]=a[i];
			j++;
			i++;
		}
	}
}

void infixToPostFix(const char *input,char *postfix){
	// the strings are already assumed to be present
	// currently symbols --->	'-' range operator
	// 							'|' for union
	//							'*' as usual
	// []() are essentially the same
	// permissible symbols are Alphabets,numerical,dot and everything else 
	char a[1000],b[1000],c[1000];
	copyString(input,a);
	printf("\nGiven Input :\n{%s}\n",a);
	replaceBraces(a);
	printf("\nAfter changing braces :\n{%s}\n",a);
	replaceRange(a,c);
	printf("\nAfter replacing the range operator :\n{%s}\n",c);
	addPlus(c,b);
	printf("\nAfter adding the extra +(s) :\n{%s}\n",b);
	/***********We will process on the string b********************/

	stack S;
	queue Q;
	init_queue(&Q);
	init_stack(&S);
	int i=0;
	char stacktop;
	while(1){
		if(b[i]=='\0'){
			// break the while loop and transfer everything stack to queue
			break;
		}
		switch(b[i]){
			case '+':{
				while(1){
					if(isStackEmpty(&S)==1){
						break;
					}
					stacktop=stack_top(&S);
					if(stacktop=='+'||stacktop=='|'||stacktop=='*'){
						stack_pop(&S);
						enqueue(&Q,stacktop);
					}else{
						break;
					}
				}
				stack_push(&S,b[i]);
				break;
			}
			case '|':{
				while(1){
					if(isStackEmpty(&S)==1){
						break;
					}
					stacktop=stack_top(&S);
					if(stacktop=='|'||stacktop=='*'){
						stack_pop(&S);
						enqueue(&Q,stacktop);
					}else{
						break;
					}
				}
				stack_push(&S,b[i]);
				break;
			}
			case '*':{
				while(1){
					if(isStackEmpty(&S)==1){
						break;
					}
					stacktop=stack_top(&S);
					if(stacktop=='*'){
						stack_pop(&S);
						enqueue(&Q,stacktop);
					}else{
						break;
					}
				}
				stack_push(&S,b[i]);
				break;
			}
			case '(':{
				stack_push(&S,b[i]);
				break;
			}
			case ')':{
				while(1){
					if(isStackEmpty(&S)==1){
						printf("\nParenthesis Missmatch !!\n");
						return;
					}
					stacktop=stack_top(&S);
					if(stacktop=='('){
						stack_pop(&S);
						break;
					}else{
						stack_pop(&S);
						enqueue(&Q,stacktop);
						// break;
					}
				}
				break;
			}
			default:{
				// otherwise the character is taken as a valid symbol
				enqueue(&Q,b[i]);
			}
		}
		i++;
	}
	while(1){
		if(isStackEmpty(&S)==1){
			break;
		}
		stacktop=stack_top(&S);
		if(stacktop=='('||stacktop==')'){
			printf("\nParenthesis Missmatch!!!\n");
			return;
		}
		enqueue(&Q,stacktop);
		stack_pop(&S);
	}
	printf("\nPostfix created successfully\n");
	queueToString(&Q,postfix);
	printf("\nPostfix\n :{%s}\n",postfix);
	return;
}




int main(){
	char postfix[1000];
	// infixToPostFix("[(_(a-c))(A-D)][_(a-b)(A-B)(0-2)]*",postfix);
	// infixToPostFix("a+(b+c)",postfix);
	// infixToPostFix("[0-9]*.[0-9]*",postfix);
	infixToPostFix("(a|b)*abb",postfix);
	// infixToPostFix("0|[1-9][0-9]*",postfix);
	return 0;
}


/*************************************************************************************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

#define MAXNFACOUNT 30

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
void getBestMatch(const int *end,int nfaCount,int *match1,int *match2){
	(*match2)=-1;
	(*match1)=0;
	int match1len,match2len;
	match1len=end[0];
	for(int i=1;i<nfaCount;i++){
		if(end[i]>match1len){
			match2len=match1len;
			(*match2)=(*match1);
			match1len=end[i];
			(*match1)=i;
			continue;
		}
		if(end[i]==match1len){
			match2len=end[i];
			(*match2)=i;
			continue;
		}
	}
	return;
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
	char symbol[MAXVARSIEZE],var[MAXVARSIEZE],marker;
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
			printf("\nkeyword detected ...\n");
			fclose(fp);
			return ((int)marker-48);		
		}
		if(SYMTAB_END==1){
			break;
		}
	}
	printf("\nIts a keyword");
	fclose(fp);
	return -1;
}
int getKeywordId(const char *id){
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
}
int insert_symtab(const char *symtab,const char *id){
	// if string is not present already then it adds that to symtab and returns Identifier
	// if string is a keyword then its respective id is returned 
	char line[100];
	int result=search_symtab(symtab,id);
	printf("\nInserting '%s' is symtab\n",id);
	if(result==-1){
		// the token is an identifier and not available in symtab
		sprintf(line,"%s 1",id);
		printf("SYMTAB >> '%s'\n",line );
		writeline(symtab,line);
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
		sprintf(token,"%d %s",result,Attrib->string);
	}
	writeline(outfile,token);
	return;
}
void addIdentifier(attrType *Attrib,const char *symtab,const char *outfile){
	printf("\ngoing Inserting '%s' is symtab\n",Attrib->string);
	int result=insert_symtab(symtab,Attrib->string);
	char token[100];
	sprintf(token,"%d %s",IDNTIFIER,Attrib->string);
	printf("outfile >> %s",token);
	writeline(outfile,token);
	return;
}
void addKeyword(attrType *Attrib,const char *symtab,const char *outfile,int code){
	// printf("\ngoing Inserting '%s' is symtab\n",Attrib->string);
	char token[100];
	sprintf(token,"%d %s",code,Attrib->string);
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
	sprintf(token,"%d %s",code,Attrib->string);
	printf("\nNumerical Token {%s}\n",token);
	writeline(outfile,token);
	return;
}
int lineToTokens(const char *line,const char *symtab,const char *outfile,NFA *N,int nfaCount){
	int lbegin=0;
	int bestmatch;
	int match1,match2;
	int Code[MAXNFACOUNT];
	int End[MAXNFACOUNT];
	attrType Attrib[MAXNFACOUNT];
	for(int i=0;i<MAXNFACOUNT;i++){
		Attrib[i].string=(char *)malloc(sizeof(char)*100);
	}
	while(1){
		for(int i=0;i<nfaCount;i++){
			simulate_NFA(&N[i],line,lbegin,&End[i],&Attrib[i]);
		}
		getBestMatch(End,nfaCount,&match1,&match2);
		printf("\nMatch1 = %d\tMatch2 = %d",match1,match2);
		if(match2==-1){
			bestmatch=match1;
		}else if(match1==match2){
			if(getNFAID(&N[match1])==IDNTIFIER){
				bestmatch=match2;
			}else{
				bestmatch=match1;
			}
		}else{
			printf("\nFatal error in matching !!\n");
			exit(-1);
		}
		lbegin=End[bestmatch];
		switch(Code[bestmatch]){
			case IDNTIFIER:{
				addIdentifier(&Attrib[bestmatch],symtab,outfile);
				break;
			}
			case INT_CONST:{
				add_Numerical(&Attrib[bestmatch],symtab,outfile,INT_CONST);
				break;	
			}
			case FLO_CONST:{
				add_Numerical(&Attrib[bestmatch],symtab,outfile,FLO_CONST);
				break;
			}
			case NOTOK:{
				printf("\nLexeme not detected in any of the NFA !!!\n");
				exit(-1);
				// break;
			}
			default:{
				if((Code[bestmatch]>=422)&&(Code[bestmatch]<=445)){
					// treat lexeme as keyword
					addKeyWord(&Attrib[bestmatch],symtab,outfile,code[bestmatch]);
				}else{
					// treat lexeme as symbol
					add_Symbol(&Attrib[bestmatch],symtab,outfile,code[bestmatch]);
				}
			}
		}
		lbegin=End[bestmatch];
		if(line[lbegin]=='\0'){
			// we have reached the End of the current line
			break;
		}
		if(line[lbegin]==' '){
			lbegin++;
			continue;
		}
	}

	for(int i=0;i<MAXNFACOUNT;i++){
		free(Attrib[i].string);
		Attrib[i].string=NULL;
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
void TokenCreator(const char *source,const char *symtab,const char *outfile,NFA *N,int nfaCount){
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
		lineToTokens(line,symtab,outfile,N,nfaCount);
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
	// for(int i=0;i<FSMCOUNT;i++){
	// 	Begin[i]=0;
	// 	End[i]=0;
	// 	Code[i]=0;
	// }
	// Attrib[0].string=(char *)malloc(sizeof(char)*100);
	// Attrib[1].string=(char *)malloc(sizeof(char)*100);
	// Attrib[2].string=(char *)malloc(sizeof(char)*100);
	// // Attrib[3].string=(char *)malloc(sizeof(char)*100);
	// Attrib[5].string=(char *)malloc(sizeof(char)*100);
	// Attrib[6].string=(char *)malloc(sizeof(char)*100);
	// // Attrib[6].string=(char *)malloc(sizeof(char)*100);
	// // Attrib[0].string=(char *)malloc(sizeof(char)*100);
	return;
}
int InitializeAllNFA(const char *regfile,int actualReg,NFA *N){
// should return NFA Count....

}
int simulate_NFA(NFA *N,const char *string,int lexemebegin,int *newbegin,attrType *Attrib){

// if accepted return ID
// else return -1
}