#ifndef FILEIO
#define FILEIO
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
#endif