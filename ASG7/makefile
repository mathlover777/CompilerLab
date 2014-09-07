all:scanner llparser
scanner:scanner.l
	lex -t scanner.l > scanner.c
	cc -c -o scanner.out scanner.c
	cc -o scanner scanner.out -ll

llparser:parser.c parsergen.c driver.c string.c generic.c stack.c fileio.c prod.c
	g++ driver.c -o llparser
	