/////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
/////////////////////////////////////////////////////////////////////////////////////
#define maxstates 500
#define maxregexp 500
#define maxnumbernfa 100
#define maxbufferlength 1000
#define maxattributesize 30
#define maxregexpsize 500
#define checkendstateidx 257
#define EpsilonIdx 256
#define OSTAR 42
#define OADD 43
#define OPIPE 124
/////////////////////////////////////////////////////////////////////////////////////
int countmalloc=0;
int countfree=0; 
int countmallocTransition=0;
//int countmallocTransition
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
	int flag=0;
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			if(x==EOF){
				flag=1;
				line[i]='\0';
				if(i==0){
					return 1;
				}
				if(line[i-1]==13){
					line[i-1]='\0';
				}
				return 1;
			}
			line[i]='\0';
			if(i==0){
				return 0;
			}
			if(line[i-1]==13){
				line[i-1]='\0';
			}
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
	//free(s->head->n);
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
	//printf("\nENTERED FREE" );
	if(in==NULL) return;
	int i;
	int j;
	for(i=0;i<maxstates;i++)
	{
		for(j=0;j<258;j++)
		{
			if(in->table[i][j].head!=NULL) 
			{
				freeLink(in->table[i][j].head);
			}
		}
	}
	for(i=0;i<maxstates;i++)
	{
		free(in->table[i]);
		
	}
	free(in->table);
	
	free(in);
	
}
void freeNFANonptr(NFA* in)
{
	//printf("\nENTERED FREE" );
	if(in==NULL) return;
	int i;
	int j;
	for(i=0;i<maxstates;i++)
	{
		for(j=0;j<258;j++)
		{
			if(in->table[i][j].head!=NULL) 
			{
				freeLink(in->table[i][j].head);
			}
		}
	}
	for(i=0;i<maxstates;i++)
	{
		free(in->table[i]);
		
	}
	free(in->table);
	
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
void singleNFA(char ch,NFA* temp)//ASSUME temp3 allocated already
{
	//NFA out;
	//init(&out);
	temp->numberofstates=2;
	MakeEndState(temp,1);
	AddStateToTransition(temp,0,ch,1);
	//printf("\nPRINTING FROM singleNFA>>>>>>>>>.");
	//printNFA(&out);
	return;
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
void concatNFA(NFA* a,NFA* b,NFA* temp)//CHANGE MAY BE NEEDED !CLUB START STATE AND END STATE INSTEAD OF ADDING EPSILON TRANSITION
{
	//printf("\n***********************");
	CopyNFA(a,temp,0);
	//printf("\nAfter Copying A TO C\n");
	//printNFA(temp);
	CopyNFA(b,temp,temp->numberofstates-1);
	//printf("\nAfter Copying B TO C\n");
	//printNFA(temp);
	MakeEndState(temp,b->AcceptState+a->numberofstates-1);
	//AddStateToTransition(&c,a->AcceptState,EpsilonIdx,a->numberofstates);
	//printf("\nAfter concatNFA \n");
	//printNFA(&c);
	//freeNFA(a);
	//freeNFA(b);
	//printf("\n***********************");
	return;
}
void concatNFADirect(NFA* b,NFA* temp)//CHANGE MAY BE NEEDED !CLUB START STATE AND END STATE INSTEAD OF ADDING EPSILON TRANSITION
{
	CopyNFA(b,temp,temp->numberofstates-1);
	//printf("\nAfter Copying B TO C\n");
	//printNFA(temp);
	MakeEndState(temp,temp->numberofstates-1);
	//AddStateToTransition(&c,a->AcceptState,EpsilonIdx,a->numberofstates);
	//printf("\nAfter concatNFA \n");
	//printNFA(&c);
	//freeNFA(a);
	//freeNFA(b);
	//printf("\n***********************");
	return;
}
void starNFA(NFA* a,NFA* temp)
{
	temp->numberofstates=1;
	AddStateToTransition(temp,0,EpsilonIdx,1);
	AddStateToTransition(temp,0,EpsilonIdx,a->numberofstates+1);
	CopyNFA(a,temp,1);
	AddStateToTransition(temp,1+a->AcceptState,EpsilonIdx,1);
	AddStateToTransition(temp,1+a->AcceptState,EpsilonIdx,a->numberofstates+1);
	MakeEndState(temp,a->numberofstates+1);
	temp->numberofstates=temp->numberofstates+1;
	//freeNFA(a);
	return;
}
void unionNFA(NFA* a,NFA* b,NFA* temp)
{
	temp->numberofstates=1;
	AddStateToTransition(temp,0,EpsilonIdx,1);
	AddStateToTransition(temp,0,EpsilonIdx,a->numberofstates+1);
	//printf("\n********BEFORE COPY A TO U::\n");
	//printNFA(&u);
	CopyNFA(a,temp,1);
	//printf("\n**************COPY A TO U::\n");
	//printNFA(&u);
	CopyNFA(b,temp,a->numberofstates+1);
	temp->numberofstates=temp->numberofstates+1;
	AddStateToTransition(temp,a->AcceptState+1,EpsilonIdx,temp->numberofstates-1);
	AddStateToTransition(temp,b->AcceptState+a->numberofstates+1,EpsilonIdx,temp->numberofstates-1);
	MakeEndState(temp,temp->numberofstates-1);
	//printf("\nPRINTING UNION FROM FUNCTION\n");
	//printNFA(&u);
	//freeNFA(a);
	//freeNFA(b);
	return;
}
/////////////////////////////REGULAR EXPRESSION TO NFA///////////////////////////////
int getId(const char *id){
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
/////////////////////////////////////////////////////////////////////////////////////
int splitRegExp(char* line,RegExp* r)
{
	int i=0,j=0;
	if(strlen(line)==0) return 0;
	if(strlen(line)==1) return 0;
	for(;line[i]!=' ';i++)
	{
		r->Attribute[i]=line[i];
	}
	r->Attribute[i-j]='\0';
	while(line[i]==' '||line[i]=='\t')
	{
		i++;
	}
	
	//i++;
	j=i;	
	for(;line[i]!='\0';i++)
	{
		r->Expression[i-j]=line[i];
	}
	r->Expression[i-j]='\0';
	return 1;
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
	//for(i=0;i<numberofstates;i++)
	//{
		//printf("-%d-",inputStates[i]);
	//}
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
	//for(i=0;i<*numberofstatesClosure;i++)
	//{
		//printf("-%d-",Closure[i]);
	//}
	//printf("\nLEFT Epsilon Closure\n");
	free(remaining);
	
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
	printf("\nENTERED TYPE1");
	NFA temp1;
	NFA temp2;
	NFA* temp3;
	//init(a);
	// init(temp1);
	// init(temp2);
	// init(temp3);
	int i;
	char f1,f2;
	NFAstack* process;
	//init_NFAstack(process);
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
			//temp1=allocNFA();
			//init(temp1);
			temp3=allocNFA();
			init(temp3);
			temp1=NFAstack_top(process);
			NFAstack_pop(process);
			starNFA(&temp1,temp3);
			NFAstack_push(process,*temp3);
			freeNFANonptr(&temp1);
			//
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
			//temp1=allocNFA();
			//init(temp1);
			//temp2=allocNFA();
			//init(temp2);
			temp3=allocNFA();
			init(temp3);
			temp1=NFAstack_top(process);
			NFAstack_pop(process);	
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			temp2=NFAstack_top(process);
			NFAstack_pop(process);
			concatNFA(&temp2,&temp1,temp3);
			NFAstack_push(process,*temp3);	
			freeNFANonptr(&temp1);
			freeNFANonptr(&temp2);
		}
		else if(r->Expression[i]==OPIPE)
		{
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			temp3=allocNFA();
			init(temp3);
			temp1=NFAstack_top(process);
			NFAstack_pop(process);	
			if(isNFAStackEmpty(process)) 
			{
				printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
				exit(1);
			}
			temp2=NFAstack_top(process);
			NFAstack_pop(process);
			unionNFA(&temp1,&temp2,temp3);
			NFAstack_push(process,*temp3);		
			freeNFANonptr(&temp1);
			freeNFANonptr(&temp2);		
		}
		else 
		{
			temp3=allocNFA();
			init(temp3);
			singleNFA(r->Expression[i],temp3);
			NFAstack_push(process,*temp3);
		}
	}
	if(isNFAStackEmpty(process)) 
	{
		printf("\nERROR IN POSTFIX EXPRESSION\nPROGRAM WILL TERMINATE");
		exit(1);
	}
	*a=NFAstack_top(process);
	NFAstack_pop(process);
	free(process);
	return;
}
void RegExpToNFADirect(RegExp* r,int regexpsize,NFA* a)
{
	//printf("\nENTERED DIRECT ::");
	NFA* temp2;
	NFA* temp3;
	//init(a);
	temp3=allocNFA();
	init(temp3);
	int i;
	singleNFA(r->Expression[0],temp3);
	for(i=1;i<regexpsize;i++)
	{
		//printf("\nENTERED LOOP");
		temp2=allocNFA();
		init(temp2);
		singleNFA(r->Expression[i],temp2);
		concatNFADirect(temp2,temp3);
		freeNFA(temp2);
	}
	//printNFA(temp3);
	// copyTable(a,temp3);
	*a=*temp3;
	return;
}
////////////////////////FUNCTIONS FOR SIMULATION OF NFA//////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void simulateNFA(NFA* in,const char* string,int lexemebegin,int* newlexeme,int* maxmatch)
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
	//printf("\nActive STates Init:::%d\n",ActiveStatesSize );
	while(ActiveStatesSize!=0 && string[i]!='\0')
	{
	//printf("\nENCOUNTERED:::%c ActiveStates:::%d \n",string[i],ActiveStatesSize);
	move(in,ActiveStates,ActiveStatesSize,Move,&numberofstatesMove,(int)string[i]);
	EpsilonClosure(in,Move,numberofstatesMove,ActiveStates,&ActiveStatesSize);
	//printf("\nActive STates After Closure:::%d\n",ActiveStatesSize );
	if(SearchArray(ActiveStates,ActiveStatesSize,F))
	{
	if(*maxmatch<j)
	{
	*maxmatch=j+1;
	}
	}
	i++;
	j++;
	}
	*newlexeme=lexemebegin+*maxmatch+1;
	return;
}
int simulate_NFA(NFA *N,const char *string,int lexemebegin,int *newbegin,attrType *Attrib)
{
	int maxmatch=0;
	int i;
	simulateNFA(N,string,lexemebegin,newbegin,&maxmatch);
	for(i=lexemebegin;i<lexemebegin+maxmatch;i++)
	{
	Attrib->string[i-lexemebegin]=string[i];
	}
	Attrib->string[i-lexemebegin]='\0';
	if(maxmatch!=0)
	{
	// printf("\nACCEPTED");
	return N->Attribute;
	}
	else
	{
	// printf("\nREJECTED");
	return -1;
	}
	    // if accepted return ID
	    // else return -1
}
int InitializeAllNFA(const char *regfile,int actualreg,NFA *N)
{
	char line[400];
	int i;
	int flag=0;
	RegExp r;
	int count;
	char postfix[maxregexp];
	int type1=actualreg;
	FILE* regfp;
	regfp=fopen(regfile,"r");
	// for(i=0;i<maxnumbernfa;i++)
	// {
	// init(&N[i]);
	// }
	for(count=0;!flag;)
	{
	flag=mreadLine(regfp,line);
	//printf("LENGTH::%ld::EXPRESSION::%s\n",strlen(line), line);
	if(splitRegExp(line,&r))
	{
	printf("\nAttribute::%s::\nRegular Expression::%s\n ",r.Attribute,r.Expression);
	if(type1>0)
	{
	infixToPostFix(r.Expression,postfix);
	//printf("\nPostfix ::&%s&",postfix);
	strcpy(r.Expression,postfix);
	RegExpToNFA(&r,strlen(r.Expression),&N[count]);
	//printNFA(&N[count]);
	}
	else
	{
	//printf("STRLEN::%ld\n",strlen(r.Expression));
	RegExpToNFADirect(&r,strlen(r.Expression),&N[count]);

	}
	N[count].Attribute=getId(r.Attribute);
	// printNFA(&N[count]);
	printf("\nNFA FOR %d CREATED\n",N[count].Attribute);
	count++;
	type1--;
	}
	}
	printf("\n*****TOTAL NFAS CREATED::%d\n*******",count);
	    return count;
}
void FreeAllNFA(NFA* n)
{
	int i;
	for(i=0;i<maxnumbernfa;i++)
	{
		freeNFANonptr(&n[i]);
	}
}
/////////////////////////////// TESTER //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// MAIN ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/**************************DFA CODES************************************************************/
/**************************DFA CODES************************************************************/
/**************************DFA CODES************************************************************/
/**************************DFA CODES************************************************************/
/**************************DFA CODES************************************************************/

