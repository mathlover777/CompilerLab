#include "string.c"
#include "fileio.c"
#ifndef MACRO_C
#define MACRO_C
#define MAXMACROSIZE 20
#define MAXMACROLINE 100
const char* NAMTAB="NAMTAB.txt"; 
int getContent(char *line,char *label,char *ins,char *operand,char *comment){
	// deletes the contents of label,ins,operand and gives new information in them
	// depending on errors returns many flags
	// 0 means success
	// 1-> more than 3 words
	// 2-> <2 words
	// 3-> nothing or comment
	// 4-> too large words or comment 
	label[0]='\0';
	ins[0]='\0';
	operand[0]='\0';
	comment[0]='\0';
	// printf("\nWORD SPACE initialised\n");

	int i,s,j,k,large=0;
	i=0;
	while(line[i]!='\0'){
		if(line[i]=='!'){
			rprintf("\nFATAL ERROR : Invalid Character : '!' not allowed !!");
			exit(-1);
		}
		i++;
	}
	i=s=0;

	/******************* EMPTY OR PURE COMMENT **********************/
	if(line[0]=='\0'){
		// nothing written on the line
		printf("\nEMPTY LINE");
		return 3;
	}
	if(line[0]=='#'){
		// line is a pure comment
		// printf("\nPURE COMMENT\n");
		i=0;
		i++;
		while(line[i]!='\0'){
			if(i-1>=MAXCOMMENTSIZE){
				// comment size increases the maximum comment buffer size
				// but as its a comment no action will be taken
				break;
			}
			comment[i-1]=line[i];
			i++;
		}
		comment[i-1]='\0';
		return 3;
	}
	/*****************************************************************/
	// printf("\nGOING to count number of spaces \n");
	/***************** COUNTING NUMBER OF SPACES************************/
	s=0;
	i=0;
	while(line[i]!='\0'&&line[i]!='#'){
		if(line[i]==' '){
			s++;
		}
		i++;
	}
	/****************************************************************/
	// printf("\nNUMBER of SPACES = %d\n",s);
	/***********if # is found replace it by \0 and take the content after that as comment*****/
	if(line[i]=='#'){
		line[i]='\0';
		j=i+1;
		k=0;
		while(line[j]!='\0'){
			if(k>=MAXCOMMENTSIZE){
				break;
			}
			comment[k]=line[j];
			k++;
			j++;
		}
		comment[k]='\0';
	}
	/****************************************************************************************/

	/*************** now s contains the number of spaces ie words-1 ************************/
	s++;// now s=# of words
	// printf("\nNUMBER of words = %d\n",s);
	if(s>3){
		return 1;
	}
	if(s==1){
		// if there is only one word we assume it to be a instruction
		i=0;
		while(line[i]!='\0'){
			if(i>=MAXINS){
				large=1;
				break;
			}
			ins[i]=line[i];
			i++;
		}
		ins[i]='\0';
		if(large==1){
			return 4;
		}
		return 2;
	}
	/*************now the line has either 2 or 3 words***************************/
	i=0;
	if(s==3){
		// the first thing is a label
		j=0;
		while(line[i]!=' '){
			if(j>=MAXLABEL){
				large=1;
				break;
			}
			label[j]=line[i];
			i++;
			j++;
		}
		label[j]='\0';
		if(large==1){
			return 4;
		}
		i++;
		// now i points to the start of the instruction
	}
	if(s==2||s==3){
		// we will have a instruction and an operand
		// we will have instruction before the first space we encounter
		// we will have the word after the space as operand

		j=0;
		while(line[i]!=' '){
			if(j>=MAXINS){
				large=1;
				break;
			}
			ins[j]=line[i];
			i++;
			j++;
		}
		ins[j]='\0';
		if(large==1){
			return 4;
		}
		i++;
		j=0;
		while(line[i]!='\0'){
			if(j>=MAXOPERAND){
				large=1;
				break;
			}
			operand[j]=line[i];
			i++;
			j++;
		}
		operand[j]='\0';
	}
	if(large==1){
		return 4;
	}
	return 0;
}
int getNAMTABContent(const char *line,char *macroname,int *start,int *end){
	if(line[i]=='\0'){
		return 0;
	}
	int i;
	while(1){
		if(line[i]==' '){
			break;
		}
		macroname[i]=line[i];
		i++;
	}
	macroname[i]='\0';
	i++;
	int j=i;
	while(line[j]!=' '){
		j++;
	}
	(*start)=strToDec2(line,i,j-1);
	i=j+1;
	j=i;
	while(line[j]!='\0'){
		j++;
	}
	(*end)=strToDec2(line,i,j-1);
	return 1;
}
int macroDriver(const char *INPUT,const char *OUTPUT){
	resetfile(OUTPUT);
	int end_f;
	FILE *f=initializeFileRead(INPUT,&end_f);
	char line[MAXBUFFERSIZE];
	while(1){
		if(end_f==1){
			printf("\nINPUT completely Read !!\n");
			break;
		}
		readLine(f,line,&end_f);
		clearTabs(line);
		processLine(line,INPUT,OUTPUT);
	}
	fclose(f);
	return 1;
}
int isExistingMacro(char *opcode,int *xstart,int *xend){
	// format of NAMTAB
	// MACRONAME MACROSTART MACROEND
	int end_f;
	FILE *f=initializeFileRead(NAMTAB,&end_f);
	char macroname[MAXMACROSIZE];
	int start,end;
	char line[MAXMACROLINE];
	while(end_f!=1){
		readLine(f,line,&end_f);
		clearTabs(line);
		getNAMTABContent(line,macroname,&start,&end);
		if(isEqual(macroname,opcode)==1){
			printf("\nMacro = {%s} to expand !!",macroname);
			(*xstart)=start;
			(*xend)=end;
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}
int expandMacro(const char *opcode,const char *operand,const char *OUTPUT){

	return 1;
}
int defineMacro(const char *macroname,const char *operand){
	

	return 1;
}
int processLine(char *line,const char *INPUT,const char *OUTPUT){
	// 0 means its just an ordinary line
	char label[MAXLABEL];
	char opcode[MAXINS];
	char operand[MAXOPERAND];
	char comment[MAXCOMMENTSIZE];
	char outline[MAXBUFFERSIZE];
	int mstart,mend;
	getContent(line,label,opcode,operand,comment);
	if(!isEqual(opcode,"MACRO")){
		if(isExistingMacro(opcode,&mstart,&mend)==1){
			printf("\nMacro Definition to EXpand !!");
			expandMacro(opcode,operand,OUTPUT);
		}else{
			copyString(outline,line);
			addLine(OUTPUT,outline);
			return 0;
		}
	}
	printf("\nNew Macro Definition {%s} found !!\n");
	defineMacro(label,operand);
}
#endif

