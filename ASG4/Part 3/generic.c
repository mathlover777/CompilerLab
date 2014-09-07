#ifndef GENERIC
#define GENERIC
int lineToTokens_DFA(const char *line,const char *symtab,const char *outfile,DFA *N,int nfaCount){
	int lbegin=0;
	int bestmatch;
	int match1,match2;
	int *Code=(int *)malloc(sizeof(int)*MAXDFACOUNT);
	if(Code==NULL){
		printf("\nMEmory alloc failed !!");
		exit(-1);
	}
	int *End=(int *)malloc(sizeof(int)*MAXDFACOUNT);
	if(End==NULL){
		printf("\nMEmory alloc failed !!");
		exit(-1);
	}
	attrType *Attrib=(attrType *)malloc(sizeof(attrType)*MAXDFACOUNT);
	if(Attrib==NULL){
		printf("\nMEmory alloc failed !!");
		exit(-1);
	}
	// attrType Attrib[MAXDFACOUNT];
	for(int i=0;i<MAXDFACOUNT;i++){
		Attrib[i].string=(char *)malloc(sizeof(char)*100);
	}
	while(1){
		for(int i=0;i<nfaCount;i++){
			Code[i]=simulate_DFA(&N[i],line,lbegin,&End[i],&Attrib[i]);
			// printf("\nCode[%d] = %d\tEND[%d] = %d",i,Code[i],i,End[i]);
			// if(Code[i]==NOTOK){
			// 	End[i]=-1;
			// }
		}
		int flag=0;
		for(int i=0;i<nfaCount;i++){
			if(Code[i]!=NOTOK){
				flag=1;
				break;
			}
		}
		if(flag!=1){
			printf("\nAll DFA failed !!");
			exit(-1);
		}
		// for(int i=0;i<nfaCount;i++){
		// 	// Code[i]=simulate_DFA(&N[i],line,lbegin,&End[i],&Attrib[i]);
		// 	printf("\nCode[%d] = %d",i,Code[i]);
		// }
		match1=-1;
		match2=-1;
		getBestMatch(End,nfaCount,&match1,&match2);
		if(match2!=-1&&match1!=-1){
			// printf("\nMatch1 = %d TYPE1 = %d\tMatch2 = %d TYPE2 = %d\n",match1,getDFAID(&N[match1]),
			// match2,getDFAID(&N[match2]));
			// printf("\nLEXEME1 = {%s}\tLEXEME2 = {%s}\n",Attrib[match1].string,Attrib[match2].string);
		}
		if(match2==-1&&match1!=-1){
			// printf("\nMatch1 = %d TYPE1 = %d\n",match1,getDFAID(&N[match1]));
			// printf("\nLEXEME1 = {%s}\n",Attrib[match1].string);
		}
		if(match1==-1&&match2==-1){
			printf("\nERROR \n");
			exit(-1);
		}
		if(match2==-1){
			bestmatch=match1;
		}else{
			if(End[match1]>End[match2]){
				bestmatch=match1;
			}else{
				if(getDFAID(&N[match1])==IDNTIFIER){
					bestmatch=match2;
				}else{
					bestmatch=match1;
				}
			}
		}
		// printf("\nbest MAtch =%d",bestmatch);
		// printf("\nCODE 1 = %d CODE2 = %d",Code[match1],Code[match2]);
		// else{
		// 	printf("\nFatal error in matching !!\n");
		// 	exit(-1);
		// }
		// printf("\nBESTMATCH DFA =%d ID = %d\n",bestmatch,Code[bestmatch]);

		printf("\nLEXEME = {%s}\tTYPE = %d\n",Attrib[bestmatch].string,getDFAID(&N[bestmatch]));
		Code[bestmatch]=getDFAID(&N[bestmatch]);
		// lbegin=End[bestmatch];
		switch(Code[bestmatch]){
			case IDNTIFIER:{
				addIdentifier(&Attrib[bestmatch],symtab,outfile);
				break;
			}
			case INT_CONST:{
				add_Numerical(&Attrib[bestmatch],symtab,outfile,INT_CONST);
				break;	
			}
			case FLO_CONST:{
				add_Numerical(&Attrib[bestmatch],symtab,outfile,FLO_CONST);
				break;
			}
			case NOTOK:{
				printf("\nLexeme not detected in any of the NFA !!!\n");
				exit(-1);
				// break;
			}
			default:{
				if((Code[bestmatch]>=422)&&(Code[bestmatch]<=445)){
					// treat lexeme as keyword
					addKeyword(&Attrib[bestmatch],symtab,outfile,Code[bestmatch]);
				}else{
					// treat lexeme as symbol
					add_Symbol(&Attrib[bestmatch],symtab,outfile,Code[bestmatch]);
				}
			}
		}
		printf("\n-----------------\n");
		lbegin=End[bestmatch];
		if(line[lbegin]=='\0'){
			// we have reached the End of the current line
			break;
		}
		if(line[lbegin]==' '){
			lbegin++;
			continue;
		}
	}

	for(int i=0;i<MAXDFACOUNT;i++){
		free(Attrib[i].string);
		Attrib[i].string=NULL;
	}
	free(Attrib);
	free(Code);
	free(End);
}
void printChar(const char *line){
	printf("\nSTART\n");
	while(*line!='\0'){
		printf("%c ASCII = %d$\n",*line,(int)(*line) );
		line++;
	}
	printf("END\n");
	return;
}
void TokenCreator_DFA(const char *source,const char *symtab,const char *outfile,DFA *N,int nfaCount){
	printf("\nFILENAME = '%s'\n",source);
	char *line=(char *)malloc(sizeof(char)*1000);
	char *debug=(char *)malloc(sizeof(char)*1000);
	// char debug[1000];
	FILE *s=fopen(source,"r");
	ENDOFFILE=0;
	while(1){
		line[0]='\0';
		readLine(s,line);
		// printChar(line);
		// printf("%s",line);
		// printf("\n **LINE = { %s }*** \n",line);
		printf("clearing tabs\n");
		clearTabs(line);
		printf("tabs cleared\n");
		sprintf(debug,"\n**********LINE = [%s]**************\n",line);
		printf("%s",debug);
		// printChar(debug);
		// printf("\n%s\n",line );
		if(line[0]=='\0'&&ENDOFFILE==1){
			break;
		}
		if(line[0]=='\0'){
			continue;
		}
		lineToTokens_DFA(line,symtab,outfile,N,nfaCount);
		if(ENDOFFILE==1){
			break;
		}
	}
	fclose(s);
}
void initialize_All(const char *symtab,const char *outfile){
	initialize_symtab(symtab);
	resetfile(outfile);
	ENDOFFILE=0;
	return;
}
void createAllDFA(NFA *N,DFA *D,int dfaCount){
	for(int i=0;i<dfaCount;i++){
		init_DFA(&D[i]);
		NFATODFA(&N[i],&D[i]);
	}
}
void compileWithDFA(const char *source,const char *symtab,const char *outfile){
	printf("\nHERE\n");
	NFA N[100];
	DFA D[100];

	int nfaCount=InitializeAllNFA("regexp.in",3,N);
	printf("\nTotal %d NFA created\n",nfaCount);
	createAllDFA(N,D,nfaCount);
	printf("\nAll DFA created !!");
	TokenCreator_DFA(source,symtab,outfile,D,nfaCount);
}
#endif