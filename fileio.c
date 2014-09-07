#ifndef FILEIO
#define FILEIO

#define MAXLABEL 15
#define MAXINS 10
#define MAXADDRESS 4
#define MAXSYMCOUNT 1000
#define	MAXLINESIZE 300
#define MAXCOMMENTSIZE 300
#define MAXOPERAND 15
#define PRINTCONTENT printcontent(label,ins,operand,comment);
#define MAXBUFFERSIZE 1000

/*****************FILE read write functions**************************/
void readLine(FILE *fp,char *line,int *endflag){
	// memory for line must be already allocated
	if(*endflag==0){
		line[0]='\0';
		return;
	}
	char x;
	int i=0;
	x=getc(fp);
	while(1){
		if(x==EOF||x=='\n'){
			if(x==EOF){
				(*endflag)=1;
			}
			line[i]='\0';
			if(i>=1){
				if((int)line[i-1]==13){
					line[i-1]='\0';
				}
			}
			return;
		}else{
			line[i]=x;
		}
		x=getc(fp);
		i++;
	}
}
FILE* initializeFileRead(const char *filename,int *fileflag){
	(*fileflag)=0;
	FILE *fp=fopen(filename,"r");
	if(fp==NULL){
		printf("\nFATAL ERROR : FILE NOT FOUND !!\n");
		return NULL;
	}
	return fp;
}
void resetfile(const char *x){
	FILE *f=fopen(x,"w");
	strcpy(FILENAME,x);
	fclose(f);
	return;
}
void addLine(const char *filename,char *line){
	// printf("\n*************APPENDING : '%s'  ****\n",line);
	FILE *f=fopen(filename,"a");
	fprintf(f, "%s\n",line);
	fclose(f);
	return;
}
int isExist(const char *FILENAME){
	FILE *fp=fopen(FILENAME);
	if(fp==NULL){
		return 0;
	}
	fclose(fp);
	return 1;
}
/********************************************************************/


#endif