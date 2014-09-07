#ifndef GENERIC
#define GENERIC

#include "string.c"
#define MAXTERMINALCOUNT 100
#define MAXNONTERMINALCOUNT 100

#define MAXSYMLEN 20
#define MAXPRODLEN 100
#define MAXPRODCOUNT 1000
#define MAXBUFFERSIZE 1000
#define MAXTOKENCOUNT 1000
#define MAXTOKENSIZE 20

struct SymList{
	char *symbol[MAXTERMINALCOUNT];
	int count;
};
typedef struct SymList SymList;
typedef struct SymList FirstList;
typedef struct SymList FollowList;

void initSymList(SymList *a){
	for(int i=0;i<MAXTERMINALCOUNT;i++){
		a->symbol[i]=(char *)malloc(sizeof(char)*(MAXSYMLEN+1));
	}
	a->count=0;
	return;
}
void freeSymList(SymList *a){
	for(int i=0;i<MAXTERMINALCOUNT;i++){
		free(a->symbol[i]);
		a->symbol[i]=0;
	}
	a->count=0;
	return;
}
void freeSymTable(SymList **a,int size){
	for(int i=0;i<size;i++){
		// printf("\n*************%d\n",i);
		freeSymList(&((*a)[i]));
	}
	free(*a);
	(*a)=0;
	return;
}
void initSymTable(SymList **a,int size){
	(*a)=(SymList *)malloc(sizeof(SymList)*size);
	for(int i=0;i<size;i++){
		initSymList(&((*a)[i]));
	}
	return;
}
void printSymList(SymList *a){
	printf("\n");
	for(int i=0;i<a->count;i++){
		printf("{%s}\t",a->symbol[i]);
	}
	return;
}
void addSymList(SymList *a,int id,char **symbols,int count){
	for(int i=0;i<count;i++){
		copyString(a[id].symbol[i],symbols[i]);
	}
	a[id].count=count;
}
void createSymList(FILE *fp,SymList **a,int maxsize,const char *ENDSTRING){
	// fp is assumed to in the first line for the list
	char buffer[MAXBUFFERSIZE];
	initSymTable(a,maxsize);
	int dummyFlag;
	char *words[maxsize];
	for(int i=0;i<maxsize;i++){
		words[i]=(char *)malloc(sizeof(char)*MAXSYMLEN);
	}
	int wCount;
	int pCount=0;
	while(1){
		readLine(fp,buffer,&dummyFlag);
		// printf("\nLINE = {%s}",buffer);
		lineToWords(buffer,words,&wCount);
		if(wCount==0){
			printf("\nWARNING EMPTY LINE\n");
		}
		if(isEqual(words[0],ENDSTRING)==1){
			break;
		}
		addSymList(*a,pCount,&(words[0]),wCount);
		// printSymList(&((*a)[pCount]));
		pCount++;
		
	}
	for(int i=0;i<maxsize;i++){
		free(words[i]);
	}
}
int isExist(SymList *a,const char *symbol){
	for(int i=0;i<a->count;i++){
		if(isEqual(a->symbol[i],symbol)==1){
			return 1;
		}
	}
	return 0;
}
#endif