#define MAXDFASIZE 5000
#define MAXNFASIZE 1000
struct DFAnode{
	int a;
	struct DFAnode *next; 
};
typedef struct DFAnode DFAnode;
struct DFAState{
	int mark;
	DFAnode *head;
};
typedef struct DFAState DFAState;
struct DFA{
	// char nextState[MAXDFASIZE][258];
	int **nextState;
	DFAState states[MAXDFASIZE];
	int stateCount;
	int start;
	int acceptStates[MAXDFASIZE];
	int acceptCount;
	int id;
};
typedef struct DFA DFA;
void freeDFA(DFA *D){
	for(int i=0;i<MAXDFASIZE;i++){
		free(D->nextState[i]);
	}
	free(D->nextState);
	D->nextState=NULL;
	return;
}
/***********************************************U**NIMPLEMENTED***********************************************************/
// int simulate_NFA(NFA *N,const char *string,int lexemebegin,int *newbegin,attrType *Attrib){

// 	// if accepted return ID
// 	// else return -1
// 	return 0;
// }

/*************************************************************************************************************************/
int getNFAID(NFA *N){
	return N->Attribute;
	// return 0;
}
	int getDFAID(DFA *D){
		return (D->id);
	}
	void init_DFA(DFA *D){
		D->nextState=(int **)malloc(sizeof(int *)*MAXDFASIZE);
		if(D->nextState==NULL){
			printf("\nMemory Allocation failed in DFA !!\n");
			exit(-1);
		}
		// printf("\nIf")
		for(int i=0;i<MAXDFASIZE;i++){
			(D->nextState)[i]=(int *)malloc(sizeof(int)*258);
		}
		for(int i=0;i<MAXDFASIZE;i++){
			for(int j=0;j<258;j++){
				D->nextState[i][j]=-1;
			}
		}

		D->stateCount=-1;
		D->start=-1;
		D->acceptCount=-1;
		D->id=0;
		return;
	}
	void init_DFAState(DFAState *A){
		A->mark=0;
		A->head=NULL;
	}
	void AddSetElt(DFAState *A,int id){
		DFAnode *temp;
		temp=A->head;
		A->head=(DFAnode *)malloc(sizeof(DFAnode));
		A->head->a=id;
		A->head->next=temp;
		return;
	}
	int isSame_state(DFAState *A,int *set,int Scount){
		int Hash1[MAXNFASIZE],Hash2[MAXNFASIZE];
		for(int i=0;i<MAXNFASIZE;i++){
			Hash1[i]=0;
			Hash2[i]=0;
		}
		DFAnode *head;
		head=A->head;
		while(head!=NULL){
			Hash1[head->a]++;
			head=head->next;
		}
		for(int i=0;i<Scount;i++){
			Hash2[set[i]]++;
		}
		for(int i=0;i<MAXNFASIZE;i++){
			if(Hash1[i]!=Hash2[i]){
				return 0;
			}
		}
		return 1;
	}
	int AddUnmarkedState(DFA *D,int *T,int Tcount){
		if(D->stateCount==(MAXDFASIZE-1)){
			printf("\nDFA Exploded !!!\n");
			return -1;
		}
		(D->stateCount)++;// increment the state counter
		init_DFAState(&(D->states[D->stateCount]));// initialise the new state .. and also marked as unmarked
		for(int i=0;i<Tcount;i++){
			AddSetElt(&(D->states[D->stateCount]),T[i]);
		}
		return D->stateCount;
	}
	int isExist(DFA *D,int *set,int Scount){
		for(int i=0;i<=D->stateCount;i++){
			if(isSame_state(  &(D->states[i]),set,Scount  )==1   ){
				return i;
			}
		}
		return -1;
	}
	int setMarked(DFA *D,int state){
		if(state>(D->stateCount)){
			printf("\nNo Such state in Dfa");
			return 0;
		}
		D->states[state].mark=1;
		return 1;
	}
	int getUnmarkedState(DFA *D){
		for(int i=0;i<=D->stateCount;i++){
			if((D->states[i].mark)==0){
				return i;
			}
		}
		return -1;
	}
	int DFAStateToArray(DFA *D,int stateid,int *states,int *scount){
		if(stateid>(D->stateCount)){
			printf("\nGoing to access invalid state in DFA !!\n");
			return 0;
		}
		(*scount)=0;
		DFAnode *head=(D->states[stateid]).head;
		while(head!=NULL){
			states[(*scount)]=head->a;
			head=head->next;
			(*scount)++;
		}
		return 1;
	}
	int addTransition(DFA *D,int state,int symbol,int nextstate){
		if(state>(D->stateCount)||nextstate>(D->stateCount)){
			printf("\nInvalid transtion one or two state does not exisit !!\n");
			return -1;
		}
		if((D->nextState[state][symbol])!=-1){
			printf("\nGoing to Add multitple transition in DFA !!\n");
			return -1;
		}
		D->nextState[state][symbol]=nextstate;
		return 1;
	}
	int isAcceptState(DFA *D,int state){
		for(int i=0;i<=D->acceptCount;i++){
			if((D->acceptStates[i])==state){
				return 1;
			}
		}
		return 0;
	}
	int mark_Accept(DFA *D,int state){
		if(state>(D->stateCount)){
			printf("\nGoing to mark non exisiting state as accept state !!\n");
			exit(-1);
		}
		if(isAcceptState(D,state)==1){
			printf("\nState already marked as accept !!\n");
			exit(-1);
		}
		(D->acceptCount)++;
		(D->acceptStates)[D->acceptCount]=state;
		return 1;
	}
	int mark_AcceptStates(DFA *D,int acceptState){
		DFAnode *head;
		int States[MAXNFASIZE];
		int sCount;
		for(int i=0;i<=(D->stateCount);i++){
			DFAStateToArray(D,i,States,&sCount);
			for(int j=0;j<sCount;j++){
				if(States[j]==acceptState){
					mark_Accept(D,i);
					break;
				}
			}
		}
		return 1;
	}
	int printList(int *a,int sCount){
		printf("\n{");
		for(int i=0;i<sCount;i++){
			printf(" %d ",a[i]);
		}
		printf("}\n");
		return 0;
	}
	void NFATODFA(NFA *N,DFA *D){
		init_DFA(D);
		D->id=getNFAID(N);
		int EpsilonClosureSet[MAXNFASIZE];
		int CurrentSet[MAXNFASIZE];
		int MoveSet[MAXNFASIZE];
		int Mcount;
		int Ccount;
		int Ecount;
		CurrentSet[0]=0;
		Ccount=1;
		EpsilonClosure(N,CurrentSet,Ccount,EpsilonClosureSet,&Ecount);
		AddUnmarkedState(D,EpsilonClosureSet,Ecount);
		int unmarked;
		int nextstate;
		while(1){
			unmarked=getUnmarkedState(D);
			// printf("\nUnmarked state = %d\n",unmarked);
			if(unmarked==-1){
				// all the states are marked !!!
				break;
			}
			DFAStateToArray(D,unmarked,CurrentSet,&Ccount);
			// printf("\nDFA STATE TO ARRAY :");
			// printList(CurrentSet,Ccount);
			for(int i=0;i<=256;i++){
				move(N,CurrentSet,Ccount,MoveSet,&Mcount,i);
				// printf("\nhere\n");
				EpsilonClosure(N,MoveSet,Mcount,EpsilonClosureSet,&Ecount);
				nextstate=isExist(D,EpsilonClosureSet,Ecount);
				if(nextstate==-1){
					// have to insert new state into the DFA !!
					nextstate=AddUnmarkedState(D,EpsilonClosureSet,Ecount);
					// printf("\n******Adding New Unmarked(%d) state : for symbol = %c next = %d",unmarked,(char)i,nextstate);
					// printList(EpsilonClosureSet,Ecount);
				}else{
					// printf("\n******Transition Found from unmarked(%d) to %d for symbol %c\n",unmarked,nextstate ,(char)i);
				}
				addTransition(D,unmarked,i,nextstate);
			}
			setMarked(D,unmarked);
		}
		printf("\nAccept state = %d\n",N->AcceptState);
		mark_AcceptStates(D,N->AcceptState);
		printf("\nDFA Successfully created !!!\n");
		return;
	}
	int getNextState(DFA *D,int state,int symbol){
		if(state>(D->stateCount)){
			printf("\nRequested State does not exist in DFA !!\n");
			exit(-1);
		}
		return (D->nextState)[state][symbol];
	}



