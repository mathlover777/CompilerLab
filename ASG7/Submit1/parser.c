#ifndef PARSER
#define PARSER
#include <stdio.h>
#include <stdlib.h>
#include "parsergen.c"
#include "string.c"
#include "stack.c"


char *tokens[MAXTOKENCOUNT];
int tCount;
FILE *tp;
int TOKENENDFLAG;

const char *TFILE="token.txt";
stack S;
char Line[MAXBUFFERSIZE];

void initializeParser(){
	TOKENENDFLAG=0;
	for(int i=0;i<MAXTOKENCOUNT;i++){
		tokens[i]=(char *)malloc(sizeof(char)*MAXTOKENSIZE);
	}
	initStack(&S);
	return;
}
void freeParser(){
	for(int i=0;i<MAXTOKENCOUNT;i++){
		free(tokens[i]);
		tokens[i]=0;
	}
	freeStack(&S);
	return;
}
void getTokenString_test(){
	printf("\nEnter String : ");
	char buffer[MAXBUFFERSIZE];
	scanf("%[^\n]",buffer);
	// printf("\ninput : %s\n",buffer);
	tCount=0;
	lineToWords(buffer,tokens,&tCount);
	copyString(tokens[tCount],"$");
	tCount++;
	printf("\nTokens :-->\n");
	for(int i=0;i<tCount;i++){
		printf("{%s}\t",tokens[i]);
	}
	printf("\n");
	TOKENENDFLAG=1;
	return;
}
void getTokenString(){
	tCount=0;
	readLine(tp,Line,&TOKENENDFLAG);
	printf("\nLine = {%s}\n",Line);
	lineToWords(Line,tokens,&tCount);
	copyString(tokens[tCount],"$");
	tCount++;
	printf("\nTokens : --->\n");
	for(int i=0;i<tCount;i++){
		printf("{%s}\t",tokens[i]);
	}
	printf("\n");
	return;
}
int isTerminal(char *word){
	for(int i=0;i<TCount-1;i++){
		if(isEqual(Terminals[i],word)==1){
			return 1;
		}
	}
	return 0;
}
int getNTid(char *word){
	for(int i=0;i<NTCount;i++){
		if(isEqual(NonTerminals[i],word)==1){
			return i;
		}
	}
	return -1;
}
int getTid(char *word){
	for(int i=0;i<TCount;i++){
		if(isEqual(Terminals[i],word)==1){
			return i;
		}
	}
	return -1;
}
void parseTokens(){
	pushStack(&S,"$");
	pushStack(&S,start);
	char X[MAXTOKENSIZE];
	int ip=0;
	char a[MAXTOKENSIZE];
	char dummyHead[MAXTOKENSIZE];
	char *words[MAXTOKENCOUNT];
	int wCount=0;
	for(int i=0;i<MAXTOKENCOUNT;i++){
		words[i]=(char *)malloc(sizeof(char)*(MAXTOKENSIZE));
	}
	int xid,aid,pid;
	int errorFlag=0;
	while(1){
		topStack(&S,X);
		// printf("\nTOPSTACK = {%s}",X);
		copyString(a,tokens[ip]);
		if(isEqual(X,"$")==1){
			break;
		}
		if(isEqual(X,a)==1){
			popStack(&S);
			ip++;
			printf("\nMATCH {%s}--{%s}\n",X,a);
		}else if(isTerminal(X)==1){
			printf("\n**********************************\n");
			printf("\nERROR ... Terminal on top of stack does not match with terminal of input !!");
			errorFlag=1;
			break;
		}else if(parser[getNTid(X)][getTid(a)]==-1){
			printf("\n**********************************\n");
			printf("\nERROR ... no rule to derive !!\n");
			errorFlag=1;
			break;
		}else{
			xid=getNTid(X);
			aid=getTid(a);
			pid=parser[xid][aid];
			printProd(&(prodList[pid]));
			extractProd(&(prodList[pid]),dummyHead,words,&wCount);
			popStack(&S);
			if(isEqual(words[0],"epsilon")==1){
				wCount=0;
			}
			for(int i=wCount-1;i>=0;i--){
				pushStack(&S,words[i]);
			}
		}
	}
	for(int i=0;i<MAXTOKENCOUNT;i++){
		free(words[i]);
		words[i]=0;
	}
	wCount=0;
	if(errorFlag==1){
		return;
	}
	if(isEqual(tokens[ip],"$")==1){
		printf("\n**********************************\n");
		printf("\nSTRING ACCEPTED !!!\n");
	}
	else{
		printf("\n**********************************\n");
		printf("\nWARNING UNPARSED TOKENS LEFT in BUFFER !!\n");
		printf("\nLEFT TOKENS : \n");
		while(isEqual(tokens[ip],"$")!=1){
			printf(" {%s} ",tokens[ip]);
			ip++;
		}
		printf(" {%s} ",tokens[ip]);
	}
	return;
}
void parse(){
	initializeParser();
	tp=initializeFileRead(TFILE,&TOKENENDFLAG);
	while(1){
		getTokenString();
		if(tCount!=0){
			parseTokens();
		}
		if(TOKENENDFLAG==1){
			break;
		}
	}
	freeParser();
	return;
}

#endif