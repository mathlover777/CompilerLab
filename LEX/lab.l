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
int firstFlag=0;
struct MatrixData Mlist[MAXVAR];
int mflag=0;
int nflag=0;
int mhold;
int initCheck;

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
	if(getMatrixId(yytext)!=-1){
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
	// ignored to do this stuff DEF
	// A[2,3]; where A is at the next line ... to make this valid !!
	//if(state==0){
	//	printf("\nERROR in LINE = %d .. unrecognised KEYWORD = '%s'\n",line,yytext);
	//	//exit(-1);
	//	return 1;
	//}
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
			return 0;
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
		if(initCheck==0){
			initCheck=1;
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
		else{
			printf("\nERROR at line %d ... can only initialize one Matrix in a single line in this version !!",line);
			return 0;
		}
	}
	/*****************************************************************************/

	/*****************************************************************************/
	// checking valid dimensions for addition
	if(state==1){
		if(firstFlag==0){
			firstFlag=1;
			int cMatrix=getMatrixId(yytext);
			if(cMatrix==-1){
				printf("\nERROR at line %d ... Going to store result of addition in undefined matrix '%s' !!",line,yytext);
				return 0;
			}
			currentM=Mlist[cMatrix].m;
			currentN=Mlist[cMatrix].n;
			return 0;
		}
		else{
			int cMatrix=getMatrixId(yytext);
			if(cMatrix==-1){
				printf("\nERROR at line %d ... Going to use addition of undefined matrix '%s'!!",line,yytext);
				return 0;
			}
			if(Mlist[cMatrix].m!=currentM||Mlist[cMatrix].n!=currentN){
				printf("\nERROR at line %d ...adding matrices of differnent dim '%s'!!",line,yytext);
			}
			return 0;
		}
	}
	/*****************************************************************************/
	
	/*****************************************************************************/
	// checking for CMUL
	if(state==3){
		if(firstFlag==0){
			firstFlag=1;
			int cMatrix=getMatrixId(yytext);
			if(cMatrix==-1){
				printf("\nERROR at line %d ... Going to store result of CMUL in undefined matrix '%s' !!",line,yytext);
				return 0;
			}
			currentM=Mlist[cMatrix].m;
			currentN=Mlist[cMatrix].n;
			return 0;
		}
		else{
			int cMatrix=getMatrixId(yytext);
			if(cMatrix==-1){
				printf("\nERROR at line %d ... Going to use CMUL of undefined matrix '%s'!!",line,yytext);
				return 0;
			}
			if(Mlist[cMatrix].m!=currentM||Mlist[cMatrix].n!=currentN){
				printf("\nERROR at line %d ...differnent dim '%s' Matrix in CMUL!!",line,yytext);
			}
			return 0;
		}	
	}
	/*****************************************************************************/


	/*****************************************************************************/
	// checking for MMUL
	if(state==2){
		if(firstFlag==0){
			firstFlag=1;
			int cMatrix=getMatrixId(yytext);
			if(cMatrix==-1){
				printf("\nERROR at line %d ... Going to store result of MMUL in undefined matrix '%s' !!",line,yytext);
				return 0;
			}
			currentM=Mlist[cMatrix].m;
			currentN=Mlist[cMatrix].n;
			return 0;
		}
		if(mflag==0){
			mflag=1;
			mhold=0;
			int cMatrix=getMatrixId(yytext);
			if(cMatrix==-1){
				printf("\nERROR at line %d ... Going to use MMUL of undefined matrix '%s'!!",line,yytext);
				return 0;
			}
			if(Mlist[cMatrix].m!=currentM){
				printf("\nERROR at line %d ...1st dim '%s' of is invalid!!",line,yytext);
			}
			mhold=Mlist[cMatrix].n;
			return 0;
		}
		if(nflag==0){
			nflag=1;
			int cMatrix=getMatrixId(yytext);
			if(cMatrix==-1){
				printf("\nERROR at line %d ... Going to use MMUL of undefined matrix '%s'!!",line,yytext);
				return 0;
			}
			if(Mlist[cMatrix].n!=currentN){
				printf("\nERROR at line %d ...  2nd dim m2 '%s' is differnent from resultant!!",line,yytext);
			}
			if(mhold!=Mlist[cMatrix].m){
				printf("\nERROR at line %d ... cant multiply m*n and a*b mat where n!=a !!",line);
			}
			return 0;
		}
		printf("\nERROR at line %d ... SYNTAX MMUL m,m1,m2..more than 3 parameters is currently not allowed!!",line);
		return 0;
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
			currentRow++;
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
			if(state!=0){
				printf("\nERROR at line %d ... multiple command '%s' not allowed in single line !!",line,yytext);
				//printf("\nstate : %d",state);
			}else{
				printf("<ADD>");
				state=1;
				last=1;
				lastToken=1;
				endline=0;
				comment=0;
			}
		}
	}