/****************************** High Level Interface *****************************************/
#define MAXDFACOUNT 100
#define MAXNFACOUNT 100

#define STRINGSIZE 1000
// #define FSMCOUNT 7TokenCreator
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
int isEmptyState(DFA *D,int state){

	if(state>D->stateCount){
		printf("\nGoing to check invalid state as EMPTY !!\n");
		exit(-1);
	}
	if((D->states[state]).head==NULL){
		return 1;
	}
	return 0;
}
int simulate_DFA(DFA *D,const char *string,int lbegin,int *newbegin,attrType *Attrib){
	int state=0;
	int nextstate;
	int begin=lbegin;
	while(1){
		if(string[lbegin]==' '||string[lbegin]=='\0'){
			// end of lexeme found
			break;
		}
		nextstate=getNextState(D,state,(int)string[lbegin]);
		if(isEmptyState(D,nextstate)==1){
			// we have gone to reject state ,,, so take it as a begining of next lexeme
			break;
		}
		if(nextstate==-1){
			printf("\nCant simulate anymore !!\n");
			// lexeme is not recognised further by this DFA!!
			break;
		}else{
			printf("\nsimulating !!\n");
		}
		state=nextstate;
		lbegin++;
	}
	int i=0;
	for(i=begin;i<=(lbegin-1);i++){
		Attrib->string[i-begin]=string[i];
	}
	Attrib->string[i-begin]='\0';
	(*newbegin)=lbegin;
	if(isAcceptState(D,state)==1){
		return getDFAID(D);
	}
	return NOTOK;
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
			if(Code[i]==-1){
				End[i]=-1;
			}
		}
		match1=-1;
		match2=-1;
		getBestMatch(End,nfaCount,&match1,&match2);
		if(match2!=-1&&match1!=-1){
			printf("\nMatch1 = %d TYPE1 = %d\tMatch2 = %d TYPE2 = %d\n",match1,getNFAID(&N[match1]),
			match2,getNFAID(&N[match2]));
			printf("\nLEXEME1 = {%s}\tLEXEME2 = {%s}\n",Attrib[match1].string,Attrib[match2].string);
		}
		if(match2==-1&&match1!=-1){
			printf("\nMatch1 = %d TYPE1 = %d\n",match1,getNFAID(&N[match1]));
			printf("\nLEXEME1 = {%s}\n",Attrib[match1].string);
		}
		if(match1==-1&&match2==-1){
			printf("\nERROR \n");
			exit(-1);
		}
		if(match2==-1){
			bestmatch=match1;
		}else /*if(match1==match2)*/{
			if(getNFAID(&N[match1])==IDNTIFIER){
				bestmatch=match2;
				printf("\nMatch 2 is best");
			}else{
				bestmatch=match1;
				printf("\nmatch 1 is best ");
			}
		}
		// }else{
		// 	printf("\nFatal error in matching !!\n");
		// 	exit(-1);
		// }
		lbegin=End[bestmatch];
		printf("\nLEXEME = {%s}\tTYPE = %d",Attrib[bestmatch].string,getNFAID(&N[bestmatch]));
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

	for(int i=0;i<MAXDFACOUNT;i++){
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
void compileWithNFA(const char *source,const char *symtab,const char *outfile){
	NFA N[100];
	int nfaCount=InitializeAllNFA("regexp.in",3,N);
	printf("\nTotal %d NFA created\n",nfaCount);
	TokenCreator(source,symtab,outfile,N,nfaCount);
}
// void printDFA(DFA *D){
// }


/*********************************************************************************************************/
/********************* add support for empty set *******************************/
/**************************DFA CODES************************************************************/
/**************************DFA CODES************************************************************/
/**************************DFA CODES************************************************************/
/**************************DFA CODES************************************************************/
/**************************DFA CODES************************************************************/
void tester()
{
	int i;//RESERVED FOR LOOP
	NFA N;
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
	//init(N);
	//initalloc(N);
	RegExpToNFA(&r,strlen(r.Expression),&N);
	printNFA(&N);
	
	printf("\nEnter String to check for maximum match\n");
	char string[200];
	scanf("%s",string);

	int lexemebegin=0;
	int newlexeme;
	int maxmatch;
	simulateNFA(&N,string,lexemebegin,&newlexeme,&maxmatch);
	printf("\nMAXIMUM PART ACCEPTED\n");
	for(i=lexemebegin;i<lexemebegin+maxmatch+1;i++)
	{
		printf("%c",string[i]);
	}
	printf("\n\n");

	DFA D;
	init_DFA(&D);
	NFATODFA(&N,&D);
	int id;
	while(1){
		printf("\nEnter String : \n");
		char lexeme[100];
		scanf("%s",lexeme);
		if(lexeme[0]=='-'){
			break;
		}
		int newbegin;
		attrType Attrib;
		Attrib.string=(char *)malloc(sizeof(char)*1000);
		// int simulate_DFA(DFA *D,const char *string,int lbegin,int *newbegin,attrType *Attrib)
		id=simulate_DFA(&D,lexeme,0,&newbegin,&Attrib);
		printf("\nID = %d\n",id );
		printf("\nTOKEN = {%s}\n",Attrib.string);
	}
	freeDFA(&D);
	freeNFANonptr(&N);
}
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
		// tester();

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
	const char *SYMBOLTABLE = "Symboltable.out";
	const char *OUTFILE="outfile.out";
	const char *SOURCE="input.c";
	initialize_All(SYMBOLTABLE,OUTFILE);
	compileWithNFA(SOURCE,SYMBOLTABLE,OUTFILE);
	return 0;
}



