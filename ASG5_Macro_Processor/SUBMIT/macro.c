#include "string.c"
#include "fileio.c"
#include <stdio.h>
#include <stdlib.h>

#ifndef MACRO_C
#define MACRO_C
#define MAXMACROSIZE 20
#define MAXMACROLINE 100
#define MAXLEVEL 100
#define MAXOPCODE 100


int EXPANDING=0;
int LEVEL=0;
int DEFTABID;
int ARGTABID;
char VLIST[1000];// need 

int NESTED=0;

const char* DEFTAB="DEFTAB.txt";
const char* ARGTAB="ARGTAB.txt";
const char* NAMTAB="NAMTAB.txt";


void initialize_macroDriver();
void free_memory();
int getArgID(const char* argList,const char *argument);
void replaceOperand(char *roperand,int id);
void replaceArguments(char *xline,char *line);
void replaceID(char *line);
void getCurrentDEFTAB(char *line);
void getLine(FILE *f,int *end_f,char *line);
int isExistingMacro(char *opcode,int *xstart,int *xend);
void setARGTABID(const char *macro);
void processLine(FILE *f,int &end_f,char *line,const char *OUTPUT);
int expand(FILE *f,int *end_f,const char *line,const char *OUTPUT);
int isComment(const char *line);
int lineCount(const char *FILENAME);
void conCat(char *dline,char *dlabel,char *dopcode,char *doperand);
int define(FILE *f,int *end_f,const char *line,const char *OUTPUT);
int macroDriver(const char *INPUT,const char *OUTPUT);