MMUL {
		if(state!=100){
			if(state!=0){
				printf("\nERROR at line %d ... multiple command '%s' not allowed in single line !!",line,yytext);
				//printf("\nstate : %d",state);
			}else{
				printf("<MMUL>");
				state=2;
				last=1;
				lastToken=2;
				endline=0;
				comment=0;
			}
		}
	 }
CMUL {
		if(state!=100){
			if(state!=0){
				printf("\nERROR at line %d ... multiple command '%s' not allowed in single line !!",line,yytext);
				//printf("\nstate : %d",state);
			}else{
				printf("<CMUL>");
				state=3;
				last=1;
				lastToken=3;
				endline=0;
				comment=0;
			}
		}
	 }
DEF {
		if(state!=100){
			if(state!=0){
				printf("\nERROR at line %d ... multiple command '%s' not allowed in single line !!",line,yytext);
				//printf("\nstate : %d",state);
			}else{
				printf("<DEF>");
				state=4;
				last=1;
				lastToken=4;
				endline=0;
				comment=0;
			}
		}
	}
INIT {
		if(state!=100){
			if(state!=0){
				printf("\nERROR at line %d ... multiple command '%s' not allowed in single line !!",line,yytext);
			}
			printf("<INIT>");
			state=5;
			last=1;
			lastToken=5;
			initCheck=0;
			endline=0;
			comment=0;
		}
	 }
PRINT {
		if(state!=100){
			if(state!=0){
				printf("\nERROR at line %d ... multiple command '%s' not allowed in single line !!",line,yytext);
			}
			printf("<PRINT>");
			printCheck=0;
			state=6;
			last=1;
			lastToken=6;
			endline=0;
			comment=0;
		}
	}

(([0-9]+)|(("+"|"-")[0-9]+)) {
		if(state!=100){
			printf("<INT>");
			last=1;
			testInt();
			lastToken=7;
			endline=0;
			//comment=0;
		}
	   }
[a-zA-Z0-9]+ {
				//printf("\nhere");
				if(state!=100){
					if(testString()==0){
						printf("<ID>");
						last=1;
						lastToken=8;
					}
				}
				endline=0;
				//comment=0;
		     }

("//") {
			if(state!=100){
				printf("<COMMENT>");
				state=100;
				//last=1;
				comment=1;
				lastToken=9;
				endline=0;
			}
	   }
(",") {
			if(state!=100){
				printf("<COMMA>");
				last=1;
				lastToken=10;
				endline=0;
				//comment=0;
			}
	  }
(";") {		
			if(state!=100){
				testEnd();
				printf("<ENDS>");
				printf("\n");
				last=0;
				//printf("last=%d",last);
				//state=-1;
				//last=1;
				endline=1;
				lastToken=11;
				state=0;
				comment=0;
			}
	  }
(":") {
			if(state!=100){
				printf("<ENDR>");
				last=1;
				lastToken=12;
				endline=0;
				comment=0;
			}
	  }
("[") {
			if(state!=100){
				printf("<SQBKTO>");
				last=1;
				lastToken=13;
	 			endline=0;
				comment=0;
	 		}
	  }
("]") {
			if(state!=100){
				printf("<SQBKTC>");
				last=1;
				lastToken=14;
				endline=0;
				comment=0;
			}
	  }
(" "|\t)* ;
(\n) {
		if(1){
			//printf("last = %d",last);
			if(last!=0&&(endline==1||comment==1)){
				printf("\n");
				last=0;
			}
			if(endline==1||comment==1){
				state=0;
				endline=0;
				comment=0;
				firstFlag=0;
				mflag=0;
				nflag=0;
			}
			line++;
			//lastToken=15;//this is never used in program
			// and disabled to implement complete multiline commands
		}
}
. ;
%%
int main(){
	//{
	//	if(state!=100){
	//		printf("\nERROR at line %d .. invalid character '%s'",line,yytext);
	//	}
	//}
	//(("+"|"-")[0-9]+)|
	initMatData();
	//printf("\n//***********************OUTPUT*********************************//\n");
	yylex();
	//printMatData();
	deleteMatData();
	//printf("//***********************OUTPUT*********************************//\n");
	return 0;
}
