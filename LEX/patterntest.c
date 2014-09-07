// code to check the pattern aabbbaa
#include <stdio.h>
#include <stdlib.h>
void getMatch(char *a){
	int state=0;
	int i=0;
	int count=0;
	while(a[i]!='\0'){
		if(!(a[i]=='a'||a[i]=='b')){
			state=8;
		}
		// printf("\nA=%c",a[i]);
		switch(state){
			case 0:{
				if(a[i]=='a'){
					// a[i] = a
					state=1;
				}else{
					// a[i] = b
					state=0;
				}
				break;
			}
			case 1:{
				if(a[i]=='a'){
					// a[i] = a
					state=2;
				}else{
					// a[i] = b
					state=0;
				}
				break;	
			}
			case 2:{
				if(a[i]=='a'){
					// a[i] = a
					state=2;
				}else{
					// a[i] = b
					state=3;
				}
				break;
			}
			case 3:{
				if(a[i]=='a'){
					// a[i] = a
					state=1;
				}else{
					// a[i] = b
					state=4;
				}
				break;
			}
			case 4:{
				if(a[i]=='a'){
					// a[i] = a
					state=1;
				}else{
					// a[i] = b
					state=5;
				}
				break;
			}
			case 5:{
				if(a[i]=='a'){
					// a[i] = a
					state=6;
				}else{
					// a[i] = b
					state=0;
				}
				break;
			}
			case 6:{
				if(a[i]=='a'){
					// a[i] = a
					state=7;
				}else{
					// a[i] = b
					state=0;
				}
				break;
			}
			case 7:{
				printf("\nMATCH AT = %d",i-7);
				count++;
				if(a[i]=='a'){
					// a[i] = a
					state=2;
				}else{
					// a[i] = b
					state=3;
				}
				break;
			}
			default:{
				printf("\nERROR !!state  = %d , i = %d\n",state,i);
				return;
			}
		}
		i++;
	}
	if(state==7){
		printf("\nMATCH AT = %d",i-7);
		count++;
	}
	printf("\nTotal Matches = %d\n",count);
	return;
}
int main(){
	char a[1000];
	scanf("%s",a);
	printf("\nTEXT = %s",a);
	getMatch(a);
	return 0;
}
