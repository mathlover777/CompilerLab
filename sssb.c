#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLABEL 15
#define MAXINS 10
#define MAXADDRESS 4
#define MAXSYMCOUNT 1000
#define	MAXLINESIZE 300
#define MAXCOMMENTSIZE 300
#define MAXOPERAND 15
#define PRINTCONTENT printcontent(label,ins,operand,comment);


// const int ALWAYSINCREMENT=0;// if this flag is set to 1 then then ivalid lines will also be given 3 byte space


struct symelt{
	char *label;
	char *address;
};
typedef struct symelt symelt;
struct SYMTAB{
	symelt *head;
	int maxsize;
	int size;
};
typedef struct SYMTAB SYMTAB;

struct opelt{
	char instruction[5];
	char opcode[3];
};
typedef struct opelt opelt;
opelt OPTAB[5];// the optable ... as its always fixed.. its declared globally


const char *START="START";
const char *END="END";
const char *RESW="RESW";
const char *WORD="WORD";
const char *RESB="RESB";
const char *BYTE="BYTE";
const char *ZERO="0000";

int ENDOFFILE=0;


/****************String functions***********************************/
int isEqual(const char *a,const char *b){
	int i=0;
	while(*(a+i)!='\0'){
		if(*(a+i)!=*(b+i)){
			return 0;
		}
		i++;
	}
	if(*(b+i)=='\0'){
		return 1;
	}
	return 0;
}
int copyString(char *dest,const char *source){
	int i=0;
	while(source[i]!='\0'){
		dest[i]=source[i];
		i++;
	}
	dest[i]='\0';
}
int getSize(char *a){
	int i=0;
	while(a[i]!='\0'){
		// printf("\ni=%d %c",i,a[i]);
		i++;
	}
	return i;
}
void reverse(char *a){
	int i,n;
	n=getSize(a)-1;
	char x;
	i=0;
	while(i<n){
		x=a[i];
		a[i]=a[n];
		a[n]=x;
		i++;
		n--;
	}
	return;
}

	// deletes the contents of label,ins,operand and gives new information in them
	// depending on errors returns many flags
	// 0 means success
	// 1-> more than 3 words
	// 2-> <2 words
	// 3-> nothing or comment
	// 4-> too large words or comment 
