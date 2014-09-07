#include <stdio.h>
#include <stdlib.h>
#include "macro.c"
#include "string.c"



int main(int argc,char **argv){
	switch(argc){
		case 0:{
			// printf("\nFATAL ERROR NO FILE GIVEN !!");
			// exit(-1);
			printf("\nWarning Taking input output name default!!");
			const char *INPUTFILE="input.s";
			const char *OUTFILE="output.s";
		}
		case 1:{
			printf("\nInput file : %s",argv[1]);
			copyString(INPUTFILE,argv[1]);
			const char *OUTFILE="output.s";
		}
		default:{
			printf("\nINPUTFILE = %s",argv[1]);
			printf("\nOUTPUT = %s",argv[2]);
			copyString(INPUTFILE,argv[1]);
			copyString(OUTFILE,argv[2]);
		}
	}
	if(isExist(INPUTFILE)!=1){
		printf("\nInput file does not exist !! BYE !!\n");
		exit(-1);
	}
	macroDriver(INPUTFILE,OUTFILE);
}