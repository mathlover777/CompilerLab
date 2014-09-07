#include <stdlib.h>
#include <stdio.h>

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
		exit(-1);
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
void replaceBraces(char *string){
	while((*string)!='\0'){
		switch(*string){
			case '[':{
				(*string)='(';
				break;
			}
			case ']':{
				(*string)=')';
				break;
			}
		}
		string++;
	}
	return;
}
void replaceRange(char *a,char *b){
	int i=0,j=0;
	char first,last;
	while(1){
		if(a[i]=='\0'){
			b[j]='\0';
			return;
		}
		if(a[i]=='-'){
			// we detected a range sign like <char1>-<charn> going to replace it like
			// (<char1>|<char2>|....|<charn-1>|<charn>)
			// currently <char1> is written and writehead is pointing at the next pos
			j--;
			first=a[i-1];
			last=a[i+1];
			b[j]='(';
			j++;
			while(first<last){
				b[j]=first;
				j++;
				b[j]='|';
				j++;
				first=(char)((int)first+1);
			}
			b[j]=first;
			j++;
			b[j]=')';
			j++;
			i++;
			i++;
		}else{
			b[j]=a[i];
			j++;
			i++;
		}
	}
}
int isOperator(char x){
	if(x=='+'||x=='*'||x=='|'){
		return 1;
	}
	return 0;
	// everything else is a valid symbol
}
int isSymbol(char x){
	if((isOperator(x)==0)&&x!='('&&x!=')'){
		return 1;
	}
	return 0;
}
void addPlus(const char *a,char *b){
    int i=0,j=0;
    if(a[i]=='\0'){
        b[i]='\0';
        return;
    }
    while(1){
        if(a[i+1]=='\0'){
            b[j]=a[i];
            j++;
            b[j]='\0';
            break;
        }
        // printf("%d %d %d\n",(a[i]=='*'),(isSymbol(a[i+1])==1||a[i+1]=='('),((a[i]=='*')&&(isSymbol(a[i+1])==1||a[i+1]=='('))   );
        // printf("a[i] = %c a[i+1]=%c",a[i],a[i+1] );
        // printf(" isSymbol(a[i+1])=%d\n",isSymbol(a[i+1]));
        if((  isSymbol(a[i])==1&&(isSymbol(a[i+1])==1||(a[i+1]=='('))  )||  (isSymbol(a[i])==1&&(a[i+1]=='('))
            || (isSymbol(a[i+1])==1&&(a[i]==')')) || ((a[i]==')')&&(a[i+1]=='(')) ||
            ((a[i]=='*')&&(isSymbol(a[i+1])==1||a[i+1]=='('))  ){
            //printf("A[i]=%c\n",a[i] );
            b[j]=a[i];
            j++;
            b[j]='+';
            j++;
            i++;
            continue;
        }else{
            b[j]=a[i];
            j++;
            i++;
        }
    }
}

int main(){
	char a[1000],c[1000],b[1000];
	scanf("%s",a);
	printf("\nGiven String : {%s}\n",a);
	replaceRange(a,c);
	replaceSq(c);
	replaceBraces(c);
	addPlus(c,b);
	printf("\nModified String : {%s}\n",b);
	return 0;
}

//((_((a|b)))|((A|B|C)))(_|((a|b|c))|((A|B))|((0|1)))*