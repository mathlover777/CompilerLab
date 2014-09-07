%{
	#include <stdio.h>
	#include <stdlib.h>

	#define NEWLINE 1

	#define ADD 1
	#define MULT 2
	#define ANGLE 3
	#define NORM 4

	#define MAXSIZE 100
	FILE *fp;
	int last=0;
	//char *buffer[1000];
	int line=1;
	int lineType=0;
	int w=0;
	char Buffer[1000];
	char temp2[1000];
	int invector=0;
	int dim1=0,dim2=0;
	
	char temp[100];
	int l=0;
	void initBuffer(){
		int i=0;
		for(i=0;i<1000;i++){
			//buffer[i]=(char *)malloc(sizeof(char)*MAXSIZE);
		}
		return;
	}
	void initFile(const char *filename){
		fp=fopen(filename,"w");
		if(fp==NULL){
			printf("\nCANT OPEN FILE ,,, exit\n");
			exit(-1);
		}
	}
	void addToBuffer(const char *a){
		int i=0;
		while(a[i]!='\0'){
			Buffer[l]=a[i];
			i++;
			l++;
		}
		Buffer[l]='\0';
		return;
	}
	void flushToFile(){
		printf("Dim1 = %d dim2=%d",dim1+1,dim2+1);
		switch(lineType){
			case ADD:{
				if(dim1!=dim2){
					printf("\nDimension Error at line %d\n",line);
					exit(-1);
				}
				sprintf(temp2,"<ADD>%s",Buffer);

				break;
			}
			case MULT:{
				sprintf(temp2,"<MULT>%s",Buffer);
				break;
			}
			case ANGLE:{
				if(dim1!=dim2){
					printf("\nDimension Error at line %d",line);
					exit(-1);
				}
				sprintf(temp2,"<ANGLE>%s",Buffer);
				break;
			}
			case NORM:{
				sprintf(temp2,"<NORM>%s",Buffer);
				break;
			}
			default:{
				printf("\nWarning no operator at line %d",line);
				sprintf(temp2,"%s",Buffer);
			}
		}
		printf("\nLINE={%s}\n",temp2);
		fprintf(fp,"%s\n",temp2);
		return;
	}
%}
add "+"
mult "*"
norm "|"
angle "#"
digit [0-9]
sign [+-]
signedint {sign}({digit}+)
unsignedint {digit}+
integer ({signedint}|{unsignedint})
vstart "["
vend "]"
newline "\n"
space (" "|"\t")
comma ","
%%
{add} {
	if(lineType!=0){
		printf("\nERROR at line = %d Cant have 2 ins in same line",line);
		exit(-1);
	}
	printf("<ADD>");
	lineType=ADD;
	last=0;
}
{mult} {
	if(lineType!=0){
		printf("\nERROR at line = %d Cant have 2 ins in same line",line);
		exit(-1);
	}
	printf("<MULT>");
	lineType=MULT;
	last=0;
}
{norm} {
	if(lineType!=0&&lineType!=NORM){
		printf("\nERROR at line = %d Cant have 2 ins in same line",line);
		exit(-1);
	}
	printf("<NORM>");
	lineType=NORM;
	last=0;
}
{angle} {
	printf("<ANGLE>");
	if(lineType!=0){
		printf("\nERROR at line = %d Cant have 2 ins in same line",line);
		exit(-1);
	}
	lineType=ANGLE;
	last=0;
}
{integer} {
	printf("<INTEGER={%s}>",yytext);
	sprintf(temp,"<%s>",yytext);
	addToBuffer(temp);
	last=0;
}
{vstart} {
	printf("<VSTART>");
	addToBuffer("<VSTART>");
	if(invector==0){
		invector=1;
	}
	if(invector==2){
		invector=3;
	}
	last=0;
}
{vend} {
	printf("<VEND>");
	addToBuffer("<VEND>");
	if(invector=1){
		invector=2;
	}
	last=0;
}
{sign}{space} {
	printf("\nERROR at line =%d cant have space after sign",line);
	exit(-1);
}
{newline} {
	printf("\nLINE = %d",line);
	if(last!=NEWLINE){
		printf("\n");
		last=NEWLINE;
		printf("\nLINE={%s}",Buffer);
		flushToFile();
	}
	l=0;
	lineType=0;
	Buffer[0]='\0';
	w=0;
	invector=0;
	dim2=dim1=0;
	line++;
}
{mult}({space}*){unsignedint}({space}*){newline} {
	printf("\nERROR at line = %d .. scalar multiplication must preceede a vector !!",line);
	exit(-1);
}
{comma} {
	if(invector==1){
		dim1++;
	}
	if(invector==3){
		dim2++;
	}
}
{space} {}
. {
	//printf("\nINVALID SYMBOL = {%s} at line %d",yytext,line);
	//exit(-1);
}
%%
int main(){
	initFile("token.txt");
	Buffer[0]='\0';
	//initBuffer();
	yylex();
	fclose(fp);
	return 0;
}