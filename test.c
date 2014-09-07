#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct opelt;
struct opelt{
	char instruction[5];
	char opcode[3];
};
typedef struct opelt opelt;
opelt x[5];// the xle ... as its always fixed.. its declared globally



int main(){
	// x[0].instruction="LDA";
	strcpy(x[0].instruction,"LDA");
x[0].opcode="00";
x[1].instruction="MUL";
x[1].opcode="20";
x[2].instruction="DIV";
x[2].opcode="24";
x[3].instruction="STA";
x[3].opcode="0C";
x[4].instruction="ADD";
x[4].opcode="18";
	return 0;
}