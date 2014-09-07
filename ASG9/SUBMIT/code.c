#ifndef CODE
#define CODE
#include <stdio.h>
#include <stdlib.h>
#include "structures.c"
#include "string.c"
#include "fileio.c"

int tIndex=0;
int lIndex=0;
const char *OUTFILE="output.txt";
void getNewTemp(char *varname){
	sprintf(varname,"t%d",tIndex);
	tIndex++;
	return;
}
int compareType(const char *type1,const char *type2,char *type){
	if(isEqual(type1,type2)==1){
		copyString(type,type1);
		return 1;
	}
	if((isEqual(type1,"int")==0)&&(isEqual(type1,"float")==0)
		||(isEqual(type2,"int")==0)&&(isEqual(type2,"float")==0)){
		// non int float data type  and they are not same
		type[0]='\0';
		printf("\nERROR going to use {%s} EXP with {%s}",type1,type2);
		return 0;
	}
	// we are here means we are using int and float only and they
	// are diffenent
	copyString(type,"float");
	if(isEqual(type1,"int")==1){
		return 10;
	}
	return 20;
}
void getNewLabel(char *label){
	sprintf(label,"L%d",lIndex);
	lIndex++;
	return;
}
void initCode(code *A){
	A->head=NULL;
	A->tail=NULL;
	return;
}
void addCode(code *C,const char *line){
	codeElt *temp=(codeElt *)malloc(sizeof(codeElt));
	copyString(temp->Line,line);
	temp->next=NULL;
	if(C->head==NULL&&C->tail==NULL){
		C->head=temp;
		C->tail=temp;
		return;
	}
	// if(C->head==NULL||C->tail==NULL){
	// 	printf("\nFATAL ERROR : INCONSISTENCY IN CODE!!");
	// 	exit(-1);
	// }
	C->tail->next=temp;
	C->tail=temp;
	return;
}
void appendCodeToLeft(code *C,code *A){
	if(C->head!=NULL){
		C->tail->next=A->head;
		if(A->tail!=NULL){
			C->tail=A->tail;
		}
	}else{
		C->head=A->head;
		C->tail=A->tail;
	}
	A->tail=NULL;
	A->head=NULL;
	return;
}
void concatCode(code *C,code *A,code *B){
	// printf("\nWE ARE HERE\n");
	// printf("\nA->head = %p A->tail = %p",A->head,A->tail);
	// printf("\nB->head = %p B->tail = %p",B->head,B->tail);
	if(A->head!=NULL){
		C->head=A->head;
		A->tail->next=B->head;
		if(B->tail!=NULL){
			C->tail=B->tail;
		}else{
			C->tail=A->tail;
		}
		// printf("\nC->head = %p C->tail = %p",C->head,C->tail);
	}else{
		if(B->head==NULL){
			C->head=NULL;
			C->tail=NULL;
			return;
		}
		C->head=B->head;
		C->tail=B->tail;
	}
	A->head=NULL;
	B->head=NULL;
	A->tail=NULL;
	B->tail=NULL;
	return;
}
void moveCode(code *Dest,code *Src){
	Dest->head=Src->head;
	Dest->tail=Src->tail;
	Src->head=NULL;
	Src->tail=NULL;
	return;
}
void deleteCode(code *C){
	codeElt *temp=C->head;
	codeElt *xtemp;
	while(temp!=NULL){
		xtemp=temp->next;
		free(temp);
		temp=xtemp;
	}
	C->head=NULL;
	C->tail=NULL;
	return;
}
void dumpCode(code *C){
	resetfile(OUTFILE);
	codeElt *c;
	c=C->head;
	while(c!=NULL){
		addLine(OUTFILE,c->Line);
		c=c->next;
	}
	return;
}
void printCode(code *C){
	codeElt *c;
	c=C->head;
	printf("\nCODE STMT : {");
	while(c!=NULL){
		printf("\n{%s}",c->Line);
		c=c->next;
	}
	printf("\n}");
	return;		
}
#endif