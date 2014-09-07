%{
int count=0;
int pos=0;
%}
pattern1 a
pattern2 abbbaa
%%
{pattern1}/{pattern2} {
	count++;
	printf("\nMatch at = %d",pos);
	pos++;
	}
. {pos++;}
%%
int main(){
	yylex();
	printf("\nCOUNT = %d ",count);
	return 0;
}