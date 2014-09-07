#include <stdio.h>
#include <stdlib.h>
#include "macro.c"
#include "string.c"

//    _____    _________ _________.___  ________  _______      _____  ___________ __________________  .________
//   /  _  \  /   _____//   _____/|   |/  _____/  \      \    /     \ \_   _____/ \      \__    ___/  |   ____/
//  /  /_\  \ \_____  \ \_____  \ |   /   \  ___  /   |   \  /  \ /  \ |    __)_  /   |   \|    |     |____  \ 
// /    |    \/        \/        \|   \    \_\  \/    |    \/    Y    \|        \/    |    \    |     /       \
// \____|__  /_______  /_______  /|___|\______  /\____|__  /\____|__  /_______  /\____|__  /____|    /______  /
//         \/        \/        \/             \/         \/         \/        \/         \/                 \/ 

// Author(s): 	Sourav Sarkar
// 			11CS30037
// 			Shrihari A Bhat
// 			11CS30008
// DATE : SEP 7/11:37PM


int main(int argc,char **argv){
	system("reset");
	char INPUTFILE[100];
	char OUTFILE[100];
	switch(argc){
		case 1:{
			// printf("\nFATAL ERROR NO FILE GIVEN !!");
			// exit(-1);
			printf("\nWarning Taking input output name default!!");
			sprintf(INPUTFILE,"input.s");
			sprintf(OUTFILE,"output.s");
			break;
		}
		case 2:{
			printf("\nInput file : %s",argv[1]);
			copyString(INPUTFILE,argv[1]);
			sprintf(OUTFILE,"output.s");
			break;
		}
		default:{
			printf("\nINPUTFILE = %s",argv[1]);
			printf("\nOUTPUT = %s",argv[2]);
			copyString(INPUTFILE,argv[1]);
			copyString(OUTFILE,argv[2]);
			break;
		}
	}
	if(isExist(INPUTFILE)!=1){
		printf("\nInput file does not exist !! BYE !!\n");
		exit(-1);
	}
	macroDriver(INPUTFILE,OUTFILE);
}