void printcontent(char *label,char *ins,char *operand,char *comment){
	printf("\nLABEL = '%s'",label);
	printf("\nINS = '%s'",ins);
	printf("\nOPERAND = '%s'",operand);
	printf("\nCOMMENT = '%s'",comment);
	return;
}
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
	printf("\nWORD SPACE initialised\n");

	int i,s,j,k,large=0;
	i=s=0;

	/******************* EMPTY OR PURE COMMENT **********************/
	if(line[0]=='\0'){
		// nothing written on the line
		printf("\nEMPTY LINE\n");
		return 3;
	}
	if(line[0]=='#'){
		// line is a pure comment
		printf("\nPURE COMMENT\n");
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
	printf("\nGOING to count number of spaces \n");
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
	printf("\nNUMBER of SPACES = %d\n",s);
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
	printf("\nNUMBER of words = %d\n",s);
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

/*******************************************************************/
/******************* HEX functions **********************************/
int isHex(char *a){
	// returns 1 if string is a valid hex
	// 0 otherwise
	// returns true ie 1 if "\0" is the input
	int i=0;
	while(a[i]!='\0'){
		switch(a[i]){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':break;
			default:return 0;
		}
		i++;
	}
	return 1;
}
int incrementByte(char *a){
	// increments the current number by 1 only [in hex]
	// returns 1 if success
	// returns 0 if overflow
	// size is assumed to MAXADDRESS
	int carry=1,i,sum;
	printf("\nIB_LC = '%s'\n",a);
	for(i=0;i<MAXADDRESS;i++){
		if(carry==0){
			printf("\nIB_A_LC = '%s'\n",a);
			return 0;
		}
		if((a[i]<=56&&a[i]>=48)||(a[i]>=65&&a[i]<=69)){
			a[i]++;
			carry=0;
			continue;
		}
		if(a[i]=='9'){
			a[i]=65;
			carry=0;
			continue;
		}
		if(a[i]==70){
			a[i]='0';
			carry=1;
			continue;
		}
	}
	printf("\nIB_A_LC = '%s'\n",a);
	if(carry==0){
		return 0;
	}
	return 1;
}
int hexToDec(char *a){
	printf("\nHEX = '%s'\n",a);
	int n=0,i=0,p=1;
	int d;
	while(a[i]!='\0'){
		if(a[i]<=57&&a[i]>=48){
			d=a[i]-48;
		}else{
			d=a[i]-55;
		}
		n=n+p*d;
		p=p*16;
		i++;
	}
	return n;
}

/********************************************************************/
/*****************FILE read write functions**************************/
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
			return;
		}else{
			line[i]=x;
		}
		x=getc(fp);
		i++;
	}
}
void clearTabs(char *line){
	// removes spaces and tabs from line
	// like "   \t  bbbb   \t\t  rrrr \t  ggg\t  "--->"bbbb rrrr ggg"
	int i,j;
	// convert all tabs into spaces
	while(line[i]!='\0'){
		if(line[i]=='\t'){
			line[i]=' ';
		}
		i++;
	}
	// all tabs are replaced by spaces,,,now we will remove redundant spaces[inplace]
	i=j=0;
	while(line[i]==' '){
		i++;
	}
	while(1){
		if(line[i]=='\0'){
			break;
		}
		if(line[i]!=' '){
			line[j]=line[i];
			j++;
			i++;
			continue;
		}
		line[j]=' ';
		i++;
		j++;
		while(line[i]==' '){
			i++;
		}
	}
	if(j-1>=0){
		if(line[j-1]==' '){
			line[j-1]='\0';
		}
	}
	line[j]='\0';
	return;
}
void resetfile(){
	FILE *f=fopen("intermediate","w");
	fclose(f);
	return;
}
void writeline(char *line){
	printf("\n*************APPENDING : '%s'  ****\n",line);
	FILE *f=fopen("intermediate","a");
	fprintf(f, "%s\n",line);
	fclose(f);
	return;
}
/********************************************************************/
/****************SYMTAB FUNCTIONS***********************************/
void initSYMTAB(SYMTAB *s,int max){
	s->head=(symelt *)malloc(sizeof(symelt)*(max));
	s->maxsize=max;
	s->size=0;
	return;
}
void initOPTAB(){
	/*********************** OPCODE ********************************/
	strcpy(OPTAB[0].instruction,"LDA");
	strcpy(OPTAB[0].opcode,"00");
	strcpy(OPTAB[1].instruction,"MUL");
	strcpy(OPTAB[1].opcode,"20");
	strcpy(OPTAB[2].instruction,"DIV");
	strcpy(OPTAB[2].opcode,"24");
	strcpy(OPTAB[3].instruction,"STA");
	strcpy(OPTAB[3].opcode,"0C");
	strcpy(OPTAB[4].instruction,"ADD");
	strcpy(OPTAB[4].opcode,"18");
	/***************************************************************/
}
void deleteSYMTAB(SYMTAB *s){
	free(s->head);
	s->head=NULL;
	s->maxsize=0;
	s->size=0;
	return;
}
char* searchSYMTAB(SYMTAB *s,char *label){
	for(int i=0;i<s->size;i++){
		if(isEqual(s->head[i].label,label)==1){
			return s->head[i].address;
		}
	}
	return NULL;
}
void printSYMTAB(const SYMTAB *s){
	printf("\n****************************************\n");
	printf("\nSIZE = %d",s->size);
	for(int i=0;i<s->size;i++){
		printf("\n%s\t%s",s->head[i].label,s->head[i].address);
	}
	printf("\n****************************************\n");
}
int isExistSYMTAB(const SYMTAB *s,char *label){
	for(int i=0;i<s->size;i++){
		if(isEqual(s->head[i].label,label)==1){
			return 1;
		}
	}
	printSYMTAB(s);
	return 0;
}
int insertSYMTAB(SYMTAB *s,char *label,char *address){
	printf("\n%%%%%%%%%%%%%%%% in insertion just before insering ////////////");
	printSYMTAB(s);
	s->head[s->size].label=(char *)malloc(sizeof(char)*(MAXLABEL+1));
	s->head[s->size].address=(char *)malloc(sizeof(char)*(MAXADDRESS+1));
	copyString(s->head[s->size].label,label);
	copyString(s->head[s->size].address,address);
	(s->size)++;
	printSYMTAB(s);
	return 1;
}
void storeSYMTAB(SYMTAB *s){
	FILE *f=fopen("SYMTAB","w");
	fprintf(f, "%d %d\n",s->maxsize,s->size);
	for(int i=0;i<s->size;i++){
		fprintf(f, "%s %s\n",s->head[i].label,s->head[i].address);
	}
	return;
}

