all:lalrparser
lalrparser:parser.y scanner.l
	bison -d parser.y -o parser.c --debug -v
	lex -t scanner.l >scanner.c
	cc parser.c scanner.c -o lalrparser
