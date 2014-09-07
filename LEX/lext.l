%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#define SPACE 1
	#define NEWLINE 2
	#define PSTART 3
	#define PEND 4
	#define PUNCT 5
	#define WORD 6
	#define NOTHING 0
	#define LETTER 7
	char a[1000];
	int last=NOTHING;
	void skipText(char *dst,const char *src,int i){
		int l=strlen(src);
		if(i>=l){
			dst[0]='\0';
			return;
		}
		int k=0;
		int j;
		for(j=i;j<l;j++){
			dst[k]=src[j];
			k++;
		}
		dst[k]='\0';
		return;
	}

%}
punctuation ("."|","|"!"|":")
space (" "|"\t")
notspace [^ \t]
newline "\n"
%%
{space}{punctuation} {
	//printf("\n<SPACE+PUNCT>");
	if(last!=SPACE){
		printf("%s",yytext);
		last=PUNCT;
	}else{
		skipText(a,yytext,1);
		printf("%s",a);
	}
}
{punctuation} {
	if(last!=SPACE){
		printf(" %s",yytext);
		last=PUNCT;
	}
}
{space}*")" {
	printf(")");
	last=PEND;
}
"(" {
	printf("( ");
	last=SPACE;
}
{space} {
	if(last!=SPACE&&last!=PUNCT){
		printf(" ");
		last=SPACE;
	}
}
{newline}({newline}+) {
	printf("\n\n");
	last=NEWLINE;
}
{newline} {
	printf("\n");
	last=NEWLINE;
}
. {
	printf("%s",yytext);
	last=LETTER;
}
%%
int main(){
	yylex();
	return 0;
}