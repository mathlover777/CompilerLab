#ifndef STRUCTURE
#define STRUCTURE


#define MAXVARLEN 20
#define INTSIZE 2
#define FLOATSIZE 4
#define BOOLEANSIZE 1
#define NULLSIZE 1
#define MAXSYMTABSIZE 1000
#define SUCCESS 1
#define FAILURE 0
#define MAXCODELEN 200

struct setElt{
	char varname[20];
	struct setElt *next;
};
typedef struct setElt setElt;
struct Set{
	setElt *head;
};
typedef struct Set Set;
struct Symelt{
	char varname[MAXVARLEN];
	char type[10];
	int offset;
};
typedef struct Symelt Symelt;
struct Symtable{
	Symelt record[MAXSYMTABSIZE];
	int size;
};
typedef struct Symtable Symtable;
Symtable SymTab;
struct codeElt{
	char Line[200];
	struct codeElt *next;
};
typedef struct codeElt codeElt;
struct code{
	codeElt *head;
	codeElt *tail;
};
typedef struct code code;
struct EXP_attr{
	struct code C;
	char ADDR[20];
	char type[20];
};
typedef struct EXP_attr EXP_attr;
struct STMT_attr{
	struct code C;
	char type[20];
	char next[50];
};
typedef struct STMT_attr STMT_attr;
struct BEXP_attr{
	struct code C;
	char True[50];
	char False[50];
};
typedef struct BEXP_attr BEXP_attr;
#endif