%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "parser.h"
	int line=1;
%}
digit [0-9]
sign [-+]
space " "
newline "\n"
tab "\t"
gap {space}|{tab}
integer ({sign}({digit}+))|({digit}+)
float ({sign}({digit}*)"."({digit}+))|(({digit}*)"."({digit}+))
id [a-zA-Z0-9]+
%%
and {
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
	return INT;
}
"(" {
	return LEFT_PAREN;
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
null {
	return NUL;
}
or {
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
	return ID;
}
{integer} {
	return INT_CONST;
}
{float} {
	return FLOAT_CONST;
}
"%d"|"%f"|"%s" {
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