/*************************************************************************************************************************/
/*************************************************************************************************************************/
/***********************************************U**NIMPLEMENTED***********************************************************/
// int InitializeAllNFA(const char *regfile,int actualReg,NFA *N){
// 	// should return NFA Count....
// 	return 0;
// }
// int simulate_NFA(NFA *N,const char *string,int lexemebegin,int *newbegin,attrType *Attrib){

// 	// if accepted return ID
// 	// else return -1
// 	return 0;
// }
// int getNFAID(NFA *N){
// 	return N->Attribute;
// }
/*************************************************************************************************************************/
/*************************************************************************************************************************/

// #define MAXNFACOUNT 100

// #define STRINGSIZE 1000
// // #define FSMCOUNT 7
// #define BUFFERSIZE 1000
// #define MAXVARSIZE 100
// // int ENDOFFILE=0;
// // void removeCarraigeReturn(char *line){
// // 	int i=0;
// // 	while((int)line[i]!=13){
// // 		// printf("%c\n",line[i]);
// // 		if(line[i]=='\0'){
// // 			// printf("\n7777 End detected...\n");
// // 			// scanf("%c",&b);
// // 			return;
// // 		}
// // 		i++;
// // 	}
// // 	line[i]='\0';
// // 	return;
// // }
// int ENDOFFILE=0;
// void readLine(FILE *fp,char *line){
// 	// memory for line must be already allocated
// 	char x;
// 	int i=0;
// 	x=getc(fp);
// 	while(1){
// 		if(x==EOF||x=='\n'){
// 			if(x==EOF){
// 				ENDOFFILE=1;
// 			}
// 			line[i]='\0';
// 			if((int)line[i-1]==13){
// 				line[i-1]='\0';// taking care of carriage return....os sensitive
// 			}
// 			return;
// 			// break;
// 		}else{
// 			line[i]=x;
// 		}
// 		x=getc(fp);
// 		i++;
// 	}
// 	// printf("Carriage return call\n");
// 	// removeCarraigeReturn(line);
// 	// printf("Carriage return end\n");
// 	return;
// }

