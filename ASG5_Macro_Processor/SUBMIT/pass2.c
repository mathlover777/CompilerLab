#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLABEL 15
#define MAXINS 10
#define MAXADDRESS 4
#define ADDRESS 4
#define MAXSYMCOUNT 1000
#define	MAXLINESIZE 300
#define MAXCOMMENTSIZE 300
#define MAXOPERAND 15
#define PRINTCONTENT printcontent(label,ins,operand,comment);

const int ENABLECOLOR=0;
         
struct symelt{                                 
	char *label;
	char *address;
};

typedef struct symelt symelt;
struct SYMTAB{                       //Stores elements of symbol table
	symelt *head;
	int maxsize;
	int size;
};
typedef struct SYMTAB SYMTAB;

struct opelt{                      //Optab Elements
	char instruction[5];
	char opcode[3];
};

struct iline{                      //Stucture to get contents of every line from the intermediate file
	char iaddress[5];
	char ilabel[16];
	char iinstruction[6];
	char ioperand[16];
	char ierror[21];
};

struct isymbol{                    //Structure to get the Symbols from Symbol table file
	char islabel[16];
	char isaddress[5];
};
typedef struct opelt opelt;
opelt OPTAB[5];


const char *START="START";
const char *END="END";
const char *RESW="RESW";
const char *WORD="WORD";
const char *RESB="RESB";
const char *BYTE="BYTE";
const char *ZERO="0000";

int ENDOFFILE=0;


/*             Our Handling String FUNCTIONS  						*/
int isEqual(const char *a,const char *b)
{
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
void copyString(char *dest,const char *source)
{
	int i=0;
	while(source[i]!='\0'){
		dest[i]=source[i];
		i++;
	}
	dest[i]='\0';
}
void copyword(const char *source,char* dest)
{
	int l=strlen(source);
	int j;
	for(j=0;l-j-1>=0;j++)
	{
		dest[5-j]=source[l-j-1];
	}
	for(;5-j>=0;j++)
	{
		dest[5-j]='0';
	}
	dest[6]='\0';
}
int getSize(char *a)                                       
{
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
void copyStringat(const char *source,char* dest,int start,int end)     // Copies string from source to dest only from index start to end of dest Cuts other characters
{
	int i;
	int j;
	for(i=start,j=0;i!=(end+1) && source[j]!='\0';i++,j++)
	{
		dest[i]=source[j];
	}
	for(;i!=(end+1);i++)
	{
		dest[i]=' ';
	}
}
/* 				FILE read write FUNCTIONS 				*/
int readLine(FILE *fp,char *line){
	// memory for line must be already allocated
	ENDOFFILE=0;
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			if(x==EOF){
				ENDOFFILE=1;
				line[i]='\0';
				return 1;
			}
			line[i]='\0';
			return 0;
		}else{
			line[i]=x;
		}
		x=getc(fp);
		i++;
	}
}

void writeline(FILE* fp,char *line){
	fprintf(fp, "%s\n",line);
	return;
}
/*   		SYMTAB functions   			*/
void initSYMTAB(SYMTAB*s,int max){
	s->head=(symelt *)malloc(max*sizeof(symelt));
	s->maxsize=max;
	s->size=0;
}
void deleteSYMTAB(SYMTAB *s){
	free(s->head);
	s->head=NULL;
	s->maxsize=0;
	s->size=0;
	return;
}
int searchSYMTAB(SYMTAB *s,char *label,char *tempaddress){
	for(int i=0;i<s->size;i++){
		if(isEqual(s->head[i].label,label)==1){
			copyString(tempaddress,s->head[i].address);
			return 1;
		}
	}
	return 0;
}
void printSYMTAB(const SYMTAB *s){
	printf("\n      PRINTING SYMBOL TABLE   \n");
	printf("\n          SIZE = %d    ",s->size);
	for(int i=0;i<s->size;i++){
		printf("\n%s %s",s->head[i].label,s->head[i].address);
	}
	printf("\n \n");
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
	//printSYMTAB(s);
	
	s->head[s->size].label=(char *)malloc(sizeof(char)*(MAXLABEL+1));
	s->head[s->size].address=(char *)malloc(sizeof(char)*(MAXADDRESS+1));
	copyString(s->head[s->size].label,label);
	copyString(s->head[s->size].address,address);
	(s->size)++;
	//printSYMTAB(s);
	
	return 1;
}

