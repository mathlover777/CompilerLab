all:tac
tac:parser.y scanner.l symtab.c set.c fileio.c string.c code.c
	bison -d parser.y -o parser.c
	lex -t scanner.l >scanner.c
	cc parser.c scanner.c -o tac -g
