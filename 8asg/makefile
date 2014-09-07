all:parser
parser:parsertest.y scannertest.l
	bison -d parsertest.y -o parsertest.c --debug
	lex -t scannertest.l > scannertest.c
	cc scannertest.c parsertest.c -o parser
