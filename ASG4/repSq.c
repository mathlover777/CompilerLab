#include <stdio.h>
#include <stdlib.h>

int replaceSq(char *a){
	// ...[...[]...]... is not allowed ,,, please use explicit union :p   
	char b[1000];
	int i=0,j=0;
	int inSq=0;
	int error=0;
	int pcount=0;
	while(1){
		if(a[i]=='\0'){
			b[j]='\0';
			break;
		}
		if(inSq==0){
			if(a[i]=='['){
				b[j]=a[i];
				j++;
				i++;
				inSq=1;
				pcount=0;
				continue;
			}else if(a[i]==']'){
				error=1;
				break;
			}else{
				b[j]=a[i];
				j++;
				i++;
				continue;
			}
		}else{
			// we are in SQ cage
			switch(a[i]){
				case '(':{
					b[j]=a[i];
					j++;
					i++;
					pcount++;
					break;
				}case ')':{
					b[j]=a[i];
					j++;
					i++;
					pcount--;
					if((pcount==0)&&(a[i]!=']')){
						b[j]='|';
						j++;
					}
					break;
				}
				case '[':{
					error=1;
					printf("\nRecursive Square Braces are not allowed");
					break;
				}
				case ']':{
					inSq=0;
					b[j]=a[i];
					j++;
					i++;
					break;
				}
				default:{
					if(pcount==0){
						// we are at the outer level ... so need to add union after charecter
						b[j]=a[i];
						j++;
						i++;
						b[j]='|';
						j++;
					}else{
						// we are in between level
						b[j]=a[i];
						j++;
						i++;
					}
				}
			}
		}
	}
	if(inSq!=0){
		printf("\nUnbalanced Sq braces in the RegExp");
		return 0;
	}
	if(error==1){
		return 0;
	}
	i=0;
	while(b[i]!='\0'){
		a[i]=b[i];
		i++;
	}
	a[i]=b[i];
	return 1;
}
// IDNTIFIER [(_(a-z))(A-Z)][_(a-z)(A-Z)(0-9)]*
// INT_CONST 0|[1-9][0-9]* 
// FLO_CONST [0-9]*.[0-9]*

int main(){
	char a[1000];
	scanf("%s",a);
	printf("\nGiven String : {%s}\n",a);
	replaceSq(a);
	printf("\nModified String : {%s}\n",a);
	return 0;
}