void initialize_macroDriver(){
	// for(int i=0;i<MAXLEVEL;i++){
	// 	VLIST[i]=(char *)malloc(sizeof(char)*MAXLISTSIZE);
	// 	if(VLIST[i]==NULL){
	// 		printf("\nMEMORY ALLOC FAILED !!\n");
	// 		exit(-1);
	// 	}
	// }
	resetfile(DEFTAB);
	resetfile(ARGTAB);
	resetfile(NAMTAB);
	LEVEL=0;
	EXPANDING=0;

	return;
}
void free_memory(){
	// for(int i=0;i<MAXLEVEL;i++){
	// 	free(VLIST[i]);
	// }
	return;
}
int getArgID(const char* argList,const char *argument){
	int i=0,j=0;
	int found=0;
	int id=0;

	while(1){
		// printf("\nITERATION");
		while((argList[j]!=',')&&(argList[j]!='\0')){
			printf("Line[%d] = %c\n",j,argList[j]);
			j++;
		}
		if(isEqual2(argList,i,j-1,argument,0,(getSize(argument)-1))==1){
			found=1;
			break;
		}else{
			id++;
		}
		if(argList[j]=='\0'){
			printf("\nEND\n");
			break;
		}
		// j++;
		printf("\nHERE\n");
		j++;
		i=j;
	}
	if(found==0){
		// printf("\nGETARGID : ARGUMENT NOT FOUND IN ARGTAB !! id = %d\n",id);
		// exit(-1);
		return -1;
	}
	return id;
}
int getOpcodeID(const char *argList,const char *opcode){
	int i=0,j=0;
	int found=0;
	int id=0;

	while(1){
		// printf("\nITERATION");
		while((argList[j]!=',')&&(argList[j]!='\0')){
			printf("Line[%d] = %c\n",j,argList[j]);
			j++;
		}
		if(isEqual2(argList,i+1,j-1,opcode,0,(getSize(opcode)-1))==1){
			found=1;
			break;
		}else{
			id++;
		}
		if(argList[j]=='\0'){
			printf("\nEND\n");
			break;
		}
		// j++;
		// printf("\nHERE\n");
		j++;
		i=j;
	}
	if(found==0){
		// printf("\nGETARGID : ARGUMENT NOT FOUND IN ARGTAB !! id = %d\n",id);
		// exit(-1);
		return -1;
	}
	return id;
}
void replace(char *roperand,int id){
	// if(LEVEL==0){
	// 	printf("\nINCONSISTENT LEVEL\n");
	// 	exit(-1);
	// }
	int i=0,j=0;
	int count=0;
	int found=0;
	while(1){
		while(VLIST/*[0LEVEL-1]*/[j]!='\0'&&VLIST/*[0LEVEL-1]*/[j]!=','){
			j++;
		}
		if(count==id){
			found=1;
			break;
		}else{
			count++;
		}
		if(VLIST/*[0LEVEL-1]*/[j]=='\0'){
			break;
		}
		j++;
		i=j;
	}
	if(found==0){
		printf("\nTOO FEW ARGUMENTS TO MACRO !!\n");
		exit(-1);
	}
	int k=i;
	for(k=i;k<j;k++){
		roperand[k-i]=VLIST/*[0LEVEL-1]*/[k];
	}
	roperand[k-i]='\0';
	return;
}
void replaceArguments(char *xline,char *line){
	// char xlabel[MAXLABEL],xopcode[MAXINS],xoperand[MAXOPERAND];
	// char xcomment[MAXCOMMENTSIZE];
	// getContent(xline,xlabel,xopcode,xoperand,xcomment);
	
	char label[MAXLABEL],opcode[MAXINS],operand[MAXOPERAND];
	char comment[MAXCOMMENTSIZE];
	getContent(line,label,opcode,operand,comment);
	printf("\nREPLACE ARGUMENTS : XLINE ={%s}\toperand ={%s}\n",
		xline,operand);
	
	int id=getArgID(xline,operand);
	char roperand[MAXOPERAND];
	if(id!=-1){
		printf("\nREPLACE ARGUMENTS : ID = %d\n",id);
		replace(roperand,id);
		sprintf(operand,"%s",roperand);
	}
	char ropcode[MAXOPCODE];
	id=getOpcodeID(xline,opcode);
	if(id!=-1){
		printf("\nREPLACING OPCODE : iD = %d\n",id);
		replace(ropcode,id);
		sprintf(opcode,"%s",ropcode);
	}
	sprintf(line,"%s %s %s",label,opcode,operand);
	printf("\nREPLACE : RETRIEVED LINE : {%s}\n",line);

	return;
}
void getARGLIST(char *line){
	if(line[0]=='\0'){
		return;
	}
	int i=0,j=0;
	while(line[i]!=' '){
		if(line[i]=='\0'){
			line[0]='\0';
			return;
		}
		i++;
	}
	i++;
	while(line[i]!='\0'){
		line[j]=line[i];
		i++;
		j++;
	}
	line[j]='\0';
	return;
}
void replaceID(char *line){
	// if(LEVEL<1){
	// 	printf("\nINCONSISTENT ARGTABID\n");
	// 	exit(-1);
	// }
	FILE *f=fopen(ARGTAB,"r");
	int end_f=0;
	char xline[MAXBUFFERSIZE];
	xline[0]='\0';
	for(int i=0;i<ARGTABID/*[0LEVEL]*/;i++){
		if(end_f==1){
			printf("\nINCONSISTENT ARGTABID\n");
			exit(-1);		
		}
		readLine(f,xline,&end_f);
	}
	if(end_f==1){
		printf("\nINCONSISTENT ARGTABID\n");
		exit(-1);		
	}
	xline[0]='\0';
	if(end_f==0){
		readLine(f,xline,&end_f);
		clearTabs(xline);
	}
	printf("\n****REPLACEID ARGTAB(BEFORE) : {%s}\n",xline);
	getARGLIST(xline);
	printf("\n****REPLACEID ARGTAB : {%s}\n",xline);
	replaceArguments(xline,line);
	fclose(f);
	return;
}
void getCurrentDEFTAB(char *line){
	// line couting starts as 0
	// if(LEVEL<1){
	// 	printf("\nINCONSISTENT DEFTABID\n");
	// 	exit(-1);
	// }
	int i;
	FILE *f=fopen(DEFTAB,"r");
	int end_f=0;
	for(i=0;i<DEFTABID/*[0]*/+1;i++){
		if(end_f==1){
			printf("\nINCOMPLETE DEFTABLE !!\n");
			exit(-1);
		}
		readLine(f,line,&end_f);
	}
	if(end_f==1){
		printf("\nINCOMPLETE DEFTABLE !!\n");
		exit(-1);
	}
	readLine(f,line,&end_f);
	clearTabs(line);
	printf("\nDIRECT DFATAB : {%s}\n",line);

	char label[MAXLABEL],opcode[MAXINS],operand[MAXOPERAND];
	char comment[MAXCOMMENTSIZE];
	getContent(line,label,opcode,operand,comment);
	if(isEqual(opcode,"MACRO")==1){
		NESTED=1;
	}
	if((isEqual(opcode,"MACRO")!=1)&&(isEqual(operand,"MACRO")!=1)){
		printf("\nGOING TO REPLACE ID!!\n");
		replaceID(line);
	}
	DEFTABID/*[0]*/++;
	fclose(f);
	return;
}
void getLine(FILE *f,int *end_f,char *line){
	if(EXPANDING==1){
		getCurrentDEFTAB(line);
	}else{
		readLine(f,line,end_f);
		clearTabs(line);
	}
	printf("\nGETLINE : {%s}\n",line);
	return;
}
int isExistingMacro(char *opcode,int *xstart,int *xend){
	// format of NAMTAB
	// MACRONAME MACROSTART MACROEND
	int end_f;
	FILE *f=initializeFileRead(NAMTAB,&end_f);
	char macroname[MAXMACROSIZE];
	int start,end;
	char line[MAXMACROLINE];
	int found=0;
	while(end_f!=1){
		if(end_f==1){
			(*xstart)=0;
			(*xend)=0;
			break;
		}
		readLine(f,line,&end_f);
		clearTabs(line);
		printf("\nNAMTAB : {%s}\n",line);
		getNAMTABContent(line,macroname,&start,&end);
		if(isEqual(macroname,opcode)==1){
			printf("\nMacro = {%s} to expand !!",macroname);
			(*xstart)=start;
			(*xend)=end;
			printf("\nMACRO START = %d\tMACRO END = %d",*xstart,*xend);
			// fclose(f);
			// return 1;
			found=1;
		}
	}
	fclose(f);
	return found;
}
void setARGTABID(const char *macro){
	FILE *f;
	int end_f;
	int found=0;
	f=initializeFileRead(ARGTAB,&end_f);
	char line[MAXBUFFERSIZE],label[MAXLABEL];
	char opcode[MAXINS],operand[MAXOPERAND],comment[MAXCOMMENTSIZE];
	int count=0;
	int lastPos=0;
	while(1){
		if(end_f==1){
			break;
		}
		readLine(f,line,&end_f);
		getContent(line,label,opcode,operand,comment);
		printf("\nARGTAB : MACRO : {%s}\tVLIST : {%s}",opcode,operand);
		if(isEqual(opcode,macro)==1){
			found=1;
			// printf("\nMACRO FOUND in ARGTAB");
			// break;
			lastPos=count;
		}
		count++;
	}
	if(found!=1){
		printf("\nMACRO NOT FOUND IN ARGTAB !!\n");
		exit(-1);
	}
	ARGTABID/*[0]*/=lastPos;
	printf("\nARGTAB[0] = %d",ARGTABID/*[0]*/);
	return;
}
void processLine(FILE *f,int *end_f,char *line,const char *OUTPUT){
	// 0 means its just an ordinary line
	// line is already there in "line"
	char label[MAXLABEL];
	char opcode[MAXINS];
	char operand[MAXOPERAND];
	char comment[MAXCOMMENTSIZE];
	char outline[MAXBUFFERSIZE];
	int mstart,mend;
	getContent(line,label,opcode,operand,comment);
	printf("\nPROCESSLINE : {%s}\n",line);
	if(isExistingMacro(opcode,&mstart,&mend)==1){
		sprintf(VLIST/*[LEVEL]*/,"%s",operand);
		// printf("\nGOING TO EXPAND MACRO : {%s}\n",opcode);
		printf("\n**ARGLIST : {%s}\n",VLIST/*[LEVEL]*/);
		expand(f,end_f,line,OUTPUT);
	}else{
		if(isEqual(opcode,"MACRO")){
			printf("\nNEW MACRO FOUND !! : {%s}\n",label);
			define(f,end_f,line,OUTPUT);
		}else if(isEqual(operand,"MACRO")){
			printf("\nNEW MACRO FOUND !!(without Argument) : {%s}\n",opcode);
			define(f,end_f,line,OUTPUT);
		}else{
			addLine(OUTPUT,line);
		}
	}
	return;
}
int expand(FILE *f,int *end_f,const char *line,const char *OUTPUT){
	char dline[MAXBUFFERSIZE],dlabel[MAXLABEL];
	char dopcode[MAXINS],doperand[MAXOPERAND],dcomment[MAXCOMMENTSIZE];
	getContent(line,dlabel,dopcode,doperand,dcomment);
	int start,end;
	EXPANDING=1;
	printf("\nEXPAND FOR :{%s}\n",dopcode);
	if(isExistingMacro(dopcode,&start,&end)!=1){
		printf("\nMACRO DEFINITION NOT FOUND !!\n");
		exit(-1);
	}
	char comment[MAXBUFFERSIZE];
	sprintf(comment,"#invoking macro '%s'",dopcode);
	addLine(OUTPUT,comment);
	setARGTABID(dopcode);
	// DEFTAB[LEVEL-1]=start;
	DEFTABID/*[0]*/=start;
	// char line[MAXBUFFERSIZE];
	while(1){
		getLine(f,end_f,dline);
		getContent(dline,dlabel,dopcode,doperand,dcomment);
		if(isEqual(dopcode,"MEND")==1){
			break;
		}
		processLine(f,end_f,dline,OUTPUT);
	}
	EXPANDING=0;
	return 1;
}
int isComment(const char *line){
	if(line[0]=='#'){
		return 1;
	}
	return 0;
}
int lineCount(const char *FILENAME){
	char buffer[MAXBUFFERSIZE];
	FILE *f=fopen(FILENAME,"r");
	int end_f=0;
	int count=0;
	while(1){
		readLine(f,buffer,&end_f);
		if(end_f==1){
			if(buffer[0]!='\0'){
				count++;
			}
			break;
		}
		count++;
	}
	return count;
}
void conCat(char *dline,char *dlabel,char *dopcode,char *doperand){
	dline[0]='\0';
	if(dlabel[0]!='\0'){
		sprintf(dline,"%s",dlabel);
	}
	if(dopcode[0]!='\0'){
		if(dline[0]!=0){
			sprintf(dline,"%s %s",dline,dopcode);
		}else{
			sprintf(dline,"%s",dopcode);
		}
	}
	if(doperand[0]!='\0'){
		if(dline[0]!='\0'){
			sprintf(dline,"%s %s",dline,doperand);
		}else{
			sprintf(dline,"%s",doperand);
		}
	}
	return;
}
int define(FILE *f,int *end_f,const char *line,const char *OUTPUT){
	char dline[MAXBUFFERSIZE];
	char dlabel[MAXLABEL],dopcode[MAXINS],doperand[MAXOPERAND],dcomment[MAXCOMMENTSIZE];
	int start,end;
	getContent(line,dlabel,dopcode,doperand,dcomment);
	int oldMAcro=0;
	// if(isExistingMacro(dlabel,&start,&end)==1){
	// 	oldMAcro=1;
	// }
	start=lineCount(DEFTAB);
	int count=0;
	if(oldMAcro==0){
		printf("\nADDING NEW MACRO :{%s} to DEFTAB",line);
		addLine(DEFTAB,line);
	}
	count++;
	LEVEL=1;
	// char argLine[MAXBUFFERSIZE];
	while(LEVEL>0){
		getLine(f,end_f,dline);
		if(isComment(dline)==1){
			continue;
		}
		getContent(dline,dlabel,dopcode,doperand,dcomment);
		conCat(dline,dlabel,dopcode,doperand);
		if(oldMAcro==0){
			addLine(DEFTAB,dline);
		}
		count++;
		if(isEqual(dopcode,"MACRO")){
			LEVEL++;
		}
		if(isEqual(dopcode,"MEND")){
			LEVEL--;
		}
	}
	NESTED=0;
	end=(start+count-1);
	getContent(line,dlabel,dopcode,doperand,dcomment);
	if(isEqual(dopcode,"MACRO")){
		sprintf(dline,"%s %d %d",dlabel,start,end);
		if(oldMAcro==0){
			addLine(NAMTAB,dline);
		}
		sprintf(dline,"%s %s",dlabel,doperand);
		if(oldMAcro==0){
			addLine(ARGTAB,dline);
		}
	}
	if(isEqual(doperand,"MACRO")){
		sprintf(dline,"%s %d %d",dopcode,start,end);
		if(oldMAcro==0){
			addLine(NAMTAB,dline);
		}
		sprintf(dline,"%s",dopcode);
		if(oldMAcro==0){
			addLine(ARGTAB,dline);
		}	
	}
	// printf("\nMacro DEF completed..\n");
	return 1;
}
int macroDriver(const char *INPUT,const char *OUTPUT){
	initialize_macroDriver();
	resetfile(OUTPUT);
	int end_f;
	FILE *f=initializeFileRead(INPUT,&end_f);
	char line[MAXBUFFERSIZE];
	while(1){
		if(end_f==1){
			printf("\nINPUT completely Read !!\n");
			break;
		}
		getLine(f,&end_f,line);
		printf("\nLINE = {%s}\n",line);
		processLine(f,&end_f,line,OUTPUT);
	}
	fclose(f);
	free_memory();
	return 1;
}
#endif

