#ifndef STACK
#define STACK
#define MAXSTACKSIZE 1000
#define MAXWORDSIZE 20
struct stack{
	char *words[MAXSTACKSIZE];
	int size;
};
typedef struct stack stack;
void initStack(stack *s){
	for(int i=0;i<MAXSTACKSIZE;i++){
		s->words[i]=(char *)malloc(sizeof(char)*MAXWORDSIZE);
	}
	s->size=0;
	return;
}
void freeStack(stack *s){
	for(int i=0;i<MAXSTACKSIZE;i++){
		free(s->words[i]);
		s->words[i]=0;
	}
	s->size=0;
	return;
}
void pushStack(stack *s,const char *word){
	if((s->size)==MAXSTACKSIZE){
		printf("\nError STACK OVERFLOW !!\n");
		exit(-3);
	}
	copyString(s->words[s->size],word);
	s->size++;
	return;
}
void popStack(stack *s){
	if(s->size==0){
		printf("\nError Going to Empty Already Stack !!\n");
		exit(-4);
	}
	s->size--;
	return;
}
int isEmptyStack(stack *s){
	if(s->size==0){
		return 1;
	}
	return 0;
}
void topStack(stack *s,char *word){
	if(isEmptyStack(s)==1){
		printf("\nError Cant Find Top of Empty Stack !!\n");
		exit(-4);
	}
	copyString(word,s->words[s->size-1]);
	return;
}
#endif