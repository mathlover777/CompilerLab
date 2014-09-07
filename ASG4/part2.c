/////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
/////////////////////////////////////////////////////////////////////////////////////
#define maxstates 300
#define maxregexp 1000
#define maxnumbernfa 100
#define maxbufferlength 1000
#define maxattributesize 30
#define maxregexpsize 100
#define checkendstateidx 257
#define EpsilonIdx 256
#define OSTAR 42
#define OADD 43
#define OPIPE 124
/////////////////////////////////////////////////////////////////////////////////////
typedef struct _node{
	struct _node* next;
	int nstate;
}node;

typedef struct _Transition{
	int possibletransitions;
	node* head;
}Transition;//LL

typedef struct _NFA{
	Transition table[maxstates][258];
	int numberofstates;
	int AcceptState;
	int Attribute;
	char* RegExpName;
}NFA;
/*
	USING MY NFA DATA STRUCTURE
	NFA DATA STUCTURE IS DESIGNED IN TABLE FORM
	EVERY TABLE ENTRY IS OF TYPE TRANSITION 
	WHICH STORES ALL THE SET OF STATES 
	IN THE CODOMAIN OF TRANSITION FUNCTION
	possibletransitions=-1 MEANS MACHINE GETS STUCK FOR THAT INPUT
	possibletransitions=0 Init value
	table[][256] is special reserved Transition for Epsilon
	table[][257] is reserved for indicating whether it is End State(1) else (0)
	START STATE::0
	END STATES::Mentioned in AcceptState SINGLE END STATE
*/
typedef struct _RegExp{
	char Attribute[maxattributesize];
	char Expression[maxregexpsize];
}RegExp;
typedef struct _Token{
	int TokenId;
	char* Name;
}Token;

typedef struct _sintnode{
	int n;
	struct _sintnode *next;
}sintnode;
typedef struct _intstack{
	sintnode *head;
}intstack;
struct scharnode{
	char c;
	struct scharnode *next;
};
typedef struct scharnode scharnode;

struct stack{
	scharnode *head;
};
typedef struct stack stack;
struct queue{
	scharnode *head;
	scharnode *tail;
};
typedef struct queue queue;

typedef struct _NFAnode{
	NFA n;
	struct _NFAnode *next;
}NFAnode;

