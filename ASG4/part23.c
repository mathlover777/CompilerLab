/////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
/////////////////////////////////////////////////////////////////////////////////////
#define maxstates 500
#define maxregexp 200
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
	//Transition table[maxstates][258];
	Transition** table;
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


int malloc1=0,malloc2=0,malloc3=0;
int free1=0,free2=0,free3=0;
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
int ENDOFFILE=0;
int mreadLine(FILE *fp,char *line){
	// memory for line must be already allocated
	ENDOFFILE=0;
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			if(x==EOF){
				ENDOFFILE=1;
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
	if(s->head==NULL)
	{
		printf("\nMALLOC FAILED stack_push!!!");
		exit(1);
	}
	s->head->c=x;
	s->head->next=temp;
	return;
}
void enqueue(queue *q,char x){
	if(q->tail==NULL){
		q->head=(scharnode *)malloc(sizeof(scharnode));
			if(q->head==NULL)
	{
		printf("\nMALLOC FAILED!!!");
		exit(1);
	}
		q->head->c=x;
		q->head->next=NULL;
		q->tail=q->head;
		return;
	}
	q->tail->next=(scharnode *)malloc(sizeof(scharnode));
		if(q->tail->next==NULL)
	{
		printf("\nMALLOC FAILED!!!");
		exit(1);
	}
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
		if(s->head==NULL)
	{
		printf("\nMALLOC FAILED!!!");
		exit(1);
	}
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
	if(s->head==NULL)
	{
		printf("\nMALLOC FAILED NFAstack_push!!!");
		exit(1);
	}
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
	if(n==NULL)
	{
		printf("\nMALLOC FAILED allocNFA!!!");
		exit(1);
	}
	return n;
}
void init(NFA* n)
{
	int i,j,k;
	n->numberofstates=0;
	n->Attribute=0;
	n->AcceptState=0;
	n->table=(Transition**)malloc(maxstates*sizeof(Transition*));
	for(i=0;i<maxstates;i++)
	{
		n->table[i]=(Transition*)malloc(258*sizeof(Transition));
	}
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
void freeLink(node* n)
{
	node* temp=NULL;
	while(n!=NULL)
	{
		temp=n->next;
		free(n);
		n=temp;
	}
}
void freeNFA(NFA* in)
{
	if(in==NULL) return;
	int i;
	int j;
	for(i=0;i<in->numberofstates;i++)
	{
		for(j=0;j<258;j++)
		{
			if(in->table[i][j].head!=NULL) 
			{
				freeLink(in->table[i][j].head);
			}
		}
	}
	for(i=0;i<in->numberofstates;i++)
	{
		free(in->table[i]);
	}
	free(in->table);
	free(in);
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
	if(temp==NULL)
	{
		printf("\nMALLOC FAILED!!!");
		exit(1);
	}
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
	//freeNFA(a);
	//freeNFA(b);
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
	//freeNFA(a);
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
	//freeNFA(a);
	//freeNFA(b);
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
	//printf("\nENTERED EPSILON Closure\n");
	//printf("\nInput State::\n");
	for(i=0;i<numberofstates;i++)
	{
		//printf("-%d-",inputStates[i]);
	}
	//printf("\n*********************************\n");
	if(numberofstates==0)
	{
	    ////printf("\nLEFT Epsilon Closure\n");
		return;
	}
	node* dummy;
	intstack* remaining;
	int t;
	remaining=(intstack*)malloc(sizeof(intstack));
	if(remaining==NULL)
	{
		printf("\nMALLOC FAILED!!!");
		exit(1);
	}
	init_intstack(remaining);
	for(i=0;i<numberofstates;i++)
	{
		intstack_push(remaining,inputStates[i]);
	}
	while(!isintStackEmpty(remaining))
	{
		////printf("\n STACK NOT EMPTY");
		t=intstack_top(remaining);
		intstack_pop(remaining);
		////printf("TOP IS %d\n",t);
		dummy=n->table[t][EpsilonIdx].head;
		while(dummy!=NULL)
		{
			//printf("\n ADD THIS:: %d",dummy->nstate);
			if(!SearchArray(Closure,*numberofstatesClosure,dummy->nstate))
			{

				AddStateToArray(Closure,numberofstatesClosure,dummy->nstate);
				//printf("\nPUSHED STACK:: %d",dummy->nstate);
				intstack_push(remaining,dummy->nstate);
			}
			dummy=dummy->next;
		}
		
	}
	for(i=0;i<numberofstates;i++)
	{
			if(!SearchArray(Closure,*numberofstatesClosure,inputStates[i]))
			{
				AddStateToArray(Closure,numberofstatesClosure,inputStates[i]);
			}
	}
	//printf("\nPRINTING Closure\n");
	//printf("\n NUMBER OF STATES CLOSURE:%d\n",*numberofstatesClosure);
	for(i=0;i<*numberofstatesClosure;i++)
	{
		//printf("-%d-",Closure[i]);
	}
	//printf("\nLEFT Epsilon Closure\n");
	return;
}
void move(NFA* n,int* inputStates,int numberofstates,int* Move,int* numberofstatesMove,int ch)//Move ALREADY ALLOCATED
{
	//printf("\nENTERED MOVE CHARACTER:INPUT:%c\n",ch);
	int i;
	if(numberofstates==0) 
	{
		*numberofstatesMove=0;
		return;
	}
	//printf("\n numberofstates input:%d\n ",numberofstates);
	for(i=0;i<numberofstates;i++)
	{
		//printf("-%d-",inputStates[i]);
	}
	//printf("\n***********\n");
	node* dummy;
	*numberofstatesMove=0;
	for(i=0;i<numberofstates;i++)
	{
		dummy=n->table[inputStates[i]][ch].head;
		while(dummy!=NULL)
		{
			if(!SearchArray(Move,*numberofstatesMove,dummy->nstate))
			{
				AddStateToArray(Move,numberofstatesMove,dummy->nstate);
			}
			dummy=dummy->next;
		}
	}
	//printf("\n numberofstates:%d\n ",*numberofstatesMove);
	for(i=0;i<*numberofstatesMove;i++)
	{
		//printf("-%d-",Move[i]);
	}
	//printf("\nLEFT MOVE\n");
	return;
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
	if(process==NULL)
	{
		printf("\nMALLOC FAILED!!!");
		exit(1);
	}
	init_NFAstack(process);
	for(i=0;i<regexpsize;i++)
	{
		////printf("PRINTING INIT TEMP3\n");
		//printNFA(temp3);
		if(r->Expression[i]==OSTAR)
		{
			// printf("ENCOUNTERED *\n");
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			temp1=allocNFA();
			init(temp1);
			temp3=allocNFA();
			init(temp3);
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
			temp1=allocNFA();
			init(temp1);
			temp2=allocNFA();
			init(temp2);
			temp3=allocNFA();
			init(temp3);
			*temp1=NFAstack_top(process);
			NFAstack_pop(process);	
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			*temp2=NFAstack_top(process);
			NFAstack_pop(process);
			*temp3=concatNFA(temp2,temp1);
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
			temp1=allocNFA();
			init(temp1);
			temp2=allocNFA();
			init(temp2);
			temp3=allocNFA();
			init(temp3);
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
			temp3=allocNFA();
			init(temp3);
			// printf("ENCOUNTERED Symbol :%c \n",r->Expression[i]);
			*temp3=singleNFA(r->Expression[i]);
			// printf("PRINTING TEMP3\n");
			//printNFA(temp3);
			NFAstack_push(process,*temp3);
			//freeNFA(temp3);		
		}
		//freeNFA(temp1);
		//freeNFA(temp2);
		//freeNFA(temp3);	
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
void simulateNFA(NFA* in,char* string,int lexemebegin,int* newlexeme,int* maxmatch)
{
	*maxmatch=0;
	int i=lexemebegin;
	int j=0;
	int ActiveStates[maxstates];
	int ActiveStatesSize=1;
	int TempStates[maxstates];
	int TempStatesSize=1;
	int Move[maxstates];
	int numberofstatesMove=0;
	TempStates[0]=0;
	int F=in->AcceptState;
	*maxmatch=0;
	//printf("\nENTERED simulateNFA");
	EpsilonClosure(in,TempStates,TempStatesSize,ActiveStates,&ActiveStatesSize);
	printf("\nActive STates Init:::%d\n",ActiveStatesSize );
	while(ActiveStatesSize!=0 && string[i]!='\0')
	{
		//printf("\nENCOUNTERED:::%c ActiveStates:::%d \n",string[i],ActiveStatesSize);
		move(in,ActiveStates,ActiveStatesSize,Move,&numberofstatesMove,(int)string[i]);
		EpsilonClosure(in,Move,numberofstatesMove,ActiveStates,&ActiveStatesSize);
		printf("\nActive STates After Closure:::%d\n",ActiveStatesSize );
		if(SearchArray(ActiveStates,ActiveStatesSize,F))
		{
			if(*maxmatch<j) *maxmatch=j;
		}
		i++;
		j++;
	}
	*newlexeme=lexemebegin+*maxmatch+1;
	return;
}
int simulateAllNFA(NFA N[][maxnumbernfa],int numbernfa,char* string,int lexemebegin,int* rmaxmatch,int* rindexnfa) //RETURN LENGTH OF MATCH
{
	int match,maxmatch,indexnfa,newlexeme;
	int i;
	for(i=0;i<numbernfa;i++)
	{
		simulateNFA(N[i],string,lexemebegin,&newlexeme,&match);
		if(match>maxmatch)
		{
			maxmatch=match;
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
		printf("\nMALLOC FAILED!!!");
		exit(1);
	}
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
	init(N);
	//initalloc(N);
	RegExpToNFA(&r,strlen(r.Expression),N);
	printNFA(N);
	printf("\nEnter String to check for maximum match\n");
	char string[200];
	scanf("%s",string);
	// int lexemebegin=0;
	// int newlexeme;
	// int maxmatch;
	//simulateNFA(N,string,lexemebegin,&newlexeme,&maxmatch);
	// printf("\nMAXIMUM PART ACCEPTED\n");
	// for(i=lexemebegin;i<lexemebegin+maxmatch+1;i++)
	// {
	// 	printf("%c",string[i]);
	// }
	// printf("\n\n");
	freeNFA(N);
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
	// system("reset");
	//NFA N;
	tester();
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