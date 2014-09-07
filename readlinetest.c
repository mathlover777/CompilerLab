#include <stdio.h>
#include <stdlib.h>
void readLine(FILE *fp,char *line){
	// memory for line must be already allocated
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			line[i]='\0';
			return;
		}else{
			line[i]=x;
		}
		x=getc(fp);
		i++;
	}
}
int main(){
	FILE *fp=fopen("test","r");
	char line[100];
	readLine(fp,line);
	printf("\n%s\n",line);
	readLine(fp,line);
	printf("\n%s\n",line);

	return 0;	
}