typedef struct _NFAstack{
	NFAnode *head;
}NFAstack;
/////////////////////////////////////////////////////////////////////////////////////
// int ENDOFFILE=0;
int mreadLine(FILE *fp,char *line){
	// memory for line must be already allocated
	// ENDOFFILE=0;
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			if(x==EOF){
				// ENDOFFILE=1;
				line[i]='\0';
				return 1;
			}
			line[i]='\0';
			return 0;
		}else{
			line[i]=x;
		}
		x=getc(fp);
		i++;
	}
}
void writeline(FILE* fp,char *line){
	fprintf(fp, "%s\n",line);
	return;
}
/////////////////////////////////////////////////////////////////////////////////////
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
	scharnode *temp=s->head;
	s->head=(scharnode *)malloc(sizeof(scharnode));
	s->head->c=x;
	s->head->next=temp;
	return;
}
void enqueue(queue *q,char x){
	if(q->tail==NULL){
		q->head=(scharnode *)malloc(sizeof(scharnode));
		q->head->c=x;
		q->head->next=NULL;
		q->tail=q->head;
		return;
	}
	q->tail->next=(scharnode *)malloc(sizeof(scharnode));
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
	scharnode *temp=s->head->next;
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
	scharnode *temp=q->head->next;
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
            //printf("A[i]=%c\n",a[i] );
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
	//printf("\nGiven Input :\n{%s}\n",a);
	replaceBraces(a);
	//printf("\nAfter changing braces :\n{%s}\n",a);
	replaceRange(a,c);
	//printf("\nAfter replacing the range operator :\n{%s}\n",c);
	addPlus(c,b);
	//printf("\nAfter adding the extra +(s) :\n{%s}\n",b);
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
						//printf("\nParenthesis Missmatch !!\n");
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
			//printf("\nParenthesis Missmatch!!!\n");
			return;
		}
		enqueue(&Q,stacktop);
		stack_pop(&S);
	}
	//printf("\nPostfix created successfully\n");
	queueToString(&Q,postfix);
	//printf("\nPostfix\n :{%s}\n",postfix);
	return;
}
/////////////////////////////////////////////////////////////////////////////////////
void init_intstack(intstack *s){
	s->head=NULL;
	return;
}
int isintStackEmpty(intstack *s){
	if(s->head==NULL){
		return 1;
	}
	return 0;
}
void intstack_push(intstack *s,int x){
	sintnode *temp=s->head;
	s->head=(sintnode *)malloc(sizeof(sintnode));
	s->head->n=x;
	s->head->next=temp;
	return;
}
int intstack_top(intstack *s){
	if(s->head==NULL){
		return '\0';
	}
	return s->head->n;
}
void intstack_pop(intstack *s){
	if(s->head==NULL){
		return;
	}
	sintnode *temp=s->head->next;
	free(s->head);
	s->head=temp;
	return;
}
/////////////////////////////////NFA STACK FUNCTIONS/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// PRINT NFA ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void printLinked(Transition t)
{
	node* dummy=t.head;
	if(dummy!=NULL)
	{
		//printf("\nENTERED NOT NULL");
		if(dummy->next==NULL)
		{
			printf(" #%d# ",dummy->nstate);
			return;
		}
		while(dummy->next!=NULL)
		{
			printf(" #%d# ",dummy->nstate);
			dummy=dummy->next;	
		}
		printf(" #%d# ",dummy->nstate);
	}
	else
	{
		//printf("\nNULL!!!!!!");
	}
}
void printNFA(NFA* n)
{
	int i,j;
	printf("\n:::::::::::::::::PRINTING NFA::::::::::::::::::::::::\n");
	printf("::::NUMBER OF STATES:%d \n",n->numberofstates);
	printf("::::::::ACCEPT STATE:%d \n",n->AcceptState );
	//printf(":::::::::::::::::::ATTRIBUTE:%d \n",n->Attribute);
	//printf("::::::::::REGULAR EXPRESSION:%s \n",n->RegExpName);
	for(i=0;i<n->numberofstates;i++)
	{
		printf("\nSTATE:%d ",i);
		for(j=0;j<256;j++)
		{
			if(n->table[i][j].possibletransitions!=0)
			{
				printf("  %c-->",j);
				printLinked(n->table[i][j]);
			}
		}
		j=256;
		if(n->table[i][j].possibletransitions!=0)
		{
			printf("  Epsilon-->");
			printLinked(n->table[i][j]);
		}
	}
	printf("\n:::::::::::::::NFA PRINTING ENDED ::::::::::::::::::::\n\n");
}
/////////////////////////////////////////////////////////////////////////////////
void init_NFAstack(NFAstack *s){	//ALREADY ALLOCATED!!
	//s=(NFAstack*)malloc(sizeof(NFAstack));
	s->head=NULL;
	return;
}
void NFAstack_push(NFAstack *s,NFA x){
	NFAnode *temp=s->head;
	s->head=(NFAnode *)malloc(sizeof(NFAnode));
	s->head->n=x;
	s->head->next=temp;
	return;
}
NFA NFAstack_top(NFAstack *s){
	return s->head->n;
}
void NFAstack_pop(NFAstack *s){
	if(s->head==NULL){
		return;
	}
	NFAnode *temp=s->head->next;
	free(s->head);
	s->head=temp;
	return;
}
int isNFAStackEmpty(NFAstack *s){
	if(s->head==NULL){
		return 1;
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////
NFA* allocNFA()
{
	NFA* n;
	n=(NFA*)malloc(sizeof(NFA));
	return n;
}
void init(NFA* n)
{
	int i,j,k;
	n->numberofstates=0;
	n->Attribute=0;
	n->AcceptState=0;
	for(i=0;i<maxstates;i++)
	{
		for(j=0;j<258;j++)
		{
			n->table[i][j].possibletransitions=0;
			//free(n->table[i][j].head);
			n->table[i][j].head=NULL;
		}
	}
}
void initalloc(NFA* n)//ALREADY ALLOCATED
{
	int i,j,k;
	n->numberofstates=0;
	n->Attribute=0;
	n->RegExpName=NULL;
	for(i=0;i<maxstates;i++)
	{
		for(j=0;j<maxstates;j++)
		{
			for(k=0;k<maxstates;k++)
			{
				n->table[i][j].possibletransitions=0;
				n->table[i][j].head=NULL;
			}
		}
	}
}
void freerec(node* n)
{
	if(n==NULL) return;
	if(n->next==NULL) free(n);
	else freerec(n->next);
}
void freeNFA(NFA* in)
{
	int i;
	int j;
	if(in==NULL)
	{
		printf("NULLLLLL\n");
		exit(1);
	}
	for(i=0;i<in->numberofstates;i++)
	{
		for(j=0;j<258;j++)
		{
			if(in->table[i][j].head!=NULL) 
			{
				//freerec(in->table[i][j].head);
				//free(in->table[i][j].head);
			}
		}
	}
	//free(in);
}
void MakeEndState(NFA * a,int state)//State is already include in number
{
	a->AcceptState=state;
}
void AddStateToTransition(NFA* n,int i,int j,int statetobeadded)
{
	node* dummy;
	node* temp;
	temp=(node*)malloc(sizeof(node));
	temp->nstate=statetobeadded;
	temp->next=NULL;
	dummy=n->table[i][j].head;
	n->table[i][j].possibletransitions=n->table[i][j].possibletransitions+1;
	if(dummy==NULL)
	{
		n->table[i][j].head=temp;
		//printf("\n ADDED %d",statetobeadded);
		return;
	}
	while(dummy->next!=NULL)
	{
		dummy=dummy->next;
	}
	dummy->next=temp;
	//printf("\n ADDED %d",statetobeadded);
}
////////////////////////////THOMPSON YAMADA CONSTRUCTION/////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
NFA singleNFA(char ch)
{
	NFA out;
	init(&out);
	out.numberofstates=2;
	MakeEndState(&out,1);
	AddStateToTransition(&out,0,ch,1);
	//printf("\nPRINTING FROM singleNFA>>>>>>>>>.");
	//printNFA(&out);
	return out;
}
void CopyNFA(NFA* in,NFA* copy,int statenum)
{
	int i,j,k;
	int nstates_copy;
	nstates_copy=statenum;
	node* dummy1;
	node* dummy2;
	for(i=0;i<in->numberofstates;i++)
	{
		for(j=0;j<258;j++)
		{
			dummy1=copy->table[i+nstates_copy][j].head;
			dummy2=in->table[i][j].head;
			for(k=0;k<in->table[i][j].possibletransitions;k++)
			{
				AddStateToTransition(copy,i+nstates_copy,j,dummy2->nstate+nstates_copy);
				if(dummy2->next==NULL)
				{

					break;
				}
				dummy2=dummy2->next;
			}
		}
	}
	copy->numberofstates=nstates_copy+in->numberofstates;
}
NFA concatNFA(NFA* a,NFA* b)//CHANGE MAY BE NEEDED !CLUB START STATE AND END STATE INSTEAD OF ADDING EPSILON TRANSITION
{
	//printf("\n***********************");
	NFA c;
	init(&c);
	CopyNFA(a,&c,0);
	//printf("\nAfter Copying A TO C\n");
	//printNFA(&c);
	CopyNFA(b,&c,c.numberofstates-1);
	//printf("\nAfter Copying B TO C\n");
	//printNFA(&c);
	MakeEndState(&c,b->AcceptState+a->numberofstates-1);
	//AddStateToTransition(&c,a->AcceptState,EpsilonIdx,a->numberofstates);
	//printf("\nAfter concatNFA \n");
	//printNFA(&c);
	freeNFA(a);
	freeNFA(b);
	//printf("\n***********************");
	return c;
}
NFA starNFA(NFA* a)
{
	NFA s;
	init(&s);
	s.numberofstates=1;
	AddStateToTransition(&s,0,EpsilonIdx,1);
	AddStateToTransition(&s,0,EpsilonIdx,a->numberofstates+1);
	CopyNFA(a,&s,1);
	AddStateToTransition(&s,1+a->AcceptState,EpsilonIdx,1);
	AddStateToTransition(&s,1+a->AcceptState,EpsilonIdx,a->numberofstates+1);
	MakeEndState(&s,a->numberofstates+1);
	s.numberofstates=s.numberofstates+1;
	freeNFA(a);
	return s;
}
NFA unionNFA(NFA* a,NFA* b)
{
	NFA u;
	init(&u);
	u.numberofstates=1;
	AddStateToTransition(&u,0,EpsilonIdx,1);
	AddStateToTransition(&u,0,EpsilonIdx,a->numberofstates+1);
	//printf("\n********BEFORE COPY A TO U::\n");
	//printNFA(&u);
	CopyNFA(a,&u,1);
	//printf("\n**************COPY A TO U::\n");
	//printNFA(&u);
	CopyNFA(b,&u,a->numberofstates+1);
	u.numberofstates=u.numberofstates+1;
	AddStateToTransition(&u,a->AcceptState+1,EpsilonIdx,u.numberofstates-1);
	AddStateToTransition(&u,b->AcceptState+a->numberofstates+1,EpsilonIdx,u.numberofstates-1);
	MakeEndState(&u,u.numberofstates-1);
	//printf("\nPRINTING UNION FROM FUNCTION\n");
	//printNFA(&u);
	freeNFA(a);
	freeNFA(b);
	return u;
}
/////////////////////////////REGULAR EXPRESSION TO NFA///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void splitRegExp(char* line,RegExp* r)
{
	int i=0,j=0;
	for(;line[i]!=' ';i++)
	{
		r->Attribute[i]=line[i];
	}
	r->Attribute[i-j]='\0';
	i++;
	j=i;	
	for(;line[i]!='\0';i++)
	{
		r->Expression[i-j]=line[i];
	}
	r->Expression[i-j]='\0';
}
int SearchIAttribute(char* CAttribute)
{
	int n;
	return n;
}
///////////////////////////FORMATTING THE INPUT//////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void readfileinbuffer(FILE* fp,char* buff)
{
	int i;
	char str[maxbufferlength];
	fscanf(fp,"%s",str);
	strcpy(str,buff);
}
void clearTabs(char *line){
	int i,j;
	i=j=0;
	while(line[i]!='\0'){
		if(line[i]=='\t'||line[i]==9||line[i]==11){
			line[i]=' ';
		}
		i++;
	}
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
	return;
}
void printToken(FILE* tokenfile,Token in)
{

}
Token generateToken(char* inputbuffer,int maxmatch)
{
	Token t;
	return t;
}
////////////////////////AUXILLARY FUNCTIONS FOR SIMULATION OF NFA////////////////////
/////////////////////////////////////////////////////////////////////////////////////
int SearchArray(int* Array,int size,int state)
{
	int r=0;
	int i;
	for(i=0;i<size;i++)
	{
		if(Array[i]==state) r=1;
	}
	return r;
}
void AddStateToArray(int* Array,int* size,int state)
{
	Array[*size]=state;
	*size=*size+1;
}
void EpsilonClosure(NFA* n,int* inputStates,int numberofstates,int* Closure,int* numberofstatesClosure)//Closure ALREADY ALLOCATED
{
	int i;
	int j;
	*numberofstatesClosure=0;
	node* dummy;
	intstack* remaining;
	int t;
	init_intstack(remaining);
	for(i=0;i<numberofstates;i++)
	{
		intstack_push(remaining,inputStates[i]);
	}
	while(isintStackEmpty(remaining))
	{
		t=intstack_top(remaining);
		intstack_pop(remaining);
		dummy=n->table[t][EpsilonIdx].head;
		if(dummy!=NULL)
		{
			while(dummy->next==NULL)
			{
				if(!SearchArray(Closure,*numberofstatesClosure,dummy->nstate))
				{
					AddStateToArray(Closure,numberofstatesClosure,dummy->nstate);
					intstack_push(remaining,dummy->nstate);
				}
			}
		}	
	}
	return;
}
void move(NFA* n,int* inputStates,int numberofstates,int* Move,int* numberofstatesMove,int ch)//Move ALREADY ALLOCATED
{
	int i;
	node* dummy;
	*numberofstatesMove=0;
	for(i=0;i<numberofstates;i++)
	{
		dummy=n->table[i][(int)ch].head;
		if(dummy!=NULL)
		{
			while(dummy->next==NULL)
			{
				if(!SearchArray(Move,*numberofstatesMove,dummy->nstate))
				{
					AddStateToArray(Move,numberofstatesMove,dummy->nstate);
				}
			}
		}
	}
}
//POSTFIX FORM OF THE REGULAR EXPRESSION CAN CONTAIN + OR | OR * OR DIGITS OR LETTERS (CAPITAL OR SMALL)
//RegExpToNFA Not to be used for Symbols 
void RegExpToNFA(RegExp* r,int regexpsize,NFA* a)//INPUT REGULAR EXPRSSION IN POSTFIX FORM
{
	NFA* temp1;
	NFA* temp2;
	NFA* temp3;

	init(a);
	// init(temp1);
	// init(temp2);
	// init(temp3);
	int i;
	char f1,f2;
	NFAstack* process;
	process=(NFAstack*)malloc(sizeof(NFAstack));
	init_NFAstack(process);
	for(i=0;i<regexpsize;i++)
	{
		temp1=allocNFA();
		temp2=allocNFA();
		temp3=allocNFA();
		init(temp1);
		init(temp2);
		init(temp3);
		//printf("PRINTING INIT TEMP3\n");
		//printNFA(temp3);
		if(r->Expression[i]==OSTAR)
		{
			printf("ENCOUNTERED *\n");
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			*temp1=NFAstack_top(process);
			NFAstack_pop(process);
			*temp3=starNFA(temp1);
			NFAstack_push(process,*temp3);
			freeNFA(temp1);
			//freeNFA(temp3);
		}
		else if(r->Expression[i]==OADD)
		{
			//printf("ENCOUNTERED +\n");
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			*temp1=NFAstack_top(process);
			NFAstack_pop(process);	
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			*temp2=NFAstack_top(process);
			//printf("TEMP1\n");
			//printNFA(temp1);
			//printf("TEMP2\n");
			//printNFA(temp2);
			NFAstack_pop(process);
			*temp3=concatNFA(temp2,temp1);
			//printf("TEMP3\n");
			//printNFA(temp3);
			NFAstack_push(process,*temp3);	
			freeNFA(temp1);
			freeNFA(temp2);
			//freeNFA(temp3);
		}
		else if(r->Expression[i]==OPIPE)
		{
			//printf("ENCOUNTERED |\n");
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			*temp1=NFAstack_top(process);
			NFAstack_pop(process);	
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			*temp2=NFAstack_top(process);
			NFAstack_pop(process);
			*temp3=unionNFA(temp1,temp2);
			NFAstack_push(process,*temp3);		
			freeNFA(temp1);
			freeNFA(temp2);
			//freeNFA(temp3);		
		}
		else 
		{
			// printf("ENCOUNTERED Symbol :%c \n",r->Expression[i]);
			*temp3=singleNFA(r->Expression[i]);
			// printf("PRINTING TEMP3\n");
			//printNFA(temp3);
			NFAstack_push(process,*temp3);
		}
	}
	if(isNFAStackEmpty(process)) 
	{
		printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
		exit(1);
	}
	*a=NFAstack_top(process);
	//printf("\nA:::");
	//printNFA(a);
	//NFAstack_pop(process);
	printf("\nSTACK EMPTY::%d\n",isNFAStackEmpty(process));
	a->Attribute=SearchIAttribute(r->Attribute);
	return;
}
////////////////////////FUNCTIONS FOR SIMULATION OF NFA//////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
int simulateNFA(NFA* in,char* string,int lexemebegin)
{

}
int simulateAllNFA(NFA N[][maxnumbernfa],int numbernfa,char* string,int lexemebegin,int* rmaxmatch,int* rindexnfa) //RETURN LENGTH OF MATCH
{
	int maxmatch,indexnfa;
	int temp;
	int i;
	for(i=0;i<numbernfa;i++)
	{
		temp=simulateNFA(N[i],string,lexemebegin);
		if(temp>maxmatch)
		{
			maxmatch=temp;
			indexnfa=i;
		}
	}
	*rindexnfa=indexnfa;
	*rmaxmatch=maxmatch;
}

/////////////////////////////// TESTER //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void tester()
{
	int i;//RESERVED FOR LOOP
	NFA* N;//ARRAY OF NFAS
	N=(NFA*)malloc(sizeof(NFA));
	if(N==NULL)
	{
		printf("MALLOC:Memory allocation Failed\n");
		exit(1);
	}
	else
	{
		printf("MALLOC:Memory allocated successfully\n");
	}
	int numbernfa=0;
	int temp;
	RegExp r;
	const char* tempatt="TEST\0";
	//const char* temprexp="ab+c+d+e+f+";
	//const char* input="a+b+c";
	char input[maxregexp];
	char postfix[maxregexp];
	printf("\nEnter Regular Expression:\n");
	scanf("%s",input);
	infixToPostFix(input,postfix);
	printf("\nPostfix Expression is\n:%s\n",postfix);
	strcpy(r.Attribute,tempatt);
	strcpy(r.Expression,postfix);
	initalloc(N);
	RegExpToNFA(&r,strlen(r.Expression),N);
	printNFA(N);

	// char regexpline[maxregexp];
	// char inputbuffer[maxbufferlength];
	// int indexnfa,maxmatch;
	// int lexemebegin=0;
	// int lengthreg;
}
/////////////////////////////// MAIN ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
int main(int argc,char **argv)
{
	system("reset");
	//argv[1]:COMMAND LINE ARGUMENT FOR REGULAR EXPRESSIONS FILE
	// if(argv[1]==NULL||argv[2]==NULL||argv[3]==NULL)
	// {
	// 	printf("Wrong Usage Of the Command Line Try ./a.out REGEXPFILENAME INPUTFILE TOKENOUTPUTFILENAME \n");
	// 	exit(1);
	// }
	
	// FILE* regexpfile;
	// regexpfile=fopen(argv[1],"r");
	// FILE* inputfile;
	// inputfile=fopen(argv[2],"r");
	// FILE* tokenfile;
	// tokenfile=fopen(argv[3],"w");

	// if(regexpfile==NULL)
	// {
	// 	printf("Regular Expression File is Missing\n");
	// 	exit(1);
	// }
	// if(inputfile==NULL)
	// {
	// 	printf("Input File is Missing\n");
	// 	exit(1);
	// }

	// int i;//RESERVED FOR LOOP
	// NFA* N;//ARRAY OF NFAS
	// N=(NFA*)malloc(maxnumbernfa*sizeof(NFA));
	// if(N==NULL)
	// {
	// 	printf("MALLOC:Memory ALlocation Failed\n");
	// 	exit(1);
	// }
	// else
	// {
	// 	printf("MALLOC:Memory ALLOCATED\n");
	// }
	// int numbernfa=0;
	// int temp;
	// RegExp* r;
	// char regexpline[maxregexp];
	// char inputbuffer[maxbufferlength];
	// int indexnfa,maxmatch;
	// int lexemebegin=0;
	// int lengthreg;
	//INITIALIZE ALL NFA's
	//CONVERT REGULAR EXPRESSIONS TO NFA'S
	tester();

	// readfileinbuffer(inputfile,inputbuffer);
	// clearTabs(inputbuffer);
	// for(i=0;;i++)
	// {
	// 	temp=mreadLine(regexpfile,regexpline);
	// 	splitRegExp(regexpline,r);
	// 	initalloc(&N[i]);
	// 	RegExpToNFA(r,lengthreg,&N[i]);
	// 	numbernfa++;
	// 	if(temp) break; //END OF FILE BREAK REACHED
	// }
	// for(i=0;;i++)
	// {
	// 	if (inputbuffer[i]==EOF) break;
	// 	simulateAllNFA(&N,numbernfa,inputbuffer,lexemebegin,&maxmatch,&indexnfa);
	// 	printToken(tokenfile,generateToken(inputbuffer,maxmatch));
	// 	lexemebegin=lexemebegin+maxmatch;
	// }
	return 0;
}


/*************************************************************************************************************************/
/*************************************************************************************************************************/
/***********************************************U**NIMPLEMENTED***********************************************************/
int InitializeAllNFA(const char *regfile,int actualReg,NFA *N){
	// should return NFA Count....
	return 0;
}
int simulate_NFA(NFA *N,const char *string,int lexemebegin,int *newbegin,attrType *Attrib){

	// if accepted return ID
	// else return -1
	return 0;
}
int getNFAID(NFA *N){
	return N->Attribute;
}
/*************************************************************************************************************************/
/*************************************************************************************************************************/

#define MAXNFACOUNT 100

#define STRINGSIZE 1000
// #define FSMCOUNT 7
#define BUFFERSIZE 1000
#define MAXVARSIZE 100
// int ENDOFFILE=0;
void removeCarraigeReturn(char *line){
	int i=0;
	while((int)line[i]!=13){
		// printf("%c\n",line[i]);
		if(line[i]=='\0'){
			// printf("\n7777 End detected...\n");
			// scanf("%c",&b);
			return;
		}
		i++;
	}
	line[i]='\0';
	return;
}
int ENDOFFILE=0;
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

void resetfile(const char *x){
	FILE *f=fopen(x,"w");
	fclose(f);
	return;
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
			Code[i]=simulate_NFA(&N[i],line,lbegin,&End[i],&Attrib[i]);
			if(Code[i]==NOTOK){
				End[i]==-1;
			}
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
					addKeyword(&Attrib[bestmatch],symtab,outfile,Code[bestmatch]);
				}else{
					// treat lexeme as symbol
					add_Symbol(&Attrib[bestmatch],symtab,outfile,Code[bestmatch]);
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
