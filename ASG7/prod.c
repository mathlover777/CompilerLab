#ifndef PRODFILE
#define PRODFILE
#include <stdio.h>
#include <stdlib.h>
#include "string.c"
#include "fileio.c"
#include "generic.c"


struct Prod{
	char *head;
	char **body;
	int blen;
};
typedef struct Prod Prod;

void initProd(Prod *a){
	a->head=(char *)malloc(sizeof(char)*(MAXSYMLEN+1));
	a->body=(char **)malloc(sizeof(char *)*MAXPRODLEN);
	a->blen=0;
	for(int i=0;i<MAXPRODLEN;i++){
		a->body[i]=(char *)malloc(sizeof(char)*(MAXSYMLEN+1));
	}
	return;
}
void freeProd(Prod *a){
	free(a->head);
	for(int i=0;i<MAXPRODLEN;i++){
		free(a->body[i]);
	}
	free(a->body);
	a->blen=0;
	a->head=0;
	a->body=0;
	return;
}
void createProd(Prod *a,char *head,char **body,int blen){
	copyString(a->head,head);
	for(int i=0;i<blen;i++){
		copyString(a->body[i],body[i]);
	}
	a->blen=blen;
	return;
}
void printProd(Prod *a){
	printf("\n[%s -->",a->head);
	for(int i=0;i<a->blen;i++){
		printf(" %s ",a->body[i]);
	}
	printf("]\n");
	return;
}
void extractProd(Prod *a,char *head,char **body,int *blen){
	copyString(head,a->head);
	(*blen)=a->blen;
	for(int i=0;i<a->blen;i++){
		copyString(body[i],a->body[i]);
	}
	return;
}
void initProdList(Prod **a){
	(*a)=(Prod *)malloc(sizeof(Prod)*MAXPRODCOUNT);
	for(int i=0;i<MAXPRODCOUNT;i++){
		initProd(&((*a)[i]));
	}
	return;
}
void freeProdList(Prod **a){
	for(int i=0;i<MAXPRODCOUNT;i++){
		freeProd(&((*a)[i]));
	}
	free(*a);
	(*a)=0;
	return;
}
void addProduct(Prod *a,int id,char *head,char **body,int blen){
	createProd(&(a[id]),head,body,blen);
}
void createProdList(FILE *fp,Prod **a,int *xpCount){
	// fp is assumed to be pointing to the first production
	char buffer[MAXBUFFERSIZE];
	initProdList(a);
	char *words[MAXPRODLEN];
	for(int i=0;i<MAXPRODLEN;i++){
		words[i]=(char *)malloc(sizeof(char)*MAXSYMLEN);
	}
	int wCount;
	int pCount=0;
	int dummyFlag;
	while(1){
		readLine(fp,buffer,&dummyFlag);
		lineToWords(buffer,words,&wCount);
		if(wCount==0){
			printf("\nWARNING EMPTY LINE\n");
		}
		if(isEqual(words[0],"PRODEND")==1){
			break;
		}
		addProduct(*a,pCount,words[0],&(words[1]),(wCount-1));
		pCount++;
	}
	// for(int i=0;i<pCount;i++){
	// 	printf("\nPRODUCTION_%d : ",pCount);
	// 	printProd(&((*a)[i]));
	// }
	for(int i=0;i<MAXPRODLEN;i++){
		free(words[i]);
	}
	(*xpCount)=pCount;
}
#endif