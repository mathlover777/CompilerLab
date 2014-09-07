#ifndef PARSERGEN
#define PARSEGREN
#include <stdio.h>
#include <stdlib.h>

#include "fileio.c"
#include "prod.c"
#include "string.c"
#include "generic.c"




const char *GFILE="Grammar.txt";
const char *FFILE="Fst-Fol.txt";
// const char *GFILE="Grammar_backup.txt";
// const char *FFILE="Fst-Fol_backup.txt";
/*************Parser Data****************************/
Prod *prodList;
FirstList *first_prod,*first_NT;
FollowList *follow;
char *Terminals[MAXTERMINALCOUNT];
char *NonTerminals[MAXNONTERMINALCOUNT];
int TCount,NTCount;
int parser[MAXNONTERMINALCOUNT][MAXTERMINALCOUNT];
char start[MAXSYMLEN];
/****************************************************/
char pLine[1000];
int pCount=0;

void getProductions(FILE *fp,int *FILEFLAG){
	createProdList(fp,&prodList,&pCount);
	//fclose(fp);
	return;
}
void getTerminalList(FILE *fp,int *FILEFLAG){
	for(int i=0;i<MAXTERMINALCOUNT;i++){
		Terminals[i]=(char *)malloc(sizeof(char)*MAXSYMLEN);
	}
	readLine(fp,pLine,FILEFLAG);
	lineToWords(pLine,Terminals,&TCount);
	copyString(Terminals[TCount],"$");
	TCount++;
	// printf("\nTERMINAL LIST : \n");
	// for(int i=0;i<TCount;i++){
	// 	printf("{%s}\t",Terminals[i]);
	// }
	// printf("\n");
	return;
}
void getNonTerminalList(FILE *fp,int *FILEFLAG){
	for(int i=0;i<MAXNONTERMINALCOUNT;i++){
		NonTerminals[i]=(char *)malloc(sizeof(char)*MAXSYMLEN);
	}
	readLine(fp,pLine,FILEFLAG);
	lineToWords(pLine,NonTerminals,&NTCount);
	// printf("\nNon-TERMINAL LIST : \n");
	// for(int i=0;i<NTCount;i++){
	// 	printf("{%s}\t",NonTerminals[i]);
	// }
	// printf("\n");
	return;
}
void freeSymbols(){
	for(int i=0;i<MAXTERMINALCOUNT;i++){
		free(Terminals[i]);
	}
	for(int i=0;i<MAXNONTERMINALCOUNT;i++){
		free(NonTerminals[i]);
	}
	return;
}
void freeStringList(char **a,int size){
	for(int i=0;i<size;i++){
		free(a[i]);
		a[i]=0;
	}
	return;
}
void getStartSymbol(FILE *fp,int *endflag){
	readLine(fp,start,endflag);
	return;
}
void initialize_All(){
	/*Getting the Grammar Information from Grammar.txt*/
	int FILEFLAG;
	FILE *fp;
	fp=initializeFileRead(GFILE,&FILEFLAG);
	getProductions(fp,&FILEFLAG);
	getNonTerminalList(fp,&FILEFLAG);
	getTerminalList(fp,&FILEFLAG);
	getStartSymbol(fp,&FILEFLAG);
	fclose(fp);
	/**************************************************/
	

	/*********First Follow Information*****************/

	/**************getting first information for the productions*/
	fp=initializeFileRead(FFILE,&FILEFLAG);
	createSymList(fp,&first_prod,MAXPRODCOUNT,"FIRSTPRODEND");
	/*****************************************************/
	
	/*getting first information for non terminals********/
	createSymList(fp,&first_NT,MAXNONTERMINALCOUNT,"FIRSTNTEND");
	/*****************************************/

	
	createSymList(fp,&follow,MAXNONTERMINALCOUNT,"FOLLOWEND");
	/**************************************************/
	for(int i=0;i<MAXNONTERMINALCOUNT;i++){
		for(int j=0;j<MAXTERMINALCOUNT;j++){
			parser[i][j]=-1;
		}
	}
	fclose(fp);
	return;
}
void freeMemory(){
	freeProdList(&prodList);
	freeSymTable(&first_prod,MAXPRODCOUNT);
	freeSymTable(&first_NT,MAXNONTERMINALCOUNT);
	freeSymTable(&follow,MAXNONTERMINALCOUNT);
	freeSymbols();
	printf("\nMemory Freed\n");
	return;
}
void checkParserData(){
	printf("\nNon Terminals --->\n");
	for(int i=0;i<NTCount;i++){
		printf("{%s}\t", NonTerminals[i]);
	}
	printf("\nTotal = %d",NTCount);
	printf("\nTerminals --->\n");
	for(int i=0;i<TCount-1;i++){
		printf("{%s}\t", Terminals[i]);
	}
	printf("\nTotal = %d",TCount-1);
	printf("\nStart Symbol : {%s}\n",start);
	printf("\nProductions ---->\n");
	for(int i=0;i<pCount;i++){
		printf("\nProduction [%d] --->",i);
		printProd(&(prodList[i]));
	}
	printf("\nFIRST for Productions----\n");
	for(int i=0;i<pCount;i++){
		printf("\nProduction [%d] --->",i);
		printSymList(&(first_prod[i]));
	}
	printf("\nFIRST for Non Terminals----\n");
	for(int i=0;i<NTCount;i++){
		printf("\nNon-TERMINAL [%d] --->",i);
		printSymList(&(first_NT[i]));
	}
	printf("\nGetting Follow for Non Terminals---\n");
	for(int i=0;i<NTCount;i++){
		printf("\nNon-TERMINAL [%d] --->",i);
		printSymList(&(follow[i]));
	}
	return;
}
int getHeadId(int pid){
	for(int i=0;i<NTCount;i++){
		if(isEqual(NonTerminals[i],prodList[pid].head)==1){
			return i;
		}
	}
	return -1;
}
void createParser(){
	printf("\nGenerating Predictive Parser .... ");
	int headId;
	for(int i=0;i<pCount;i++){
		headId=getHeadId(i);
		/************checking for terminals in the first prod***********/
		for(int j=0;j<TCount;j++){
			if(isExist(&(first_prod[i]),Terminals[j])==1){
				if(parser[headId][j]!=-1){
					printf("\nError First-First Conflict in Grammer !!\n");
					exit(-1);
				}
				parser[headId][j]=i;
			}
		}
		/****************************************************************/

		/**************checking for epsilon***********************/
		if(isExist(&(first_prod[i]),"epsilon")==1){
			for(int j=0;j<TCount;j++){
				if(isExist(&(follow[headId]),Terminals[j])==1){
					if(parser[headId][j]!=-1){
						printf("\nError First-Follow Conflict in Grammer !!\n");
						exit(-2);
					}
					parser[headId][j]=i;
				}
			}
		}

	}
}
void showParser(){
	printf("\nThe Predictive Parsing Table ---> \n");
	for(int i=0;i<NTCount;i++){
		for(int j=0;j<TCount;j++){
			printf("%d\t",parser[i][j]);
		}
		printf("\n");
	}
	return;
}
#endif