#include <stdlib.h>
	#include <stdio.h>
	#define MAXVAR 1000
	struct MatrixData{
		char *name;
		int m;
		int n;
	};
	
	int a=0;
	int state=0;
	int last=0;
	int line=1;
	int printCheck=0;
	int endline=0;
	int comment=0;
	int currentMatrix=0;
	int firstMatrix=0;
	int matrixCount=0;
	int redefinition=0;
	struct MatrixData Mlist[MAXVAR];
	char *f;

	void initMatData(){
		int i=0;
		for(i=0;i<MAXVAR;i++){
			Mlist[i].name=(char *)malloc(sizeof(char)*100);
			Mlist[i].name[0]='\0';
			Mlist[i].m=0;
			Mlist[i].n=0;
		}
		return;
	}
	void deleteMatData(){
		int i=0;
		for(i=0;i<MAXVAR;i++){
			free(Mlist[i].name);
			Mlist[i].name=NULL;
			Mlist[i].m=Mlist[i].n=0;
		}
		return;
	}
	void printMatData(){
		int i=0;
		for(i=0;i<matrixCount;i++){
			printf("\n%s",Mlist[i].name);
		}
		return;
	}
	void copyString(char *dest,const char *source){
		while((*source)!='\0'){
			(*dest)=(*source);
			source++;
			dest++;
		}
		(*dest)=(*source);
		return;
	}
	int isEqual(const char *a,const char *b){
		while((*a)!='\0'){
			if((*a)!=(*b)){
				return 0;
			}
			a++;
			b++;
		}
		if((*a)!=(*b)){
			return 0;
		}
		return 0;
	}
	int getMatrixId(const char *a){
		int i=0;
		for(i=0;i<matrixCount;i++){
			if(isEqual(a,Mlist[i].name)==1){
				return i;
			}
		}
		return -1;
	}
	void addMatrix(){
		redefinition=0;
		if(getMatrixId(f)==1){
			printf("\nERROR at Line %d ... Redifiniton of Matrix : '%s'",line,f);
			redefinition=1;
			return;
		}
		if(matrixCount==MAXVAR){
			printf("\nEXIT TOO MUCH MATRICES !!\n");
			exit(-1);
		}
		copyString(Mlist[matrixCount].name,f);
		currentMatrix=matrixCount;
		matrixCount++;
	}

	int main(){
	//(("+"|"-")[0-9]+)|
	initMatData();
	//yylex();
	//printf("\nHERE ");
	printMatData();
	deleteMatData();
	return 0;
}
