#ifndef FILEIO
#define FILEIO
#define STRINGSIZE 1000
// #define FSMCOUNT 7
#define BUFFERSIZE 1000
#define MAXVARSIZE 100
// int ENDOFFILE=0;
void removeCarraigeReturn(char *line){
	int i=0;
	while((int)line[i]!=13){
		// printf("%c\n",line[i]);
		if(line[i]=='\0'){
			// printf("\n7777 End detected...\n");
			// scanf("%c",&b);
			return;
		}
		i++;
	}
	line[i]='\0';
	return;
}
int ENDOFFILE=0;
void readLine(FILE *fp,char *line){
	// memory for line must be already allocated
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			if(x==EOF){
				ENDOFFILE=1;
			}
			line[i]='\0';
			if((int)line[i-1]==13){
				line[i-1]='\0';// taking care of carriage return....os sensitive
			}
			return;
			// break;
		}else{
			line[i]=x;
		}
		x=getc(fp);
		i++;
	}
	// printf("Carriage return call\n");
	// removeCarraigeReturn(line);
	// printf("Carriage return end\n");
	return;
}

void resetfile(const char *x){
	FILE *f=fopen(x,"w");
	fclose(f);
	return;
}

void initialize_symtab(const char *filename){
	// short sizeof int float double bool char signed unsigned for while do
	// return struct const void switch break case break goto long static union
	// default
	FILE *fp=fopen(filename,"w");
	fprintf(fp, "short 0\n");
	fprintf(fp, "sizeof 0\n");
	fprintf(fp, "int 0\n");
	fprintf(fp, "float 0\n");
	fprintf(fp, "double 0\n");
	fprintf(fp, "bool 0\n");
	fprintf(fp, "char 0\n");
	fprintf(fp, "signed 0\n");
	fprintf(fp, "unsigned 0\n");
	fprintf(fp, "for 0\n");
	fprintf(fp, "while 0\n");
	fprintf(fp, "do 0\n");
	fprintf(fp, "return 0\n");
	fprintf(fp, "struct 0\n");
	fprintf(fp, "const 0\n");
	fprintf(fp, "void 0\n");
	fprintf(fp, "switch 0\n");
	fprintf(fp, "break 0\n");
	fprintf(fp, "case 0\n");
	fprintf(fp, "continue 0\n");
	fprintf(fp, "goto 0\n");
	fprintf(fp, "long 0\n");
	fprintf(fp, "static 0\n");
	fprintf(fp, "union 0\n");
	fprintf(fp, "default 0\n");
	fclose(fp);
	return;
}
void getBestMatch(const int *end,int nfaCount,int *match1,int *match2){
	// printf("\nNFA count = %d",nfaCount);
	(*match2)=-1;
	(*match1)=0;
	int match1len,match2len;
	match1len=end[0];
	for(int i=1;i<nfaCount;i++){
		// printf("\ni=%d",i);
		if(end[i]>match1len){
			match2len=match1len;
			(*match2)=(*match1);
			match1len=end[i];
			(*match1)=i;
			continue;
		}
		if(end[i]==match1len){
			match2len=end[i];
			(*match2)=i;
			continue;
		}
	}
	// printf("\nMatch 1=%d match 2 =%d",*match1,*match2);
	return;
}
int SYMTAB_END=0;
void read_symtab(FILE *fp,char *line){
	// memory for line must be already allocated
	// printf("here...in read symtab\n");
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			if(x==EOF){
				SYMTAB_END=1;
			}
			line[i]='\0';
			return;
		}else{
			line[i]=x;
		}
		x=getc(fp);
		i++;
	}
}

