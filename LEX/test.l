%{
int charcount=0,linecount=0;
%}
%%
ADD {printf("\nWE GOT ADD!!");}
. charcount++;
\n {linecount++; charcount++;}
%%
int main()
{
yylex();
printf("There were %d characters in %d lines\n",
charcount,linecount);
return 0;
}