// void resetfile(const char *x){
// 	FILE *f=fopen(x,"w");
// 	fclose(f);
// 	return;
// }

// void initialize_symtab(const char *filename){
// 	// short sizeof int float double bool char signed unsigned for while do
// 	// return struct const void switch break case break goto long static union
// 	// default
// 	FILE *fp=fopen(filename,"w");
// 	fprintf(fp, "short 0\n");
// 	fprintf(fp, "sizeof 0\n");
// 	fprintf(fp, "int 0\n");
// 	fprintf(fp, "float 0\n");
// 	fprintf(fp, "double 0\n");
// 	fprintf(fp, "bool 0\n");
// 	fprintf(fp, "char 0\n");
// 	fprintf(fp, "signed 0\n");
// 	fprintf(fp, "unsigned 0\n");
// 	fprintf(fp, "for 0\n");
// 	fprintf(fp, "while 0\n");
// 	fprintf(fp, "do 0\n");
// 	fprintf(fp, "return 0\n");
// 	fprintf(fp, "struct 0\n");
// 	fprintf(fp, "const 0\n");
// 	fprintf(fp, "void 0\n");
// 	fprintf(fp, "switch 0\n");
// 	fprintf(fp, "break 0\n");
// 	fprintf(fp, "case 0\n");
// 	fprintf(fp, "continue 0\n");
// 	fprintf(fp, "goto 0\n");
// 	fprintf(fp, "long 0\n");
// 	fprintf(fp, "static 0\n");
// 	fprintf(fp, "union 0\n");
// 	fprintf(fp, "default 0\n");
// 	fclose(fp);
// 	return;
// }
// void getBestMatch(const int *end,int nfaCount,int *match1,int *match2){
// 	(*match2)=-1;
// 	(*match1)=0;
// 	int match1len,match2len;
// 	match1len=end[0];
// 	for(int i=1;i<nfaCount;i++){
// 		if(end[i]>match1len){
// 			match2len=match1len;
// 			(*match2)=(*match1);
// 			match1len=end[i];
// 			(*match1)=i;
// 			continue;
// 		}
// 		if(end[i]==match1len){
// 			match2len=end[i];
// 			(*match2)=i;
// 			continue;
// 		}
// 	}
// 	return;
// }
// int SYMTAB_END=0;
// void read_symtab(FILE *fp,char *line){
// 	// memory for line must be already allocated
// 	// printf("here...in read symtab\n");
// 	char x;
// 	int i=0;
// 	x=getc(fp);
// 	while(1){
// 		if(x==EOF||x=='\n'){
// 			if(x==EOF){
// 				SYMTAB_END=1;
// 			}
// 			line[i]='\0';
// 			return;
// 		}else{
// 			line[i]=x;
// 		}
// 		x=getc(fp);
// 		i++;
// 	}
// }
// int isSame(const char *a,const char *b){
// 	// printf("here..\n");
// 	while(1){
// 		if((*a)!=(*b)){
// 			return 0;
// 		}
// 		if((*a)=='\0'){
// 			return 1;
// 		}
// 		// printf("a=%c b=%c\n",*a,*b );
// 		a++;
// 		b++;
// 	}
// }
// void writeline(const char *filename,const char *line){
// 	// printf("\n*************APPENDING : '%s'  ****\n",line);
// 	FILE *f=fopen(filename,"a");
// 	fprintf(f, "%s\n",line);
// 	fclose(f);
// 	return;
// }
// int search_symtab(const char *symtab,const char *query){
// 	SYMTAB_END=0;
// 	char symbol[MAXVARSIZE],var[MAXVARSIZE],marker;
// 	FILE *fp=fopen(symtab,"r");
// 	int i;
// 	while(1){
// 		read_symtab(fp,symbol);
// 		// printf("%s\n",symbol );
// 		if(symbol[0]=='\0'&&SYMTAB_END!=1){
// 			continue;
// 		}
// 		if(symbol[0]=='\0'){
// 			break;
// 		}
// 		i=0;
// 		while(symbol[i]!=' '){
// 			var[i]=symbol[i];
// 			i++;
// 		}
// 		var[i]='\0';
// 		// printf("%s\n",var );
// 		i++;
// 		marker=symbol[i];
// 		// printf("%c\n",marker );
// 		if(isSame(var,query)==1){
// 			printf("\nkeyword detected ...\n");
// 			fclose(fp);
// 			return ((int)marker-48);		
// 		}
// 		if(SYMTAB_END==1){
// 			break;
// 		}
// 	}
// 	printf("\nIts a keyword");
// 	fclose(fp);
// 	return -1;
// }
// int getKeywordId(const char *id){
// 	if(isSame(id,"SHORT")){
// 		return SHORT;
// 	}
// 	if(isSame(id,"INT")){
// 		return INT;
// 	}
// 	if(isSame(id,"FLOAT")){
// 		return FLOAT;
// 	}
// 	if(isSame(id,"DOUBLE")){
// 		return DOUBLE;
// 	}
// 	if(isSame(id,"BOOL")){
// 		return BOOL;
// 	}
// 	if(isSame(id,"CHAR")){
// 		return CHAR;
// 	}
// 	if(isSame(id,"SIGNED")){
// 		return SIGNED;
// 	}
// 	if(isSame(id,"UNSIGNED")){
// 		return UNSIGNED;
// 	}
// 	if(isSame(id,"FOR")){
// 		return FOR;
// 	}
// 	if(isSame(id,"WHILE")){
// 		return WHILE;
// 	}
// 	if(isSame(id,"DO")){
// 		return DO;
// 	}
// 	if(isSame(id,"RETURN")){
// 		return RETURN;
// 	}
// 	if(isSame(id,"STRUCT")){
// 		return STRUCT;
// 	}
// 	if(isSame(id,"CONST")){
// 		return CONST;
// 	}
// 	if(isSame(id,"VOID")){
// 		return VOID;
// 	}
// 	if(isSame(id,"SWITCH")){
// 		return SWITCH;
// 	}
// 	if(isSame(id,"BREAK")){
// 		return BREAK;
// 	}
// 	if(isSame(id,"CASE")){
// 		return CASE;
// 	}
// 	if(isSame(id,"CONTINUE")){
// 		return CONTINUE;
// 	}
// 	if(isSame(id,"GOTO")){
// 		return GOTO;
// 	}
// 	if(isSame(id,"LONG")){
// 		return LONG;
// 	}
// 	if(isSame(id,"STATIC")){
// 		return STATIC;
// 	}
// 	if(isSame(id,"UNION")){
// 		return UNION;
// 	}
// 	if(isSame(id,"DEFAULT")){
// 		return DEFAULT;
// 	}
// }
// int insert_symtab(const char *symtab,const char *id){
// 	// if string is not present already then it adds that to symtab and returns Identifier
// 	// if string is a keyword then its respective id is returned 
// 	char line[100];
// 	int result=search_symtab(symtab,id);
// 	printf("\nInserting '%s' is symtab\n",id);
// 	if(result==-1){
// 		// the token is an identifier and not available in symtab
// 		sprintf(line,"%s 1",id);
// 		printf("SYMTAB >> '%s'\n",line );
// 		writeline(symtab,line);
// 		return IDNTIFIER;
// 	}
// 	if(result==0){
// 		return getKeywordId(id);
// 	}
// }
// void addIdentifier_KeyWord(attrType *Attrib,const char *symtab,const char *outfile){
// 	printf("\ngoing Inserting '%s' is symtab\n",Attrib->string);
// 	int result=insert_symtab(symtab,Attrib->string);