void writeline(const char *filename,const char *line){
	// printf("\n*************APPENDING : '%s'  ****\n",line);
	FILE *f=fopen(filename,"a");
	fprintf(f, "%s\n",line);
	fclose(f);
	return;
}
int search_symtab(const char *symtab,const char *query){
	SYMTAB_END=0;
	char symbol[MAXVARSIZE],var[MAXVARSIZE],marker;
	FILE *fp=fopen(symtab,"r");
	int i;
	while(1){
		read_symtab(fp,symbol);
		// printf("%s\n",symbol );
		if(symbol[0]=='\0'&&SYMTAB_END!=1){
			continue;
		}
		if(symbol[0]=='\0'){
			break;
		}
		i=0;
		while(symbol[i]!=' '){
			var[i]=symbol[i];
			i++;
		}
		var[i]='\0';
		// printf("%s\n",var );
		i++;
		marker=symbol[i];
		// printf("%c\n",marker );
		if(isSame(var,query)==1){
			// printf("\nkeyword detected ...\n");
			fclose(fp);
			return ((int)marker-48);		
		}
		if(SYMTAB_END==1){
			break;
		}
	}
	// printf("\nIts a keyword");
	fclose(fp);
	return -1;
}
int getKeywordId(const char *id){
	if(isSame(id,"SHORT")){
		return SHORT;
	}
	if(isSame(id,"INT")){
		return INT;
	}
	if(isSame(id,"FLOAT")){
		return FLOAT;
	}
	if(isSame(id,"DOUBLE")){
		return DOUBLE;
	}
	if(isSame(id,"BOOL")){
		return BOOL;
	}
	if(isSame(id,"CHAR")){
		return CHAR;
	}
	if(isSame(id,"SIGNED")){
		return SIGNED;
	}
	if(isSame(id,"UNSIGNED")){
		return UNSIGNED;
	}
	if(isSame(id,"FOR")){
		return FOR;
	}
	if(isSame(id,"WHILE")){
		return WHILE;
	}
	if(isSame(id,"DO")){
		return DO;
	}
	if(isSame(id,"RETURN")){
		return RETURN;
	}
	if(isSame(id,"STRUCT")){
		return STRUCT;
	}
	if(isSame(id,"CONST")){
		return CONST;
	}
	if(isSame(id,"VOID")){
		return VOID;
	}
	if(isSame(id,"SWITCH")){
		return SWITCH;
	}
	if(isSame(id,"BREAK")){
		return BREAK;
	}
	if(isSame(id,"CASE")){
		return CASE;
	}
	if(isSame(id,"CONTINUE")){
		return CONTINUE;
	}
	if(isSame(id,"GOTO")){
		return GOTO;
	}
	if(isSame(id,"LONG")){
		return LONG;
	}
	if(isSame(id,"STATIC")){
		return STATIC;
	}
	if(isSame(id,"UNION")){
		return UNION;
	}
	if(isSame(id,"DEFAULT")){
		return DEFAULT;
	}
}

int insert_symtab(const char *symtab,const char *id){
	// if string is not present already then it adds that to symtab and returns Identifier
	// if string is a keyword then its respective id is returned 
	char line[100];
	int result=search_symtab(symtab,id);
	// printf("\nInserting '%s' is symtab\n",id);
	if(result==-1){
		// the token is an identifier and not available in symtab
		sprintf(line,"%s 1",id);
		printf("SYMTAB >> '%s'\n",line );
		writeline(symtab,line);
		return IDNTIFIER;
	}
	if(result==0){
		return getKeywordId(id);
	}
}
void addIdentifier_KeyWord(attrType *Attrib,const char *symtab,const char *outfile){
	// printf("\ngoing Inserting '%s' is symtab\n",Attrib->string);
	int result=insert_symtab(symtab,Attrib->string);

	char token[100];
	if(result==IDNTIFIER){
		// means inserted it was a identifier
		sprintf(token,"%d %s",IDNTIFIER,Attrib->string);
		// printf("outfile >> %s",token);
	}else{
		// its a key word
		sprintf(token,"%d %s",result,Attrib->string);
	}
	writeline(outfile,token);
	return;
}
void addIdentifier(attrType *Attrib,const char *symtab,const char *outfile){
	// printf("\ngoing Inserting '%s' is symtab\n",Attrib->string);
	int result=insert_symtab(symtab,Attrib->string);
	char token[100];
	sprintf(token,"%d %s",IDNTIFIER,Attrib->string);
	// printf("outfile >> %s",token);
	writeline(outfile,token);
	return;
}
void addKeyword(attrType *Attrib,const char *symtab,const char *outfile,int code){
	// printf("\ngoing Inserting '%s' is symtab\n",Attrib->string);
	char token[100];
	sprintf(token,"%d %s",code,Attrib->string);
	writeline(outfile,token);
	return;
}
void add_Symbol(attrType *Attrib,const char *symtab,const char *outfile,int code){
	char token[100];
	sprintf(token,"%d",code);
	writeline(outfile,token);
	return;
}
void add_Numerical(attrType *Attrib,const char *symtab,const char *outfile,int code){
	char token[100];
	sprintf(token,"%d %s",code,Attrib->string);
	// printf("\nNumerical Token {%s}\n",token);
	writeline(outfile,token);
	return;
}
#endif
