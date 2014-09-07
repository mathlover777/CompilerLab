%{
	#include <stdio.h>
	char a[1000];
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
digit [0-9]
sign [-+]
space " "
newline "\n"
tab "\t"
integer ({sign}({digit}+))|({digit}+)
float ({sign}({digit}*)"."({digit}+))|(({digit}*)"."({digit}+))
invalid ({integer}|{float})
%%
{invalid} {
	printf("\nNOT DIGIT : {%s}",yytext);
}
{integer} {
	//if(yytext[0]==' '){
	//	skipText(a,yytext,1);
	//}else{
	//	skipText(a,yytext,0);
	//}
	//{space}{newline}{tab}
	printf("\nINTEGER = {%s}",yytext);
}
{float} {
	//if(yytext[0]==' '){
	//	skipText(a,yytext,1);
	//}else{
	//	skipText(a,yytext,0);
	//}
	printf("\nFLOAT = {%s}",yytext);
}
{space}|{tab}|{newline} {}
.|"\n" {}
%%
int main(){
	yylex();
	return 0;
}