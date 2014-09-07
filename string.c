#ifndef STRING_MINE
#define STRING_MINE
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
void clearTabs(char *line){
	// removes spaces and tabs from line
	// like "   \t  bbbb   \t\t  rrrr \t  ggg\t  "--->"bbbb rrrr ggg"
	int i,j;
	// convert all tabs into spaces
	// printf("\n*****************BEFORE TAB REMOVAL : '%s'",line);
	i=j=0;
	while(line[i]!='\0'){
		if(line[i]=='\t'||line[i]==9||line[i]==11){
			// printf("\ntab detected !!");
			line[i]=' ';
		}
		i++;
	}
	// printf("\n****************AFTER TAB REMOVAL : '%s'",line);
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
	// printf("\n****************FINAL TAB REMOVAL : '%s'",line);
	return;
}
int strToDec(char *a){
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
int strToDec2(char *a,int i,int j){
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

#endif