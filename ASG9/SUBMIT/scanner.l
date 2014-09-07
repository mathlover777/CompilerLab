%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "parser.h"
	#include <string.h>
	int line=1;
%}
digit [0-9]
sign [-+]
space " "
newline "\n"
tab "\t"
gap {space}|{tab}
signedinteger {sign}({digit}+)
unsignedinteger {digit}+ 
integer {signedinteger}|{unsignedinteger}
float (({digit}*)"."({digit}+))
id [a-zA-Z]([a-zA-Z0-9]*)
%%
"and" {
	return AND;
}
":=" {
	return ASSIGN;
}
":" {
	return COLON;
}
"," {
	return COMMA;
}
def {
	return DEF;
}
else {
	return ELSE;
}
"/" {
	return DIV;
}
end {
	return END;
}
"=" {
	return EQ;
}
float {
	strcpy(yylval.type,"float");
	return FLOAT;
}
">=" {
	return GE;
}
global {
	return GLOBAL;
}
">" {
	return GT;
}
if {
	return IF;
}
int {
	strcpy(yylval.type,"int");
	return INT;
}
boolean {
	strcpy(yylval.type,"boolean");
	return BOOLEAN;
}
"null" {
	strcpy(yylval.type,"null");
	return NULL_;
}
"(" {
	return LEFT_PAREN;
}
"{" {
	return LEFT_BRACES;
}
"}" {
	return RIGHT_BRACES;
}
"[" {
	return LEFT_SQ_BKT;
}
"<=" {
	return LE;
}
"<" {
	return LT;
}
"-" {
	return MINUS;
}
"%" {
	return MOD;
}
"*" {
	return MULT;
}
"<>" {
	return NE;
}
not {
	return NOT;
}
"or" {
	return OR;
}
"+" {
	return PLUS;
}
print {
	return PRINT;
}
product {
	return PRODUCT;
}
read {
	return READ;
}
return {
	return RETURN;
}
")" {
	return RIGHT_PAREN;
}
"]" {
	return RIGHT_SQ_BKT;
}
";" {
	return SEMICOLON;
}
while {
	return WHILE;
}
{id} {
	//printf("\n*****ID  = {%s}",yytext);
	strcpy(yylval.id,yytext);
	return ID;
}
{unsignedinteger} {
	strcpy(yylval.number,yytext);
	//printf("\n**********INTEGER = %s*****",yytext);
	return INT_CONST;
}
{float} {
	strcpy(yylval.number,yytext);
	//printf("\n**********FLOAT = %s*****",yytext);
	return FLOAT_CONST;
}
"%d" {
	strcpy(yylval.type,"int");
	return FORMAT;
}
"%f" {
	strcpy(yylval.type,"float");
	return FORMAT;
}
"." {
	return DOT;
}
{newline} {
	line++;
}
"//"((.)*)"\n" {
	//printf("\nCOMMENT = {%s}\n",yytext);
	line++;
}
{gap} {}
. {
	printf("\nERROR : Unknown Symbol {%s} at line %d\n",yytext,line);
}
%%
