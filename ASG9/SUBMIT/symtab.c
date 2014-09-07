#ifndef SYMTAB
#define SYMTAB

#include "string.c"
#include "fileio.c"
#include "structures.c"




const char *SYMTABFILE="SYMTAB.txt";

// struct Symelt{
// 	char varname[MAXVARLEN];
// 	char type[10];
// 	int offset;
// };
// typedef struct Symelt Symelt;
// struct Symtable{
// 	Symelt record[MAXSYMTABSIZE];
// 	int size;
// };
// typedef struct Symtable Symtable;
// Symtable SymTab;
void initSymTab(){
	int i=0;
	for(i=0;i<MAXSYMTABSIZE;i++){
		SymTab.record[i].varname[0]='\0';
		SymTab.record[i].type[0]='\0';
		SymTab.record[i].offset=0;
	}
	SymTab.size=0;
}
int insertSymTab(const char *varname,const char *type,int *offset){
	if(MAXSYMTABSIZE==SymTab.size){
		printf("\nERROR NOT ENOUGH SPACE IN SYMTAB");
		return FAILURE;
	}
	int i=0;
	for(i=0;i<SymTab.size;i++){
		if(isEqual(SymTab.record[i].varname,varname)==1){
			printf("\nERROR REDECLARATION OF VARIABLE ={%s}!! ",varname);
			return FAILURE;
		}
	}
	copyString(SymTab.record[SymTab.size].varname,varname);
	copyString(SymTab.record[SymTab.size].type,type);
	int size=0;
	if(isEqual(type,"int")){
		size=INTSIZE;
	}
	if(isEqual(type,"float")){
		size=FLOATSIZE;
	}
	if(isEqual(type,"boolean")){
		size=BOOLEANSIZE;
	}
	if(isEqual(type,"null")){
		size=NULLSIZE;
	}
	if(size==0){
		printf("\nERROR UNKNOWN TYPE {%s}",type);
		return FAILURE;
	}
	SymTab.record[SymTab.size].offset=(*offset);
	(*offset)=(*offset)+size;
	(SymTab.size)++;
	return SUCCESS;
}
int getType(const char *varname,char *type){
	int i=0;
	for(i=0;i<SymTab.size;i++){
		if(isEqual(SymTab.record[i].varname,varname)==1){
			copyString(type,SymTab.record[i].type);
			return SUCCESS;
		}
	}
	type[0]='\0';
	return FAILURE;
}
void saveSymTab(){
	resetfile(SYMTABFILE);
	char Line[1000];
	int i;
	for(i=0;i<SymTab.size;i++){
		sprintf(Line,"%s %s %d",SymTab.record[i].varname,
			SymTab.record[i].type,SymTab.record[i].offset);
		addLine(SYMTABFILE,Line);
	}
	return;
}
void test_symtab(){
	printf("\nTEST");
	return;
}

#endif