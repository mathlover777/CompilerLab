all:lalrparser
lalrparser:parser_test.y scanner_test.l
	bison -d parser_test.y -o parser_test.c --debug -v
	lex -t scanner_test.l >scanner_test.c
	cc parser_test.c scanner_test.c -o lalrparser
