all:lalrparser testparser
testparser:parser_test.y scanner_test.l
	# bison -d parser_test.y -o parser_test.c --debug -v
	bison -d parser_test.y -o parser_test.c
	lex -t scanner_test.l >scanner_test.c
	cc parser_test.c scanner_test.c -o testparser
lalrparser:parser.y scanner.l symtab.c set.c fileio.c string.c code.c
	# bison -d parser.y -o parser.c --debug -v
	bison -d parser.y -o parser.c
	lex -t scanner.l >scanner.c
	cc parser.c scanner.c -o lalrparser -g