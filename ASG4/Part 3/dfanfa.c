#ifndef DFANFA
#define DFANFA
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

int replaceSq(char *a){
	// ...[...[]...]... is not allowed ,,, please use explicit union :p   
	char b[1000];
	int i=0,j=0;
	int inSq=0;
	int error=0;
	int pcount=0;
	while(1){
		if(a[i]=='\0'){
			b[j]='\0';
			break;
		}
		if(inSq==0){
			if(a[i]=='['){
				b[j]=a[i];
				j++;
				i++;
				inSq=1;
				pcount=0;
				continue;
			}else if(a[i]==']'){
				error=1;
				break;
			}else{
				b[j]=a[i];
				j++;
				i++;
				continue;
			}
		}else{
			// we are in SQ cage
			switch(a[i]){
				case '(':{
					b[j]=a[i];
					j++;
					i++;
					pcount++;
					break;
				}case ')':{
					b[j]=a[i];
					j++;
					i++;
					pcount--;
					if((pcount==0)&&(a[i]!=']')){
						b[j]='|';
						j++;
					}
					break;
				}
				case '[':{
					error=1;
					printf("\nRecursive Square Braces are not allowed");
					break;
				}
				case ']':{
					inSq=0;
					b[j]=a[i];
					j++;
					i++;
					break;
				}
				default:{
					if(pcount==0){
						// we are at the outer level ... so need to add union after charecter
						b[j]=a[i];
						j++;
						i++;
						b[j]='|';
						j++;
					}else{
						// we are in between level
						b[j]=a[i];
						j++;
						i++;
					}
				}
			}
		}
	}
	if(inSq!=0){
		printf("\nUnbalanced Sq braces in the RegExp");
		exit(-1);
	}
	if(error==1){
		return 0;
	}
	i=0;
	while(b[i]!='\0'){
		a[i]=b[i];
		i++;
	}
	a[i]=b[i];
	return 1;
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

	replaceRange(a,c);
	replaceSq(c);
	replaceBraces(c);
	//printf("\nAfter changing braces :\n{%s}\n",a);
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
    if(isSame(id,"MULT")==1){
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
clearTabs(line);
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
for(;line[i]!='\0'&&line[i]!=' ';i++)
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
	printf("\nENTERED simulateNFA");
	EpsilonClosure(in,TempStates,TempStatesSize,ActiveStates,&ActiveStatesSize);
	printf("\nActive STates Init:::%d\n",ActiveStatesSize );
	while(ActiveStatesSize!=0 && string[i]!='\0')
	{
		printf("\nENCOUNTERED:::%c ActiveStates:::%d \n",string[i],ActiveStatesSize);
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
			printf("\nAttribute::{%s}::\nRegular Expression::{%s}\n ",r.Attribute,r.Expression);
			if(type1>0)
			{
			infixToPostFix(r.Expression,postfix);
			//printf("\nPostfix ::&%s&",postfix);
			strcpy(r.Expression,postfix);
			RegExpToNFA(&r,strlen(r.Expression),&N[count]);
			//printNFA(&N[count]);
			}else
			{
			//printf("STRLEN::%ld\n",strlen(r.Expression));
				RegExpToNFADirect(&r,strlen(r.Expression),&N[count]);

			}
			N[count].Attribute=getId(r.Attribute);
			printf("\nNFA FOR %d CREATED\n",N[count].Attribute);
			// if(count==1||count==2){
				
			// printNFA(&N[count]);
			// }
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


/***********************************************U**NIMPLEMENTED***********************************************************/

/*************************************************************************************************************************/
int getNFAID(NFA *N){
	return N->Attribute;
	// return 0;
}




/****************************** High Level Interface *****************************************/

/************************DFA CODES**************************************************/
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
	DFAState *states;
	int stateCount;
	int start;
	int *acceptStates;
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
		if(D->nextState[i]==NULL){
			printf("\nMemory Alloc failed !!\n");
			exit(-1);
		}
	}
	for(int i=0;i<MAXDFASIZE;i++){
		for(int j=0;j<258;j++){
			D->nextState[i][j]=-1;
		}
	}
	D->states=(DFAState *)malloc(sizeof(DFAState)*MAXDFASIZE);
	if(D->states==NULL){
		printf("\nAlloc failed ");
		exit(-1);
	}
	D->acceptStates=(int *)malloc(sizeof(int)*MAXDFASIZE);
	if(D->acceptStates==NULL){
		printf("\nAlloc failed ");
		exit(-1);
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
	// printf("\nAccept state = %d\n",N->AcceptState);
	mark_AcceptStates(D,N->AcceptState);
	// printf("\nDFA Successfully created !!!\n");
	return;
}
int getNextState(DFA *D,int state,int symbol){
	if(state>(D->stateCount)){
		printf("\nRequested State does not exist in DFA !!\n");
		exit(-1);
	}
	return (D->nextState)[state][symbol];
}
#define MAXDFACOUNT 60
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
	// printf("\n");
	while(1){
		if(string[lbegin]==' '||string[lbegin]=='\0'){
			// end of lexeme found
			break;
		}
		// printf("CHAR  = %c;\t",string[lbegin] );
		nextstate=getNextState(D,state,(int)string[lbegin]);
		if(isEmptyState(D,nextstate)==1){
			// we have gone to reject state ,,, so take it as a begining of next lexeme
			break;
		}
		if(nextstate==-1){
			// printf("\nCant simulate anymore !!\n");
			// lexeme is not recognised further by this DFA!!
			break;
		}else{
			// printf("\nsimulating !!\n");
		}
		state=nextstate;
		lbegin++;
	}
	// printf("\nState = %d",state);
	// printf("\nThe accept States : ");
		// printList(D->acceptStates,D->acceptCount+1);
	if(isAcceptState(D,state)==0){
		(*newbegin)=begin;
		Attrib->string[0]='\0';
		return NOTOK;
	}
	// printf("\nAccepted by DFA = %d",getDFAID(D));
	(*newbegin)=lbegin;
	int i=0;
	for(i=begin;i<=(lbegin-1);i++){
		Attrib->string[i-begin]=string[i];
	}
	Attrib->string[i-begin]='\0';
	// if(isAcceptState(D,state)==1){
	if(getDFAID(D)==NOTOK){
		printf("\nShrihari Fucked again !!\n");
		exit(-1);
	}
	return getDFAID(D);
	// }
}
/*******************************************************************************************/
#endif