// 	char token[100];
// 	if(result==IDNTIFIER){
// 		// means inserted it was a identifier
// 		sprintf(token,"%d %s",IDNTIFIER,Attrib->string);
// 		printf("outfile >> %s",token);
// 	}else{
// 		// its a key word
// 		sprintf(token,"%d %s",result,Attrib->string);
// 	}
// 	writeline(outfile,token);
// 	return;
// }
// void addIdentifier(attrType *Attrib,const char *symtab,const char *outfile){
// 	printf("\ngoing Inserting '%s' is symtab\n",Attrib->string);
// 	int result=insert_symtab(symtab,Attrib->string);
// 	char token[100];
// 	sprintf(token,"%d %s",IDNTIFIER,Attrib->string);
// 	printf("outfile >> %s",token);
// 	writeline(outfile,token);
// 	return;
// }
// void addKeyword(attrType *Attrib,const char *symtab,const char *outfile,int code){
// 	// printf("\ngoing Inserting '%s' is symtab\n",Attrib->string);
// 	char token[100];
// 	sprintf(token,"%d %s",code,Attrib->string);
// 	writeline(outfile,token);
// 	return;
// }
// void add_Symbol(attrType *Attrib,const char *symtab,const char *outfile,int code){
// 	char token[100];
// 	sprintf(token,"%d",code);
// 	writeline(outfile,token);
// 	return;
// }
// void add_Numerical(attrType *Attrib,const char *symtab,const char *outfile,int code){
// 	char token[100];
// 	sprintf(token,"%d %s",code,Attrib->string);
// 	printf("\nNumerical Token {%s}\n",token);
// 	writeline(outfile,token);
// 	return;
// }
// int lineToTokens(const char *line,const char *symtab,const char *outfile,NFA *N,int nfaCount){
// 	int lbegin=0;
// 	int bestmatch;
// 	int match1,match2;
// 	int Code[MAXNFACOUNT];
// 	int End[MAXNFACOUNT];
// 	attrType Attrib[MAXNFACOUNT];
// 	for(int i=0;i<MAXNFACOUNT;i++){
// 		Attrib[i].string=(char *)malloc(sizeof(char)*100);
// 	}
// 	while(1){
// 		for(int i=0;i<nfaCount;i++){
// 			Code[i]=simulate_NFA(&N[i],line,lbegin,&End[i],&Attrib[i]);
// 			if(Code[i]==NOTOK){
// 				End[i]==-1;
// 			}
// 		}
// 		getBestMatch(End,nfaCount,&match1,&match2);
// 		printf("\nMatch1 = %d\tMatch2 = %d",match1,match2);
// 		if(match2==-1){
// 			bestmatch=match1;
// 		}else if(match1==match2){
// 			if(getNFAID(&N[match1])==IDNTIFIER){
// 				bestmatch=match2;
// 			}else{
// 				bestmatch=match1;
// 			}
// 		}else{
// 			printf("\nFatal error in matching !!\n");
// 			exit(-1);
// 		}
// 		lbegin=End[bestmatch];
// 		switch(Code[bestmatch]){
// 			case IDNTIFIER:{
// 				addIdentifier(&Attrib[bestmatch],symtab,outfile);
// 				break;
// 			}
// 			case INT_CONST:{
// 				add_Numerical(&Attrib[bestmatch],symtab,outfile,INT_CONST);
// 				break;	
// 			}
// 			case FLO_CONST:{
// 				add_Numerical(&Attrib[bestmatch],symtab,outfile,FLO_CONST);
// 				break;
// 			}
// 			case NOTOK:{
// 				printf("\nLexeme not detected in any of the NFA !!!\n");
// 				exit(-1);
// 				// break;
// 			}
// 			default:{
// 				if((Code[bestmatch]>=422)&&(Code[bestmatch]<=445)){
// 					// treat lexeme as keyword
// 					addKeyword(&Attrib[bestmatch],symtab,outfile,Code[bestmatch]);
// 				}else{
// 					// treat lexeme as symbol
// 					add_Symbol(&Attrib[bestmatch],symtab,outfile,Code[bestmatch]);
// 				}
// 			}
// 		}
// 		lbegin=End[bestmatch];
// 		if(line[lbegin]=='\0'){
// 			// we have reached the End of the current line
// 			break;
// 		}
// 		if(line[lbegin]==' '){
// 			lbegin++;
// 			continue;
// 		}
// 	}

