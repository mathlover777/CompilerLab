#ifndef SET
#define SET

#include <stdio.h>
#include <stdlib.h>
#include "string.c"
#include "structures.c"
// struct setElt{
// 	char varname[20];
// 	struct setElt *next;
// };
// typedef struct setElt setElt;
// struct Set{
// 	setElt *head;
// };
// typedef struct Set Set;
void initSet(Set *S){
	S->head=NULL;
	return;
}
void insertSet(Set *S,char *varname){
	setElt *temp=(setElt *)malloc(sizeof(setElt));
	copyString(temp->varname,varname);
	temp->next=S->head;
	S->head=temp;
	return;
}
void deleteSet(Set *S){
	setElt *temp=S->head;
	setElt *xtemp;
	while(temp!=NULL){
		xtemp=temp->next;
		free(temp);
		temp=xtemp;
	}
	S->head=NULL;
	return;
}
void moveSet(Set *A,Set *B){
	A->head=B->head;
	B->head=NULL;
	return;
}
void printSet(Set *S){
	setElt *temp=S->head;
	printf("\n{");
	while(temp!=NULL){
		printf(" <%s> ",temp->varname);
		temp=temp->next;
	}
	printf("}");
	return;
}

#endif