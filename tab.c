#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *f=fopen("cfc.txt","r");
	char c;
	c=getc(f);
	while(1){
		if(c==EOF){
			return 0;
		}
		if(c=='\n'){
			printf("\n");
			c=getc(f);
		}
		printf(" %d ",(int)c);
		c=getc(f);
	}
}
