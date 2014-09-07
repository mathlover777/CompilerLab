#include <stdio.h>
#include <stdlib.h>
int strToDec(const char *a){
	int n=0,i=0,p=1;
	int d;
	while(a[i]!='\0'){
		if(a[i]<=57&&a[i]>=48){
			d=a[i]-48;
		}else{
			// d=a[i]-55;
			printf("\nNon decimal number detedted !! taking that as 1");
			return 1;
		}
		n=n*10+d;
		// p=p*10;
		i++;
	}
	return n;
}
int main(){
	const char *p="01234";
	printf("\nINT = %d\n",strToDec(p));
	return 0;
}