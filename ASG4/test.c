#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getFloat(const char *source,float *dest,int i,int j){
	int floatflag=0;
	int dotPos=j+1;
	int sign=1;
	if(source[i]=='+'){
		i++;
	}
	if(source[i]=='-'){
		i++;
		sign=-1;
	}
	// for(int k=i;k<=j;k++){
	// 	if(source[k]=='\0'){
	// 		printf("\nERROR !!! going to read null in flaot !!\n");
	// 		*dest=0.0;
	// 		return;
	// 	}
	// 	if(source[k]=='.'){
	// 		dotPos=k;
	// 		// floatflag=1;
	// 		break;
	// 	}
	// }
	float integer=0.0;
	float frac=0.0;
	int k=i;
	while(1){
		if(k==j+1){
			break;
		}
		if(source[k]=='.'){
			break;
		}
		integer=integer*10+((int)source[k]-48);
		k++;
	}
	if(k==j+1){
		*dest=(integer)*(sign);
		return;
	}
	if(k==j&&source[k]=='.'){
		printf("\nError in expresssion !");
		*dest=0.0;
		return;
	}
	k++;
	float power=10;
	while(k<=j){
		frac=frac+(((int)source[k]-48)/power);
		power=power*10;
		k++;
	}
	*dest=(integer+frac)*(sign);
	return;

}
int main(){
	char a[100];
	scanf("%s",a);
	float x;
	getFloat(a,&x,0,strlen(a)-1);
	printf("\nFloat ->%f\n",x);
}