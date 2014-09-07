#include <stdio.h>
#include <stdlib.h>
#include "macro.c"

int main(int argc,char **argv){
	char INPUTFILE[100];
	char OUTPUTFILE[100];
	switch(argc){
		case 1:{
			sprintf(INPUTFILE,"input.s");
			sprintf(OUTPUTFILE,"output.out");
			break;
		}
		case 2:{
			sprintf(INPUTFILE,"%s",argv[1]);
			sprintf(OUTPUTFILE,"output.out");
			break;
		}
		case 3:{
			sprintf(INPUTFILE,"%s",argv[1]);
			sprintf(OUTPUTFILE,"%s",argv[2]);
			break;
		}
		default:{
			printf("\nUSAGE : './macroASM <input[optional]> <output[optional]>");
			exit(-1);
		}
	}
	printf("\nINPUTFILE : {%s}",INPUTFILE);
	printf("\nOUTPUTFILE : {%s}",OUTPUTFILE);
	char call[1000];
	sprintf(call,"./macroProcessor %s expand_%s",INPUTFILE,INPUTFILE);
	system(call);
	sprintf(call,"./pass1 expand INTFILE_%s",INPUTFILE,INPUTFILE);
	system(call);
	sprintf(call,"./pass2 INTFILE_%s SYMTAB length.txt %s",INPUTFILE,OUTPUTFILE);
	system(call);
	return 0;
}