#include "fileio.c"

#ifndef STRING_MINE
#define STRING_MINE

int ENABLECOLOR=0;
/****************String functions***********************************/
#define MAXSTRINGSIZE 100
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
int isEqual2(const char *a,int i,int j,const char *b,int k,int l){
	if((j-i)!=(l-k)){
		// printf("\nSIZE NOT SAME !!\n");
		return 0;
	}
	// char x[MAXSTRINGSIZE],y[MAXSTRINGSIZE];
	for(int t=0;t<=(j-i);t++){
		// printf("\nEQUAL2 : a = %c b=%c",a[t+i],b[t+k]);
		if(a[t+i]!=b[t+k]){
			return 0;
		}
	}
	return 1;
}
int copyString(char *dest,const char *source){
	int i=0;
	while(source[i]!='\0'){
		dest[i]=source[i];
		i++;
	}
	dest[i]='\0';
}
int getSize(const char *a){
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
	printf("\tINS = '%s'",ins);
	printf("\tOPERND = '%s'",operand);
	printf("\tCOMMENT = '%s'",comment);
	// printf("\n***********************************************");
	return;
}
void rprintf(const char *msg){
	if(ENABLECOLOR==1){
		printf("\033[31m\033[40m%s\033[0m",msg);
	}else{
		printf("%s",msg);
	}
	return;
}
void gprintf(const char *msg){
	if(ENABLECOLOR==1){
		printf("\033[32m\033[47m%s\033[0m",msg);
	}else{
		printf("%s",msg);
	}
	return;
}
int strToDec(const char *a){
	int n=0,i=0,p=1;
	int d;
	while(a[i]!='\0'){
		if(a[i]<=57&&a[i]>=48){
			d=a[i]-48;
		}else{
			// d=a[i]-55;
			rprintf("\nNon decimal number detedted !! taking that as 1");
			return 1;
		}
		n=n*10+d;
		// p=p*10;
		i++;
	}
	return n;
}
int strToDec2(const char *a,int i,int j){
	char b[20];
	int k=i;
	while(1){
		if(k>j){
			break;
		}
		b[k-i]=a[k];
		k++;
	}
	b[k-i]='\0';
	return strToDec(b);
}
/*******************************************************************/
int getContent(const char *xline,char *label,char *ins,char *operand,char *comment){
	// deletes the contents of label,ins,operand and gives new information in them
	// depending on errors returns many flags
	// 0 means success
	// 1-> more than 3 words
	// 2-> <2 words
	// 3-> nothing or comment
	// 4-> too large words or comment
	char line[MAXBUFFERSIZE];
	sprintf(line,"%s",xline); 
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
	if(line[0]=='\0'){
		return 0;
	}
	macroname[0]='\0';
	(*start)=0;
	(*end)=0;
	int i=0;
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
#endif