/*				HEX functions 										*/
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
	// printf("\nIB_LC = '%s'\n",a);
	for(i=0;i<MAXADDRESS;i++){
		if(carry==0){
			// printf("\nIB_A_LC = '%s'\n",a);
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
	// printf("\nIB_A_LC = '%s'\n",a);
	if(carry==0){
		return 0;
	}
	return 1;
}
int hexToDec(char *a){
	// printf("\nHEX = '%s'\n",a);
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
int getdec(char* str)
{
	int ret=0;
	int i;
	int j;
	for(j=0;j<strlen(str);j++)
	{
		ret=10*ret+(str[j]-48);
	}
	return ret;
}
char* decToHex(int decimalNumber)
{
	int remainder,quotient;
    int i=1,j,temp;
    int k=0;
    char hexadecimalNumber[7];
    quotient = decimalNumber;
    char* output;
    output=(char*)malloc(7*sizeof(char));
    for(j=0;j<6;j++) output[j]=' ';
    output[6]='\0';
    while(quotient!=0){
         temp = quotient % 16;
      
      //To convert integer into character
      if( temp < 10)
           temp =temp + 48;
      else
         temp = temp + 55;

      hexadecimalNumber[i++]= temp;
      quotient = quotient / 16;
    }
    for(k=0;k<6;k++) output[k]='0';
    k=0;
	//printf("\nHEX:");
    for(j = i -1 ;j> 0;j--)
    {
      output[6-(i-1)+k]=hexadecimalNumber[j];
      k++;
      //printf("%c",hexadecimalNumber[j]);
    }
    //printf("\nOUTPUT:%s",output);
    return output;
}
void rprintf(const char *msg){
	//printf("\nCALLED :%s",msg);
    if(ENABLECOLOR==1){
        printf("\033[31m\033[40m%s\033[0m",msg);
    }else{
        printf("%s",msg);
    }
    return;
}
/*					OPTAB functions 				*/
int getOpcode(char *instruction,char *tempopcode){
	int i=0;
	for(i=0;i<5;i++){
		if(isEqual(OPTAB[i].instruction,instruction)==1){
			copyString(tempopcode,OPTAB[i].opcode);
			return 1;
		}
	}
	return 0;
}

void makestring(char* in,char* out,int start,int end)
{
	out=(char*)malloc((out-in+1)*sizeof(char));
	int i;
	for(i=start;i<=end;i++)
	{
		out[i-start]=in[i];
	}
}
int split(char *bline,iline* temp)
{
	if(bline[0]=='#') return 0;//Line is a comment Skip in processing
	int i,j;
	for(i=0;bline[i]!=' ';i++)
	{
		temp->iaddress[i]=bline[i];
	}
	temp->iaddress[i]='\0';
	i++;
	j=i;
	for(;bline[i]!=' ';i++)
	{
		temp->ilabel[i-j]=bline[i];
	}
	temp->ilabel[i-j]='\0';
	i++;
	j=i;
	for(;bline[i]!=' ';i++)
	{
		temp->iinstruction[i-j]=bline[i];
	}
	temp->iinstruction[i-j]='\0';
	i++;
	j=i;
	for(;bline[i]!=' ';i++)
	{
		temp->ioperand[i-j]=bline[i];
	}
	temp->ioperand[i-j]='\0';
	i++;
	j=i;	
	for(;bline[i]!='\0';i++)
	{
		temp->ierror[i-j]=bline[i];
	}
	temp->ierror[i-j]='\0';
	return 1;
}
void splitsymbol(char *bline,isymbol* temp)
{
	int i,j;
	for(i=0;bline[i]!=' ';i++)
	{
		temp->islabel[i]=bline[i];
	}
	temp->islabel[i]='\0';
	i++;
	j=i;
	for(;bline[i]!='\0';i++)
	{
		temp->isaddress[i-j]=bline[i];
	}
	temp->isaddress[i-j]='\0';
}
void printinstruction(iline in)
{
	printf("\n%s %s %s %s %s",in.iaddress,in.ilabel,in.iinstruction,in.ioperand,in.ierror);
}
void init(char* str,int length)
{
	int i;
	for(i=0;i<length-1;i++) str[i]=' ';
	str[1]='0';
	str[2]='0';
	str[i]='\0';
}
void senderror(char e,int k)
{
	char strerr[1000];
	//printf("\n ERROr");
	if(e=='1')
	{
		sprintf(strerr,"\nError in Line Number %d :Instruction Not found\n",k);
		rprintf(strerr);
	}
	else if(e=='2')
	{
		sprintf(strerr,"\nError in Line Number %d :Label Previously Declared\n",k);
		rprintf(strerr);
	}
	else if(e=='3')
	{
		sprintf(strerr,"\nError in Line Number %d :Non Decimal Numerical or Address Reference\n",k);
		rprintf(strerr);
	}
	else if(e=='4')
	{
		sprintf(strerr,"\nError in Line Number %d :Too many words in instruction format\n",k);
		rprintf(strerr);
	}
	else if(e=='5')
	{
		sprintf(strerr,"\nError in Line Number %d :Too few words in instruction format\n",k);
		rprintf(strerr);
	}
	else if(e=='6')
	{
		sprintf(strerr,"\nError in Line Number %d :Too Large words in instruction format\n",k);
		rprintf(strerr);
	}
	else if(e=='7')
	{
		sprintf(strerr,"\nError in Line Number %d :Invalid operand in RESW\n",k);
		rprintf(strerr);
	}
	else if(e=='8')
	{
		sprintf(strerr,"Error in Line Number %d :Invalid operand after WORD",k);
		rprintf(strerr);
	}
	else if(e=='9')
	{
		sprintf(strerr,"Error in Line Number %d :Operand Not Declared or Missing",k);
		rprintf(strerr);		
	}
}
void pass2(FILE* fpi,FILE* fpl,FILE* fpo,SYMTAB* S)
{	  
	char *bline;
	bline=(char*)malloc(MAXLINESIZE*sizeof(char));
	iline temp;
	int startflag=0,endflag=0,errorflag=0,newtextrecordflag=0,savetextrecordflag=1;
	char hline[20],tline[70],eline[7];
	int i;
	for(i=0;i<70;i++) tline[i]=' ';
	char tcount[3],tcountrev[3];
	int idx;
	hline[0]='H';
	tline[0]='T';
	eline[0]='E';
	hline[19]='\0';	
	tline[69]='\0';
	eline[7]='\0';
	tcount[2]='\0';	
	tcount[0]='0';
	tcount[1]='0';
	idx=9;
	char lengthprot[7],lengthpro[7];
	readLine(fpl,lengthprot);
	char* lengthhex;
	lengthhex=decToHex(getdec(lengthprot));
	copyword(lengthhex,lengthpro);
	//copyword(lengthprot,lengthpro);
	const char* startins="START\0";
	const char* endins="END\0";
	const char* appendzero="00";
	const char* fourzero="0000";
	const char* star="******";
	char addfirst[6];
	addfirst[0]='0';
	addfirst[1]='0';
	char tempopcode[3];
	tempopcode[3]='\0';
	char tempoperand[5];
	tempoperand[5]='\0';
	char tempword[7];
	tempword[6]='\0';
	char* hex;
	int k=0;
	int count=0;
	while(readLine(fpi,bline)==0)
	{
		k++;
		if(split(bline,&temp))			//Split function splits the components of the intermediate file into the needed fields
		{
			//printinstruction(temp);
			if(temp.ierror[0]!='0')
			{
				senderror(temp.ierror[0],k);
				//exit(1);
			}
			
			if(isEqual(temp.iinstruction,startins))           //START Assembler directive
			{
				startflag=1;
				newtextrecordflag=1;
				copyStringat(temp.ilabel,hline,1,6);
				copyStringat(appendzero,hline,7,8);
				copyStringat(temp.ioperand,hline,9,12);
				copyStringat(lengthpro,hline,13,18);
				copyStringat(temp.ioperand,addfirst,2,5);
				writeline(fpo,hline);
			}
			else if(isEqual(temp.iinstruction,endins))         //END Assembler Directive
			{
				tline[1]='0';
				tline[2]='0';
				writeline(fpo,tline);
				endflag=1;
				copyStringat(addfirst,eline,1,6);
				writeline(fpo,eline);
				fclose(fpo);
				return;
			}
			else
			{
				if(isEqual(temp.iinstruction,RESW))          //RESW Assembler Directive
				{
					//ignore this assembler directive has already been processed in pass 1
					count=getdec(temp.ioperand);
					//printf("\n COUNT: %d",count);
					while(count!=0)
					{
						if(idx>63)
						{
							tline[1]='0';
							tline[2]='0';
							writeline(fpo,tline);
							init(tline,70);
							tline[0]='T';
							tline[1]='0';
							tline[2]='0';
							copyStringat(temp.iaddress,tline,3,6);
							tcount[2]='\0';	
							tcount[0]='0';
							tcount[1]='0';
							idx=9;
						}
						copyStringat(star,tline,idx,idx+5);
						idx=idx+6;
						incrementByte(tcount);
						incrementByte(tcount);
						incrementByte(tcount);
						tline[8]=tcount[0];
						tline[7]=tcount[1];
						count--;
					}
				}
				else if(isEqual(temp.iinstruction,RESB))       //RESB Assembler Directive
				{
					//ignore this assembler directive has already been processed in pass 1
				}
				else if(isEqual(temp.iinstruction,WORD))          //WORD Assembler Directive
				{
					if(idx>63)
					{
						tline[1]='0';
						tline[2]='0';
						writeline(fpo,tline);
						init(tline,70);
						tline[0]='T';
						tline[1]='0';
						tline[2]='0';
						copyStringat(temp.iaddress,tline,3,6);
						tcount[2]='\0';	
						tcount[0]='0';
						tcount[1]='0';
						idx=9;
					}
					hex=decToHex(getdec(temp.ioperand));
					copyword(temp.ioperand,tempword);
				    copyStringat(hex,tline,idx,idx+5);
					idx=idx+6;
					incrementByte(tcount);
					incrementByte(tcount);
					incrementByte(tcount);
					tline[8]=tcount[0];
					tline[7]=tcount[1];
				}
				else if(isEqual(temp.iinstruction,BYTE))	//BYTE Assembler Directive
				{
					//Not to be done
				}
				else
				{
					if(newtextrecordflag==1)
					{
						newtextrecordflag=0;
						copyStringat(appendzero,tline,1,2);
						copyStringat(temp.iaddress,tline,3,6);
						savetextrecordflag=0;
					}
					if(idx>63)
					{
						savetextrecordflag=1;
						tline[1]='0';
						tline[2]='0';
						writeline(fpo,tline);
						init(tline,70);
						tline[0]='T';
						tline[1]='0';
						tline[2]='0';
						savetextrecordflag=0;
						copyStringat(appendzero,tline,1,2);
						copyStringat(temp.iaddress,tline,3,6);
						tcount[0]='0';
						tcount[1]='0';
						idx=9;
					}
					if(temp.ioperand[0]=='!')
					{
						//printf("\nERROR:OPERAND Missing in Instruction at address %s",temp.iaddress);
						getOpcode(temp.iinstruction,tempopcode);
						copyStringat(tempopcode,tline,idx,idx+1);
					    idx=idx+2;
					    copyStringat(fourzero,tline,idx,idx+3);
						idx=idx+4;
						incrementByte(tcount);
						incrementByte(tcount);
						incrementByte(tcount);
						tline[8]=tcount[0];
						tline[7]=tcount[1];
					}
					else if(!searchSYMTAB(S,temp.ioperand,tempoperand))
					{
					 	//printf("\nERROR: Operand is not use or Operand declaration is missing");
					 	senderror('9',k);
						getOpcode(temp.iinstruction,tempopcode);
						copyStringat(tempopcode,tline,idx,idx+1);
					    idx=idx+2;
					    copyStringat(fourzero,tline,idx,idx+3);
						idx=idx+4;
						incrementByte(tcount);
						incrementByte(tcount);
						incrementByte(tcount);
						tline[8]=tcount[0];
						tline[7]=tcount[1];
					}
					else
					{
						getOpcode(temp.iinstruction,tempopcode);
						//printf("\n OPCODE:%s:OPERAND:%s:\n",tempopcode,tempoperand);
						copyStringat(tempopcode,tline,idx,idx+1);
					    idx=idx+2;
					    copyStringat(tempoperand,tline,idx,idx+3);
						idx=idx+4;
						incrementByte(tcount);
						incrementByte(tcount);
						incrementByte(tcount);
						tline[8]=tcount[0];
						tline[7]=tcount[1];
					}
				}
			}
		}
	}
}
char* tocharstar(char* temp)
{
	char *ret;
	ret=(char*)malloc(strlen(temp)*sizeof(char));
	strncpy(temp,ret,strlen(temp));
}
void readSymtab(FILE* fps,SYMTAB* S)
{
	initSYMTAB(S,MAXSYMCOUNT);
	char *bline;
	bline=(char*)malloc(MAXLINESIZE*sizeof(char));
	isymbol temp;
	char* templabel,tempaddress;
	readLine(fps,bline);
	while(readLine(fps,bline)==0)
	{
		splitsymbol(bline,&temp);
		insertSYMTAB(S,temp.islabel,temp.isaddress);
	}
	//splitsymbol(bline,&temp);
	//insertSYMTAB(S,temp.islabel,temp.isaddress);
}
int main(int argc,char **argv)
{
	// argv[1] will be the name of intermediate file
	// argv[2] will be the name of symbol table file
	// argv[3] will be the name of length file 
	// argv[4] will be the name of desired output file
	// usage "./a.out input symtab length output"
	strcpy(OPTAB[0].instruction,"LDA");                //Copy opcode pnemonics and opcode into Optab
	strcpy(OPTAB[0].opcode,"00");
	strcpy(OPTAB[1].instruction,"MUL");
	strcpy(OPTAB[1].opcode,"20");
	strcpy(OPTAB[2].instruction,"DIV");
	strcpy(OPTAB[2].opcode,"24");
	strcpy(OPTAB[3].instruction,"STA");
	strcpy(OPTAB[3].opcode,"0C");
	strcpy(OPTAB[4].instruction,"ADD");
	strcpy(OPTAB[4].opcode,"18");
	FILE *fpi=fopen(argv[1],"r");                      //fpi is the file pointer  of intermediate file
	FILE *fps=fopen(argv[2],"r");					   //fps is the file pointer of symbol table file
	FILE *fpl=fopen(argv[3],"r");					   //fpl is the file pointer  of length file
	FILE *fpo=fopen(argv[4],"w");                      //fpo is the file pointer of the output file
	SYMTAB* S;
	S=(SYMTAB*)malloc(sizeof(SYMTAB));
	if(fpi==NULL)
	{
		printf("\nERROR:Intermediate file %s not found",argv[1]);
		return 0;
	}
	else
	{
		printf("\nIntermediate File Found.........Processing");
	}
	if(fps==NULL)
	{
		printf("\nERROR:Symbol Table file %s not found",argv[2]);
		return 0;
	}
	if(fpl==NULL)
	{
		printf("\nERROR:Length file %s not found",argv[3]);
		return 0;
	}
	else
	{
		printf("\nSymbol Table File Found.........Processing");
	}
	printf("\n 				Reading Symbol Table                  \n");
	
	readSymtab(fps,S);                                  //Function to read Symbol table from symbol file 
	//printSYMTAB(S);
	printf("\n              Preparing for Pass 2                \n");
	pass2(fpi,fpl,fpo,S);								//Call Pass 2 Function
	printf("\n");
	return 0;
}