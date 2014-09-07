%{
#define MAXVAR 1000
#include <stdlib.h>
#include <stdio.h>
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
int lastToken=-1;
int currentRow;
int currentCol;
int currentM;
int currentN;
struct MatrixData Mlist[MAXVAR];

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
	printf("\nMatrix Count = %d",matrixCount);
	for(i=0;i<matrixCount;i++){
		printf("\n%s\t%d\t%d",Mlist[i].name,Mlist[i].m,Mlist[i].n);
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
	//printf("\nCOMPARING : '%s' and '%s'",a,b);
	while((*a)!='\0'){
		if((*a)!=(*b)){
			//printf("\nRESULT unmatched");
			return 0;
		}
		a++;
		b++;
	}
	if((*a)!=(*b)){
		return 0;
	}
	return 1;
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
	if(getMatrixId(yytext)==1){
		printf("\nERROR at Line %d ... Redifiniton of Matrix : '%s'",line,yytext);
		redefinition=1;
		return;
	}
	if(matrixCount==MAXVAR){
		printf("\nEXIT TOO MUCH MATRICES !!\n");
		exit(-1);
	}
	copyString(Mlist[matrixCount].name,yytext);
	currentMatrix=matrixCount;
	matrixCount++;
}
int addDimensionData(){
	int n=atoi(yytext);
	//printf("\nYYTEXT = %s n = %d",yytext,n);
	if(n<=0){
		printf("\nERROR at LINE %d ... Dimension must be > 0 ",line);
		return 1;
	}
	if(lastToken==13){
		Mlist[currentMatrix].m=n;
		return 0;
	}
	if(lastToken==10){
		Mlist[currentMatrix].n=n;
		return 0;
	}
	printf("\nUNKNOWN ERROR at LINE %d ... ",line);
	return 1;
}
int testString(){
	// yytext points to the matched text
	
	/*****************************************************************************/
	// if the currentline does not start with a valid KEYWORD
	if(state==0){
		printf("\nERROR in LINE = %d .. unrecognised KEYWORD = '%s'\n",line,yytext);
		//exit(-1);
		return 1;
	}
	/*****************************************************************************/
	
	/*****************************************************************************/
	// detecting the PRINT A,B error
	if(state==6){
		if(printCheck==0){
			printCheck=1;
			return 0;
		}
		if(printCheck==1){
			printf("\nERROR in LINE = %d ... PRINT A,B...; not allowed !!\n",line);
			return 2;
		}
		printf("\nUNRECOGNISED ERROR !! printCheck has invalid value !!\n");
		return 3;
	}
	/*****************************************************************************/

	/*****************************************************************************/
	// add the matrix in the symbol table if we are in add state
	if(state==4){
		addMatrix(yytext);
	}
	/*****************************************************************************/

	/*****************************************************************************/
	// init validation
	if(state==5){
		int cMatrix=getMatrixId(yytext);
		//printf("\ncMatrix  : %d",cMatrix);
		if(cMatrix==-1){
			printf("\nERROR at line %d ... going to initialize undefined matrix !!",line);
			printMatData();
			//printf("\nGOING TO GET :'%s'",yytext);
			return 0;
		}
		currentMatrix=cMatrix;
		currentM=Mlist[currentMatrix].m;
		currentN=Mlist[currentMatrix].n;
		currentRow=0;
		currentCol=0;
		// initialization initialized :D
	}
	/*****************************************************************************/

	return 0;
}
int testInt(){
	/*****************************************************************************/
	// if we are in a DEF type statement then add dimension data to last added matrix
	if(state==4){
		addDimensionData();
	}
	/*****************************************************************************/

	/*****************************************************************************/
	// init validation
	if(state==5){
		if(lastToken==13){
			currentRow=1;
			currentCol=1;
		}
		if(lastToken==12){
			currentCol=1;
			currentRow++;,3
		}
		if(lastToken==10){
			currentCol++;
		}
		if(currentRow>currentM||currentCol>currentN){
			printf("\nERROR at Line %d ... Dim missmatch in initialization!!",line);
		}
	}
	/*****************************************************************************/
}
int testEnd(){
	if(state==5){
		if(currentRow!=currentM||currentCol!=currentN){
			printf("\nCROW = %d CM = %d\tCCOL = %d CN = %d",currentRow,currentM,
				currentCol,currentN);
			printf("\nERROR at Line %d ... dim missmatch in initialization!!",line);	
		}
	}
}
%}
INT ([0-9])+
%%
ADD {	
		if(state!=100){
			printf("<ADD>");
			state=1;
			last=1;
			lastToken=1;
		}
	}
MMUL {
		if(state!=100){
			printf("<MMUL>");
			state=2;
			last=1;
			lastToken=2;
		}
	 }
CMUL {
		if(state!=100){
			printf("<CMUL>");
			state=3;
			last=1;
			lastToken=3;
		}
	 }
DEF {
		if(state!=100){
			printf("<DEF>");
			state=4;
			last=1;
			lastToken=4;
		}
	}
INIT {
		if(state!=100){
			printf("<INIT>");
			state=5;
			last=1;
			lastToken=5;
		}
	 }
PRINT {
		if(state!=100){
			printf("<PRINT>");
			state=6;
			last=1;
			lastToken=6;
		}
	}

(([0-9]+)|(("+"|"-")[0-9]+)) {
		if(state!=100){
			printf("<INT>");
			last=1;
			testInt();
			lastToken=7;
		}
	   }
[a-zA-Z0-9]+ {
				if(state!=100){
					if(testString()==0){
						printf("<ID>");
						last=1;
						lastToken=8;
					}
				}
		     }

("//") {
			if(state!=100){
				//printf("<COMMENT>");
				state=100;
				//last=1;
				comment=1;
				lastToken=9;
			}
	   }
(",") {
			if(state!=100){
				printf("<COMMA>");
				last=1;
				lastToken=10;
			}
	  }
(";") {		
			if(state!=100){
				testEnd();
				printf("<ENDS>");
				state=-1;
				last=1;
				endline=1;
				lastToken=11;
			}
	  }
(":") {
			if(state!=100){
				printf("<ENDR>");
				last=1;
				lastToken=12;
			}
	  }
("[") {
			if(state!=100){
				printf("<SQBKTO>");
				last=1;
				lastToken=13;
	 		}
	  }
("]") {
			if(state!=100){
				printf("<SQBKTC>");
				last=1;
				lastToken=14;
			}
	  }
(" "|\t)* ;
(\n) {
		if(state!=0){
			if(last!=0&&(endline==1||comment==1)){
				printf("\n");
				last=0;
			}
			if(endline==1||comment==1){
				state=0;
				endline=0;
				comment=0;
			}
			line++;
			lastToken=15;
		}
}
. ;
%%
int main(){
	//(("+"|"-")[0-9]+)|
	initMatData();
	yylex();
	printMatData();
	deleteMatData();
	printf("\n");
	return 0;
}