// 	for(int i=0;i<MAXNFACOUNT;i++){
// 		free(Attrib[i].string);
// 		Attrib[i].string=NULL;
// 	}
// }
// // void printChar(const char *line){
// // 	printf("\nSTART\n");
// // 	while(*line!='\0'){
// // 		printf("%c ASCII = %d$\n",*line,(int)(*line) );
// // 		line++;
// // 	}
// // 	printf("END\n");
// // 	return;
// // }
// void TokenCreator(const char *source,const char *symtab,const char *outfile,NFA *N,int nfaCount){
// 	printf("\nFILENAME = '%s'\n",source);
// 	char *line=(char *)malloc(sizeof(char)*1000);
// 	char *debug=(char *)malloc(sizeof(char)*1000);
// 	// char debug[1000];
// 	FILE *s=fopen(source,"r");
// 	ENDOFFILE=0;
// 	while(1){
// 		line[0]='\0';
// 		readLine(s,line);
// 		// printChar(line);
// 		// printf("%s",line);
// 		// printf("\n **LINE = { %s }*** \n",line);
// 		printf("clearing tabs\n");
// 		clearTabs(line);
// 		printf("tabs cleared\n");
// 		sprintf(debug,"\n**********LINE = [%s]**************\n",line);
// 		printf("%s",debug);
// 		// printChar(debug);
// 		// printf("\n%s\n",line );
// 		if(line[0]=='\0'&&ENDOFFILE==1){
// 			break;
// 		}
// 		if(line[0]=='\0'){
// 			continue;
// 		}
// 		lineToTokens(line,symtab,outfile,N,nfaCount);
// 		if(ENDOFFILE==1){
// 			break;
// 		}
// 	}
// 	fclose(s);
// }
// void initialize_All(const char *symtab,const char *outfile){
// 	initialize_symtab(symtab);
// 	resetfile(outfile);
// 	ENDOFFILE=0;
// 	// for(int i=0;i<FSMCOUNT;i++){
// 	// 	Begin[i]=0;
// 	// 	End[i]=0;
// 	// 	Code[i]=0;
// 	// }
// 	// Attrib[0].string=(char *)malloc(sizeof(char)*100);
// 	// Attrib[1].string=(char *)malloc(sizeof(char)*100);
// 	// Attrib[2].string=(char *)malloc(sizeof(char)*100);
// 	// // Attrib[3].string=(char *)malloc(sizeof(char)*100);
// 	// Attrib[5].string=(char *)malloc(sizeof(char)*100);
// 	// Attrib[6].string=(char *)malloc(sizeof(char)*100);
// 	// // Attrib[6].string=(char *)malloc(sizeof(char)*100);
// 	// // Attrib[0].string=(char *)malloc(sizeof(char)*100);
// 	return;
// }
