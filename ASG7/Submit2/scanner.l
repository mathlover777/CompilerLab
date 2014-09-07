%{
	#include <stdio.h>
	#include <stdlib.h>

	#define NEWLINE 1

	FILE *fp;
	int last=0;
	int line=1;
	char Buffer[1000];
	int l=0;
	int normstart;


	void initFile(const char *filename){
		fp=fopen(filename,"w");
		if(fp==NULL){
			printf("\nCANT OPEN FILE ,,, exit\n");
			exit(-1);
		}
		return;
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
		printf("\nLINE={%s}\n",Buffer);
		fprintf(fp,"%s\n",Buffer);
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
dot "."
cross "X"
pstart "("
pend ")"
invalid ([a-zA-Z]|"_"|"-"|";"|":"|"!"|"@"|"$"|"%"|"^"|"&"|"{"|"}"|"/")+
%%
{add} {
	addToBuffer(" + ");
	last=0;
}
{mult} {
	addToBuffer(" * ");
	last=0;
}
{norm} {
	if(normstart==0){
		addToBuffer(" NORM ");
		normstart=1;
	}else{
		normstart=0;
	}
	last=0;
	
}
{angle} {
	addToBuffer(" # ");
	last=0;
}
{dot} {
	addToBuffer(" . ");
	last=0;
}
{pstart} {
	addToBuffer(" ( ");
	last=0;
}
{pend} {
	addToBuffer(" ) ");
	last=0;
}
{cross} {
	addToBuffer(" X ");
	last=0;
}
{integer} {
	addToBuffer(" id ");
	last=0;
}
{vstart} {
	addToBuffer(" [ ");
	last=0;
}
{vend} {
	addToBuffer(" ] ");
	last=0;
}
{sign}{space} {
	printf("\nERROR at line =%d cant have space after sign",line);
	//exit(-1);
}
{newline} {
	if(last!=NEWLINE){
		printf("\n");
		last=NEWLINE;
		flushToFile();
	}
	l=0;
	Buffer[0]='\0';
	line++;
	normstart=0;
}
{comma} {
	addToBuffer(" , ");
	last=0;
}
{space} {}
{invalid} {
	printf("\nINVALID SYMBOL = {%s} at line %d",yytext,line);
	//exit(-1);
}
. {
	//printf("\nINVALID SYMBOL = {%s} at line %d",yytext,line);
	//exit(-1);
}
%%
int main(){
	initFile("token.txt");
	Buffer[0]='\0';
	yylex();
	if(Buffer[0]!='\0'){
		flushToFile();
		Buffer[0]='\0';
	}
	fclose(fp);
	return 0;
}