/********************************************************************/

/*******************OPTAB FUNCTIONS*************************************/
char* getOpcode(char *instruction){
	int i=0;
	for(i=0;i<5;i++){
		if(isEqual(OPTAB[i].instruction,instruction)==1){
			return OPTAB[i].opcode;
		}
	}
	return NULL;
}
/***********************************************************************/
/*************defintions**********************/
int pass1(FILE *fp,SYMTAB *S);
/*********************************************/
int main(int argc,char **argv){
	// argv[1] will be the file name
	// argv[2] will be the name of the outfile
	// usage "./sssb input output"
	system("reset");
	FILE *fp=fopen(argv[1],"r");
	if(fp==NULL){
		printf("\nFATAL ERROR : input file %s not found !! ",argv[1]);
	}else{
		printf("\nFILE found !!");
	}
	printf("\npreparing pass1");
	SYMTAB S;
	initOPTAB();
	initSYMTAB(&S,MAXSYMCOUNT);
	pass1(fp,&S);
	printf("\n");
	return 0;
}
int pass1(FILE *fp,SYMTAB *S){
	// 0 - > success
	// 1 - > file does not have END
	// 2 - > END does not have label
	// 3 - > END label does not exist in SYMTAB
	// 4 - > out of memory
	// 5 -> bad starting address
	// 6 - > filename not given
	// 9 - > cant create intermediate file
	resetfile();
	//FILE *f=fopen("intermediatecode","w");
	// if(f==NULL){
	// 	printf("\nFATAL ERROR : cant create intermediate file !!! ");
	// 	return 9;
	// }else{
	// 	printf("\nintermediate file initiated\n");
	// }
	char line[MAXLINESIZE];
	char outline[1000];
	char label[MAXLABEL+1],ins[MAXINS+1],address[MAXADDRESS+1],comment[MAXCOMMENTSIZE+1],operand[MAXOPERAND+1];
	char LC[5],LCC[5];
	copyString(LC,ZERO);
	line[0]='\0';
	label[0]='\0';
	ins[0]='\0';
	address[0]='\0';
	comment[0]='\0';
	int flag,noEOF=0,finalflag;
	int addressoverflow=0;
	int resw,i;
	int nhex=0,duplicatelabel=0;
	char *opcode,*startAddress,enddetected=0,startfound=0;
	int found,length=0,b;
	/*********************Detect Start**********************************/
	readLine(fp,line);
	clearTabs(line);
	printf("\nline = '%s'\n",line);
	flag=getContent(line,label,ins,operand,comment);
	PRINTCONTENT
	if(strcmp(ins,START)==0){
		startfound=1;
		if(label[0]=='\0'){
			printf("\nFATAL ERROR : No program name provided at start");
		}
		printf("\nStart detected !!! Starting address = %s",operand);
		printf("\nProgram name = %s ",label);
		if(getSize(operand)>4){
			printf("\nFATAL ERROR : Starting address cant be %d [cant be more than 2 bytes] !!! ",getSize(operand));
			return 2;
		}
		if(isHex(operand)==0){
			printf("\nFATAL ERROR : Non Hex starting address reference !!! ");
			return 2;
		}
		copyString(LC,operand);
		sprintf(outline,"0000 %s %s %s %d",label,ins,operand,flag);
		printf("\nOUTLINE = '%s'",outline);
		//fprintf(f,"%s\n",outline);
		writeline(outline);
		reverse(LC);
	}
	copyString(LCC,LC);
	// now LCC and LC have hex values 

	// read the rest of the program
	while(1){
		length++;
		if(startfound==1){
			readLine(fp,line);
			clearTabs(line);
		}else{
			startfound=1;
		}
		printf("\nLINE= '%s'\n",line);
		flag=getContent(line,label,ins,operand,comment);
		PRINTCONTENT
		// scanf("%d",&b);
		/******************* Check for END **************************************/
		if(strcmp(ins,END)==0){
			// we have reached the end of the program
			enddetected=1;
			printf("\nXXXXXXXXXXXX   END DETECTED XXXXXXXXXXXX\n");
			break;
		}
		if(ENDOFFILE==1){
			noEOF=1;
			break;
		}
		/************************************************************************/

		/******************** Check for comment *********************************/
		if(flag==3){
			// we have a blank line or comment.....write whatever there is in the comment to the output file after a #
			//fprintf(f,"#%s\n",comment);
			sprintf(outline,"#%s",comment);
			writeline(outline);
			continue;
		}
		/************************************************************************/

		/**************** check if there is label ******************************/
		duplicatelabel=0;
		if(label[0]!='\0'){
			printf("\nLABEL FOUND...going to lookup in symbol table !!");
			printSYMTAB(S);
			if(isExistSYMTAB(S,label)==0){
				// label not present in the symbol table
				printf("\nLABEL = '%s' not in SYMTAB ",label);
				reverse(LCC);
				printSYMTAB(S);
				insertSYMTAB(S,label,LCC);
				reverse(LCC);
			}else{
				// label already present in the symbol table
				printf("\nERROR : label = %s already present in the address table !! still attempting compilation ",label);
				duplicatelabel=1;// means duplicate label
			}
		}
		/************************************************************************/

		/****************** Check for instructions **********************************/
		printf("\nGoing to look for INS = '%s'\n",ins);
		if(ins[0]!='\0'){
			found=0;
			opcode=getOpcode(ins);
			if(opcode!=NULL){
				// we have found a opcode
				printf("\nOPCODE FOUND --> '%s' --> '%s'\n",ins,opcode);
				found=1;
				addressoverflow=incrementByte(LC);
				if(addressoverflow==1){
					printf("\nFATAL ERROR : out of memory !!! ");
					return 4;
				}
				addressoverflow=incrementByte(LC);
				if(addressoverflow==1){
					printf("\nFATAL ERROR : out of memory !!! ");
					return 4;
				}
				addressoverflow=incrementByte(LC);
				if(addressoverflow==1){
					printf("\nFATAL ERROR : out of memory !!! ");
					return 4;
				}

			}else{
				// ins is not a valid opcode...may be error or an assembler directive
				// see if its an assembler directive
				printf("\nINS = '%s' not present in OPTAB\n",ins);
				/***************Check if ins = WORD ****************************************/
				if(isEqual(ins,WORD)==1){
					printf("\n'WORD' detected \n");
					found=1;
					addressoverflow=incrementByte(LC);
					if(addressoverflow==1){
						printf("\nFATAL ERROR : out of memory !!! ");
						return 4;
					}
					addressoverflow=incrementByte(LC);
					if(addressoverflow==1){
						printf("\nFATAL ERROR : out of memory !!! ");
						return 4;
					}
					addressoverflow=incrementByte(LC);
					if(addressoverflow==1){
						printf("\nFATAL ERROR : out of memory !!! ");
						return 4;
					}
				}
				/***************************************************************************/
				
				/******************Check if ins = RESW ***************************************/
				if(isEqual(ins,RESW)==1){
					printf("\nRESW detected\n");
					found=1;
					nhex=0;
					if(isHex(operand)==1){
						resw=hexToDec(operand);
						if(resw==0){
							printf("\nConverting 0 hex to 1");
							resw=1;
						}
						printf("\nresw = %d \n",resw);
						/**************incrementing location counter by res*3******************/
						for(i=1;i<=resw;i++){
							/************ incrementing location counter by 3 ******************/
							addressoverflow=incrementByte(LC);
							if(addressoverflow==1){
								printf("\nFATAL ERROR : out of memory !!! ");
								return 4;
							}
							addressoverflow=incrementByte(LC);
							if(addressoverflow==1){
								printf("\nFATAL ERROR : out of memory !!! ");
								return 4;
							}
							addressoverflow=incrementByte(LC);
							if(addressoverflow==1){
								printf("\nFATAL ERROR : out of memory !!! ");
								return 4;
							}
							/****************************************************************/
						}
						/********************************************************************/
					}else{
						nhex=1;
						printf("\nWARNING : non hex numerical !![reserving 3 bytes only]");
						resw=1;
						/************ incrementing location counter by 3 ******************/
						addressoverflow=incrementByte(LC);
						if(addressoverflow==1){
							printf("\nFATAL ERROR : out of memory !!! ");
							return 4;
						}
						addressoverflow=incrementByte(LC);
						if(addressoverflow==1){
							printf("\nFATAL ERROR : out of memory !!! ");
							return 4;
						}
						addressoverflow=incrementByte(LC);
						if(addressoverflow==1){
							printf("\nFATAL ERROR : out of memory !!! ");
							return 4;
						}
						/****************************************************************/
					}
				}
				/********************************************************************************/

				/***********************Check if ins = RESB **************************************/
				if(isEqual(ins,RESB)==1){
					printf("\nRESB detected \n");
					found=1;
					nhex=0;
					if(isHex(operand)==1){
						resw=hexToDec(operand);
						if(resw==0){
							resw=1;
						}
						/**************incrementing location counter by res*3******************/
						for(i=1;i<=resw;i++){
							/************ incrementing location counter by 1 ******************/
							addressoverflow=incrementByte(LC);
							if(addressoverflow==1){
								printf("\nFATAL ERROR : out of memory !!! ");
								return 4;
							}
							/****************************************************************/
						}
						/********************************************************************/
					}else{
						nhex=1;
						printf("\nWARNING : non hex numerical !![reserving 1 bytes only]");
						resw=1;
						/************ incrementing location counter by 1 ******************/
						addressoverflow=incrementByte(LC);
						if(addressoverflow==1){
							printf("\nFATAL ERROR : out of memory !!! ");
							return 4;
						}
						/****************************************************************/
					}
				}
				/**********************************************************************************/

				/*************************check if ins = BYTE **********************************/
				// CURRENT VERSION HAS NO SUPPORT FOR BYTE

				/********************************************************************************/
			}
		}else{
			found=0;
		}
		if(found==0){
			// instruction does not found in inventory !!
			// if ALWAYSINCREMENT = 1 then allocating 3 byte space
			if(ALWAYSINCREMENT==1){
				addressoverflow=incrementByte(LC);
				if(addressoverflow==1){
					printf("\nFATAL ERROR : out of memory !!! ");
					return 4;
				}
				addressoverflow=incrementByte(LC);
				if(addressoverflow==1){
					printf("\nFATAL ERROR : out of memory !!! ");
					return 4;
				}
				addressoverflow=incrementByte(LC);
				if(addressoverflow==1){
					printf("\nFATAL ERROR : out of memory !!! ");
					return 4;
				}
			}
		}


		/****************** now whatever we have found , to be written in the intermediate file***********/
		if(label[0]=='\0'){
			label[0]='!';// in the intermediate file there will be no empty field,..atleast dummies by "!"
			label[1]='\0';
		}
		if(ins[0]=='\0'){
			ins[0]='!';
			ins[1]='\0';
		}
		if(operand[0]=='\0'){
			operand[0]='!';
			operand[1]='\0';
		}
		finalflag=0;
		if(found==0){
			finalflag=1;// instruction not found
		}
		if(duplicatelabel==1){
			finalflag=2;// label already exists in SYMTAB
		}
		if(nhex==1){
			finalflag=3;// non hex numerical or address reference
		}
		if(flag==1){
			finalflag=4;// too few words
		}
		if(flag==2){
			finalflag=5;// too many words
		}
		if(flag==4){
			finalflag=6;// too large words
		}
		reverse(LCC);
		sprintf(outline,"%s %s %s %s %d",LCC,label,ins,operand,finalflag);
		copyString(LCC,LC);
		printf("\nOUTLINE = '%s'\n",outline);
		writeline(outline);
		// fprintf(f,"%s\n",outline );
		/****************************************************************************/
	}
	printSYMTAB(S);
	printf("\nout of file reading loop !!\n");
	//fclose(f);
	//exit(-1);
	if(noEOF==1&&enddetected==0){
		printf("\nFATAL ERROR : No END found at the end of the program !! ");
		return 1;
	}
	// here means END is found
	if(operand==NULL){
		printf("\nFATAL ERROR : END label does not have operand !! ");
		return 2;
	}else{
		startAddress=searchSYMTAB(S,operand);
		if(startAddress==NULL){
			printf("\nFATAL ERROR : END operand does not exist in program !! ");
			return 3;
		}
		if(label[0]=='\0'){
			label[0]='!';
			label[1]='\0';
		}
		reverse(LCC);
		sprintf(outline,"%s %s END %s 0",LCC,label,startAddress);
		reverse(LCC);
		printf("\noutline = '%s' \n",outline );
		// if(f==NULL){
		// 	exit(-1);
		// }
		writeline(outline);
		// fprintf(f, "%s\n",outline );
		//fclose(f);
		//exit(-1);
	}

	// fclose(f);

	/*******************************************************************/
}
void pass2(){

}