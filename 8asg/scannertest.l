%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "parsertest.h"	
%}
%%
id {
	return ID;
}
"*" {
	return MULT;
}
"+" {
	return PLUS;
}
"(" {
	return PLEFT;
}
")" {
	return PRIGHT;
}
"\n" {}
" " {}
. {}
%%

