/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "parser.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.c"
#include "symtab.c"
#include "code.c"
#include "string.c"
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 int yywrap()
{
        return 1;
}
int offset=0;


/* Line 268 of yacc.c  */
#line 92 "parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 288 of yacc.c  */
#line 20 "parser.y"

	#include "structures.c"



/* Line 288 of yacc.c  */
#line 122 "parser.c"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     AND = 258,
     ASSIGN = 259,
     COLON = 260,
     COMMA = 261,
     DEF = 262,
     ELSE = 263,
     DIV = 264,
     END = 265,
     EQ = 266,
     FLOAT = 267,
     GE = 268,
     GLOBAL = 269,
     GT = 270,
     IF = 271,
     INT = 272,
     LEFT_PAREN = 273,
     LEFT_SQ_BKT = 274,
     LE = 275,
     LT = 276,
     MINUS = 277,
     MOD = 278,
     MULT = 279,
     NE = 280,
     NOT = 281,
     NUL = 282,
     OR = 283,
     PLUS = 284,
     PRINT = 285,
     PRODUCT = 286,
     READ = 287,
     RETURN = 288,
     RIGHT_PAREN = 289,
     RIGHT_SQ_BKT = 290,
     SEMICOLON = 291,
     WHILE = 292,
     ID = 293,
     INT_CONST = 294,
     FLOAT_CONST = 295,
     DOT = 296,
     FORMAT = 297,
     BOOLEAN = 298,
     NULL_ = 299,
     RIGHT_BRACES = 300,
     LEFT_BRACES = 301
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 24 "parser.y"

	char type[20];
	char id[20];
	Set S;
	int dummy;
	EXP_attr EXP;
	char number[20];
	STMT_attr STMT;
	BEXP_attr B;
	char OP[20];



/* Line 293 of yacc.c  */
#line 199 "parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 211 "parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   112

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNRULES -- Number of states.  */
#define YYNSTATES  109

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     8,     9,    12,    16,    22,    27,    31,
      33,    35,    37,    39,    41,    43,    44,    46,    50,    53,
      56,    59,    62,    65,    68,    72,    76,    80,    89,    90,
      93,    94,    95,    96,   103,   104,   106,   107,   108,   109,
     114,   119,   123,   128,   132,   133,   134,   135,   136,   137,
     139,   141,   143,   145,   147,   149,   153,   157,   161,   165,
     169,   173,   177,   179,   181
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      48,     0,    -1,    14,    49,    55,    10,    -1,    -1,    50,
      49,    -1,     7,    51,    10,    -1,    51,    52,     5,    54,
      36,    -1,    52,     5,    54,    36,    -1,    53,     6,    52,
      -1,    53,    -1,    38,    -1,    17,    -1,    12,    -1,    43,
      -1,    44,    -1,    -1,    56,    -1,    56,    68,    57,    -1,
      69,    57,    -1,    58,    36,    -1,    59,    36,    -1,    60,
      36,    -1,    64,    61,    -1,    70,    66,    -1,    38,     4,
      67,    -1,    32,    42,    67,    -1,    30,    42,    67,    -1,
      16,    63,    71,     5,    56,    65,    62,    10,    -1,    -1,
       8,    56,    -1,    -1,    -1,    -1,    37,    73,    71,     5,
      56,    10,    -1,    -1,    78,    -1,    -1,    -1,    -1,    71,
      28,    74,    71,    -1,    71,     3,    72,    71,    -1,    26,
      75,    71,    -1,    46,    76,    71,    45,    -1,    78,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    20,
      -1,    21,    -1,    13,    -1,    15,    -1,    25,    -1,    78,
      29,    78,    -1,    78,    22,    78,    -1,    78,    24,    78,
      -1,    78,     9,    78,    -1,    78,    23,    78,    -1,    78,
      41,    78,    -1,    18,    78,    34,    -1,    38,    -1,    40,
      -1,    39,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    97,    97,   102,   104,   107,   110,   119,   130,   138,
     147,   156,   162,   168,   173,   181,   184,   192,   203,   216,
     223,   230,   237,   242,   252,   287,   312,   337,   358,   362,
     373,   379,   383,   389,   415,   420,   428,   434,   439,   445,
     460,   472,   479,   487,   505,   511,   522,   530,   536,   543,
     547,   551,   555,   559,   563,   569,   610,   650,   688,   726,
     741,   756,   763,   773,   786
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "AND", "ASSIGN", "COLON", "COMMA", "DEF",
  "ELSE", "DIV", "END", "EQ", "FLOAT", "GE", "GLOBAL", "GT", "IF", "INT",
  "LEFT_PAREN", "LEFT_SQ_BKT", "LE", "LT", "MINUS", "MOD", "MULT", "NE",
  "NOT", "NUL", "OR", "PLUS", "PRINT", "PRODUCT", "READ", "RETURN",
  "RIGHT_PAREN", "RIGHT_SQ_BKT", "SEMICOLON", "WHILE", "ID", "INT_CONST",
  "FLOAT_CONST", "DOT", "FORMAT", "BOOLEAN", "NULL_", "RIGHT_BRACES",
  "LEFT_BRACES", "$accept", "prog", "declList", "decl", "typeList",
  "varList", "var", "type", "stmtListO", "stmtList", "stmt",
  "assignmentStmt", "readStmt", "printStmt", "ifStmt", "elsePart", "M5",
  "M3", "M7", "whileStmt", "expO", "M1", "M2", "M4", "bExp", "M14", "M9",
  "M12", "M15", "M16", "relOP", "exp", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    49,    49,    50,    51,    51,    52,    52,
      53,    54,    54,    54,    54,    55,    55,    56,    56,    57,
      57,    57,    57,    57,    58,    59,    60,    61,    62,    62,
      63,    64,    65,    66,    67,    67,    68,    69,    70,    71,
      71,    71,    71,    71,    72,    73,    74,    75,    76,    77,
      77,    77,    77,    77,    77,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     0,     2,     3,     5,     4,     3,     1,
       1,     1,     1,     1,     1,     0,     1,     3,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     8,     0,     2,
       0,     0,     0,     6,     0,     1,     0,     0,     0,     4,
       4,     3,     4,     3,     0,     0,     0,     0,     0,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     3,     0,     0,    37,     3,     1,    10,     0,     0,
       9,     0,    36,    31,     4,     5,     0,     0,     0,     2,
      31,     0,     0,     0,    18,     0,     0,     0,     0,     0,
       0,    12,    11,    13,    14,     0,     8,    17,    34,    34,
      34,    19,    20,    21,    30,    22,    45,    23,     0,     7,
       0,    62,    64,    63,    26,    35,    25,    24,     0,     0,
       6,     0,     0,     0,     0,     0,     0,     0,    47,    48,
       0,     0,     0,    61,    58,    56,    59,    57,    55,    60,
       0,     0,    44,    37,    46,    49,    52,    53,    50,    51,
      54,     0,    37,    41,     0,     0,    36,     0,    43,    36,
      42,    40,    28,    39,    33,    37,     0,    36,    27
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     4,     5,     8,     9,    10,    35,    11,    12,
      24,    25,    26,    27,    45,   106,    58,    28,   102,    47,
      54,    20,    13,    29,    70,    95,    59,    97,    80,    81,
      91,    71
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -50
static const yytype_int8 yypact[] =
{
     -10,     0,    11,   -19,    12,     0,   -50,   -50,    -1,    28,
      43,    35,    50,    55,   -50,   -50,    64,    -4,   -19,   -50,
      55,    32,    38,    71,   -50,    45,    52,    53,    75,    57,
      -4,   -50,   -50,   -50,   -50,    66,   -50,   -50,    61,    61,
      61,   -50,   -50,   -50,   -50,   -50,   -50,   -50,    67,   -50,
      61,   -50,   -50,   -50,   -50,    -6,   -50,   -50,    58,    58,
     -50,    49,    61,    61,    61,    61,    61,    61,   -50,   -50,
      31,    41,    39,   -50,    36,    -3,    36,    36,    -3,   -50,
      58,    58,   -50,   -50,   -50,   -50,   -50,   -50,   -50,   -50,
     -50,    61,   -50,    40,     2,    58,    47,    58,    -6,    95,
     -50,    40,    98,    40,   -50,   -50,    97,    99,   -50
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -50,   -50,   105,   -50,   -50,    33,   -50,    81,   -50,     3,
      92,   -50,   -50,   -50,   -50,   -50,   -50,   -50,   -50,   -50,
     -25,   -50,   -50,   -50,   -49,   -50,   -50,   -50,   -50,   -50,
     -50,   -38
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -39
static const yytype_int8 yytable[] =
{
      55,    55,    55,    62,     1,    82,    62,     3,    31,    15,
      72,     6,    61,    32,    56,    57,    63,    64,    65,     7,
      64,    65,   -15,    66,    74,    75,    76,    77,    78,    79,
      84,    93,    94,    17,    82,    67,    83,     7,    67,    33,
      34,    16,    82,    82,    92,    19,   101,   100,   103,    18,
      62,    36,    85,    98,    86,   -32,    87,   -32,    62,    84,
     -16,    88,    89,    63,    64,    65,    90,    84,    84,    30,
      66,    63,    64,    65,    38,    40,    50,    67,    66,    50,
      39,    41,    67,    73,    68,    21,    96,    22,    42,    43,
      67,    44,   -38,    23,    46,    99,    51,    52,    53,    51,
      52,    53,    49,    60,    69,   104,   105,   108,   107,   -29,
      14,    48,    37
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-50))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_uint8 yycheck[] =
{
      38,    39,    40,     9,    14,     3,     9,     7,    12,    10,
      59,     0,    50,    17,    39,    40,    22,    23,    24,    38,
      23,    24,    10,    29,    62,    63,    64,    65,    66,    67,
      28,    80,    81,     5,     3,    41,     5,    38,    41,    43,
      44,     8,     3,     3,     5,    10,    95,    45,    97,     6,
       9,    18,    11,    91,    13,     8,    15,    10,     9,    28,
      10,    20,    21,    22,    23,    24,    25,    28,    28,     5,
      29,    22,    23,    24,    42,     4,    18,    41,    29,    18,
      42,    36,    41,    34,    26,    30,    83,    32,    36,    36,
      41,    16,    37,    38,    37,    92,    38,    39,    40,    38,
      39,    40,    36,    36,    46,    10,     8,    10,   105,    10,
       5,    30,    20
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    14,    48,     7,    49,    50,     0,    38,    51,    52,
      53,    55,    56,    69,    49,    10,    52,     5,     6,    10,
      68,    30,    32,    38,    57,    58,    59,    60,    64,    70,
       5,    12,    17,    43,    44,    54,    52,    57,    42,    42,
       4,    36,    36,    36,    16,    61,    37,    66,    54,    36,
      18,    38,    39,    40,    67,    78,    67,    67,    63,    73,
      36,    78,     9,    22,    23,    24,    29,    41,    26,    46,
      71,    78,    71,    34,    78,    78,    78,    78,    78,    78,
      75,    76,     3,     5,    28,    11,    13,    15,    20,    21,
      25,    77,     5,    71,    71,    72,    56,    74,    78,    56,
      45,    71,    65,    71,    10,     8,    62,    56,    10
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 98 "parser.y"
    {
		saveSymTab();
	}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 111 "parser.y"
    {
		setElt *temp=(yyvsp[(2) - (5)].S).head;
		while(temp!=NULL){
			insertSymTab(temp->varname,(yyvsp[(4) - (5)].type),&offset);
			temp=temp->next;
		}
		deleteSet(&((yyvsp[(2) - (5)].S)));
	}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 120 "parser.y"
    {
		setElt *temp=(yyvsp[(1) - (4)].S).head;
		while(temp!=NULL){
			insertSymTab(temp->varname,(yyvsp[(3) - (4)].type),&offset);
			temp=temp->next;
		}
		deleteSet(&((yyvsp[(1) - (4)].S)));
	}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 131 "parser.y"
    {
		insertSet(&((yyvsp[(3) - (3)].S)),(yyvsp[(1) - (3)].S).head->varname);
		deleteSet(&((yyvsp[(1) - (3)].S)));
		moveSet(&((yyval.S)),&((yyvsp[(3) - (3)].S)));
		//printf("\nvarList -> var COMMA varList");
		//printSet(&($$));
	}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 139 "parser.y"
    {
		initSet(&((yyval.S)));
		insertSet(&((yyval.S)),(yyvsp[(1) - (1)].S).head->varname);
		deleteSet(&((yyvsp[(1) - (1)].S)));
		//printf("\nVARLIST --> var {%s}",$$.head->varname);
	}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 148 "parser.y"
    {
		//printf("\nID = {%s}",$1);
		initSet(&((yyval.S)));
		insertSet(&((yyval.S)),(yyvsp[(1) - (1)].id));
		//printf("\nSET = {%s}",$$.head->varname);
	}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 157 "parser.y"
    {
		//test_symtab();
		strcpy((yyval.type),(yyvsp[(1) - (1)].type));
		//printf("\nTYPE = %s",$$);
	}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 163 "parser.y"
    {
		strcpy((yyval.type),(yyvsp[(1) - (1)].type));
		//test_symtab();
		//printf("\nTYPE = %s",$$);
	}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 169 "parser.y"
    {
		strcpy((yyval.type),(yyvsp[(1) - (1)].type));
		//printf("\nTYPE = %s",$$);
	}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 174 "parser.y"
    {
		strcpy((yyval.type),(yyvsp[(1) - (1)].type));
		//printf("\nTYPE = %s",$$);
	}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 181 "parser.y"
    {
		//initCode(&($$.C));
	}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 184 "parser.y"
    {
		initCode(&((yyval.STMT).C));
		moveCode(&((yyval.STMT).C),&((yyvsp[(1) - (1)].STMT).C));
		printCode(&((yyval.STMT).C));
		dumpCode(&((yyval.STMT).C));
	}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 193 "parser.y"
    {
		initCode(&((yyval.STMT).C));
		moveCode(&((yyval.STMT).C),&((yyvsp[(1) - (3)].STMT).C));
		char Line[200];
		//sprintf(Line,"%s:",$1.next);
		//addCode(&($$.C),Line);
		appendCodeToLeft(&((yyval.STMT).C),&((yyvsp[(3) - (3)].STMT).C));
		sprintf(Line,"%s:",(yyvsp[(3) - (3)].STMT).next);
		addCode(&((yyval.STMT).C),Line);	
	}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 204 "parser.y"
    {
		//printf("\nLABEL at stmtList = {%s}",$<STMT>1.next);
		initCode(&((yyval.STMT).C));
		moveCode(&((yyval.STMT).C),&((yyvsp[(2) - (2)].STMT).C));
		char Line[100];
		sprintf(Line,"%s:",(yyvsp[(2) - (2)].STMT).next);
		//printf("\nHERE GONE 1 {%s}??\n",Line);
		addCode(&((yyval.STMT).C),Line);
		//printf("\nNO 1\n");	
	}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 217 "parser.y"
    {
		strcpy((yyval.STMT).next,(yyvsp[(0) - (2)].STMT).next);
		initCode(&((yyval.STMT).C));
		moveCode(&((yyval.STMT).C),&((yyvsp[(1) - (2)].EXP).C));
		strcpy((yyval.STMT).type,(yyvsp[(1) - (2)].EXP).type);
	}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 224 "parser.y"
    {
		strcpy((yyval.STMT).next,(yyvsp[(0) - (2)].STMT).next);
		initCode(&((yyval.STMT).C));
		moveCode(&((yyval.STMT).C),&((yyvsp[(1) - (2)].EXP).C));
		strcpy((yyval.STMT).type,(yyvsp[(1) - (2)].EXP).type);
	}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 231 "parser.y"
    {	
		strcpy((yyval.STMT).next,(yyvsp[(0) - (2)].STMT).next);
		initCode(&((yyval.STMT).C));
		moveCode(&((yyval.STMT).C),&((yyvsp[(1) - (2)].EXP).C));
		strcpy((yyval.STMT).type,(yyvsp[(1) - (2)].EXP).type);
	}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 237 "parser.y"
    {
		initCode(&((yyval.STMT).C));
		moveCode(&((yyval.STMT).C),&((yyvsp[(2) - (2)].STMT).C));
		//strcpy($$.next,$2.next);
	}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 242 "parser.y"
    {
		strcpy((yyval.STMT).next,(yyvsp[(2) - (2)].STMT).next);
		//printf("\nINIT WHILE!!");
		initCode(&((yyval.STMT).C));
		//printf("\nWHILE NEXT = {%s},",$<STMT>1.next);
		//printf("\nWHILE HEAD =%p TAIL =%p",$$.C.head,$$.C.tail);
		moveCode(&((yyval.STMT).C),&((yyvsp[(2) - (2)].STMT).C));
	}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 253 "parser.y"
    {
		char idType[20];
		int typeR=getType((yyvsp[(1) - (3)].id),idType);
		if(typeR!=SUCCESS){
			printf("\nERROR ACCESSING UNDEFINED VARIABLE!!\n");
			exit(-1);
		}
		char dummyType[20];
		int compR=compareType(idType,(yyvsp[(3) - (3)].EXP).type,dummyType);
		//printf("\ncompR = %d",compR);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char Line[200];
		if(compR!=1){
			if(compR==10){
				printf("\nERROR CANT STORE FLOAT IN INT");
				exit(-1);
			}
			if(compR==20){
				sprintf(Line,"%s = (float) %s",(yyvsp[(1) - (3)].id),(yyvsp[(3) - (3)].EXP).ADDR);
			}
		}else{
			sprintf(Line,"%s = %s",(yyvsp[(1) - (3)].id),(yyvsp[(3) - (3)].EXP).ADDR);
		}
		initCode(&((yyval.EXP).C));
		moveCode(&((yyval.EXP).C),&((yyvsp[(3) - (3)].EXP).C));
		addCode(&((yyval.EXP).C),Line);
		//printf("\nASSIGNMENTSTMT ---> ");
		//printCode(&($$.C));
	}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 288 "parser.y"
    {
		initCode(&((yyval.EXP).C));
		char dummyType[20];
		int compR=compareType((yyvsp[(2) - (3)].type),(yyvsp[(3) - (3)].EXP).type,dummyType);
		if(compR!=1){
			printf("\nERROR FORMAT MISSMATCH IN READ FORMAT = {%s} EXPTYPE = {%s}!!",(yyvsp[(2) - (3)].type),(yyvsp[(3) - (3)].EXP).type);
			exit(-1);
		}
		strcpy((yyval.EXP).type,dummyType);
		char Line[200];
		moveCode(&((yyval.EXP).C),&((yyvsp[(3) - (3)].EXP)).C);
		sprintf(Line,"param %s",(yyvsp[(3) - (3)].EXP).ADDR);
		addCode(&((yyval.EXP).C),Line);
		if(isEqual(dummyType,"int")){
			sprintf(Line,"call readInt",(yyvsp[(3) - (3)].EXP).ADDR);
		}else{
			sprintf(Line,"call readFloat",(yyvsp[(3) - (3)].EXP).ADDR);
		}
		addCode(&((yyval.EXP).C),Line);
		//printf("\nREADSTMT ---> ");
		//printCode(&($$.C));
	}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 313 "parser.y"
    {
		initCode(&((yyval.EXP).C));
		char dummyType[20];
		int compR=compareType((yyvsp[(2) - (3)].type),(yyvsp[(3) - (3)].EXP).type,dummyType);
		if(compR!=1){
			printf("\nERROR FORMAT MISSMATCH IN READ FORMAT = {%s} EXPTYPE = {%s}!!",(yyvsp[(2) - (3)].type),(yyvsp[(3) - (3)].EXP).type);
			exit(-1);
		}
		strcpy((yyval.EXP).type,dummyType);
		char Line[200];
		moveCode(&((yyval.EXP).C),&((yyvsp[(3) - (3)].EXP)).C);
		sprintf(Line,"param %s",(yyvsp[(3) - (3)].EXP).ADDR);
		addCode(&((yyval.EXP).C),Line);
		if(isEqual(dummyType,"int")){
			sprintf(Line,"call printInt",(yyvsp[(3) - (3)].EXP).ADDR);
		}else{
			sprintf(Line,"call printFloat",(yyvsp[(3) - (3)].EXP).ADDR);
		}
		addCode(&((yyval.EXP).C),Line);
		//printf("\nREADSTMT ---> ");
		//printCode(&($$.C));
	}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 337 "parser.y"
    {
		//ifStmt.Code=bExp.Code||Label(bExp.True)||stmtList.Code||
		//gen(‘goto’ ifstmt.Next||Label(bExp.False)||elsePart.Code)
		printf("\nIN IFSTMT FALSE = {%s}",(yyvsp[(3) - (8)].B).False);
		printf("\nINIF READ ADDRESS = {%p}",&(yyval.STMT));
		initCode(&((yyval.STMT).C));
		moveCode(&((yyval.STMT).C),&((yyvsp[(3) - (8)].B).C));
		char Line[100];
		sprintf(Line,"%s:",(yyvsp[(3) - (8)].B).True);
		addCode(&((yyval.STMT).C),Line);
		appendCodeToLeft(&((yyval.STMT).C),&((yyvsp[(5) - (8)].STMT).C));
		sprintf(Line,"goto %s;",(yyvsp[(-1) - (8)].STMT).next);
		addCode(&((yyval.STMT).C),Line);
		sprintf(Line,"%s:",(yyvsp[(3) - (8)].B).False);
		addCode(&((yyval.STMT).C),Line);
		appendCodeToLeft(&((yyval.STMT).C),&((yyvsp[(7) - (8)].STMT).C));
		//strcpy($$.next,)
	}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 358 "parser.y"
    {
		initCode(&((yyval.STMT).C));
		//strcpy($$.next,"");
	}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 363 "parser.y"
    {
		//stmtList.Next=elsePart.Next
		//elsePart.Code=stmtList.Code||Gen(‘goto’ elsePart.Next)
		initCode(&((yyval.STMT).C));
		moveCode(&((yyval.STMT).C),&((yyvsp[(2) - (2)].STMT).C));
		char Line[100];
		sprintf(Line,"goto %s",(yyvsp[(0) - (2)].STMT).next);
		addCode(&((yyval.STMT).C),Line);
	}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 373 "parser.y"
    {
		getNewLabel((yyval.B).True);
		getNewLabel((yyval.B).False);
		printf("\nWE ARE HERE 333!!");
	}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 379 "parser.y"
    {
		strcpy((yyval.STMT).next,(yyvsp[(0) - (0)].STMT).next);
	}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 383 "parser.y"
    {
		printf("\nIFNEXT = {%s}",(yyvsp[(-5) - (0)].STMT).next);
		strcpy((yyval.STMT).next,(yyvsp[(-5) - (0)].STMT).next);
	}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 390 "parser.y"
    {
		//printf("\nWHILE NEXT = {%s}",$<STMT>0.next);
		//printf("\nstmtNext = {%s}",$5.next);
		initCode(&((yyval.STMT).C));
		//whileStmt.Code=Label(stmtList.Next)||bExp.Code||
		//Label(bExp.True)||stmtList.Code||Gen(goto stmtList.Next)
		char Line[200];
		char begin[50];
		getNewLabel(begin);
		sprintf(Line,"%s:",begin);
		addCode(&((yyval.STMT).C),Line);
		appendCodeToLeft(&((yyval.STMT).C),&((yyvsp[(3) - (6)].B).C));
		char Label[50];
		sprintf(Label,"%s",(yyvsp[(3) - (6)].B).True);
		sprintf(Line,"%s:",Label);
		addCode(&((yyval.STMT).C),Line);
		appendCodeToLeft(&((yyval.STMT).C),&((yyvsp[(5) - (6)].STMT).C));
		sprintf(Line,"goto %s",begin);
		addCode(&((yyval.STMT).C),Line);
		strcpy((yyval.STMT).next,(yyvsp[(3) - (6)].B).False);
		printf("\nWHILE FALSE = {%s}",(yyval.STMT).next);
	}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 415 "parser.y"
    {
		initCode(&((yyval.EXP).C));
		strcpy((yyval.EXP).ADDR,"null");
		strcpy((yyval.EXP).type,"null");
	}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 421 "parser.y"
    {
		initCode(&((yyval.EXP).C));
		strcpy((yyval.EXP).ADDR,(yyvsp[(1) - (1)].EXP).ADDR);
		strcpy((yyval.EXP).type,(yyvsp[(1) - (1)].EXP).type);
		moveCode(&((yyval.EXP).C),&((yyvsp[(1) - (1)].EXP).C));
	}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 428 "parser.y"
    {
	getNewLabel((yyval.STMT).next);
	//printf("\nLABEL FROM M1 : {%s}",$$.next);
}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 434 "parser.y"
    {
	getNewLabel((yyval.STMT).next);
	//printf("\nLABEL FROM M2 : {%s}",$$.next);
}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 439 "parser.y"
    {
	printf("\nNEXT LABEL = {%s}",(yyvsp[(0) - (0)].STMT).next);
	strcpy((yyval.STMT).next,(yyvsp[(0) - (0)].STMT).next);
}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 446 "parser.y"
    {
		moveCode(&((yyval.B).C),&((yyvsp[(1) - (4)].B).C));
		char Line[200];
		sprintf(Line,"%s:",(yyvsp[(1) - (4)].B).False);
		addCode(&((yyval.B).C),Line);
		appendCodeToLeft(&((yyval.B).C),&((yyvsp[(4) - (4)].B).C));
		strcpy((yyval.B).True,(yyvsp[(0) - (4)].B).True);
		//printf("\nWE HAVE ={%s}",$4.False);
		strcpy((yyvsp[(0) - (4)].B).False,(yyvsp[(4) - (4)].B).False);
		strcpy((yyval.B).False,(yyvsp[(4) - (4)].B).False);
		//strcpy($$.False,"2434");
		//printf("\nWE HAVE ={%s}",$$.False);
		//printf("\nINBEXP WRITE ADDRESS = {%p}",&$$);
	}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 461 "parser.y"
    {
		initCode(&((yyval.B).C));
		//bExp.Code=bExp1.Code||Label(bExp1.True)||bExp2.Code
		moveCode(&((yyval.B).C),&((yyvsp[(1) - (4)].B).C));
		char Line[200];
		sprintf(Line,"%s:",(yyvsp[(1) - (4)].B).True);
		addCode(&((yyval.B).C),Line);
		appendCodeToLeft(&((yyval.B).C),&((yyvsp[(4) - (4)].B).C));
		strcpy((yyval.B).True,(yyvsp[(4) - (4)].B).True);
		//strcpy($$.False,$<B>0.False);
	}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 473 "parser.y"
    {
		initCode(&((yyval.B).C));
		moveCode(&((yyval.B).C),&((yyvsp[(3) - (3)].B).C));
		strcpy((yyval.B).True,(yyvsp[(3) - (3)].B).False);
		strcpy((yyval.B).False,(yyvsp[(3) - (3)].B).True);
	}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 480 "parser.y"
    {
		printf("\nHERE !!");
		initCode(&((yyval.B).C));
		moveCode(&((yyval.B).C),&((yyvsp[(3) - (4)].B).C));
		strcpy((yyval.B).True,(yyvsp[(3) - (4)].B).True);
		strcpy((yyval.B).False,(yyvsp[(3) - (4)].B).False);
	}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 488 "parser.y"
    {
		//printf("\nRELOP = {%s}",$2);
		printf("\nBEXP TRUE = {%s}",(yyvsp[(0) - (3)].B).True);
		printf("\nBEXP FALSE = {%s}",(yyvsp[(0) - (3)].B).False);
		initCode(&((yyval.B).C));
		concatCode(&((yyval.B).C),&((yyvsp[(1) - (3)].EXP).C),&((yyvsp[(3) - (3)].EXP).C));
		char Line[200];
		sprintf(Line,"if %s %s %s goto %s",(yyvsp[(1) - (3)].EXP).ADDR,(yyvsp[(2) - (3)].OP),(yyvsp[(3) - (3)].EXP).ADDR,(yyvsp[(0) - (3)].B).True);
		addCode(&((yyval.B).C),Line);
		sprintf(Line,"goto %s",(yyvsp[(0) - (3)].B).False);
		addCode(&((yyval.B).C),Line);
		printf("\nMAY BE ME TOO !!");
		strcpy((yyval.B).True,(yyvsp[(0) - (3)].B).True);
		strcpy((yyval.B).False,(yyvsp[(0) - (3)].B).False);
	}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 505 "parser.y"
    {
		//strcpy($$.True,$<B>-2.True);
		getNewLabel((yyval.B).True);
		strcpy((yyval.B).False,(yyvsp[(-2) - (0)].B).False);
	}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 511 "parser.y"
    {

		//printf("\nBFALSE = {%s}",$<STMT>-1.next);
		getNewLabel((yyval.B).True);
		strcpy((yyval.B).False,(yyvsp[(-1) - (0)].STMT).next);
		printf("\nB.TRUE = {%s}",(yyval.B).True);
		printf("\nB.FALSE = {%s}",(yyval.B).False);
	}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 522 "parser.y"
    {
		strcpy((yyval.B).True,(yyvsp[(-2) - (0)].B).True);
		//strcpy($$.False,$<B>-2.False);
		getNewLabel((yyval.B).False);
		printf("\nnew FALSE LABEL ={%s}",(yyval.B).False);
	}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 530 "parser.y"
    {
		strcpy((yyval.B).True,(yyvsp[(-1) - (0)].B).False);
		strcpy((yyval.B).False,(yyvsp[(-1) - (0)].B).True);
	}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 536 "parser.y"
    {
		strcpy((yyval.B).True,(yyvsp[(-1) - (0)].B).True);
		strcpy((yyval.B).False,(yyvsp[(-1) - (0)].B).False);
		printf("\nM16 : TRUE = {%s} FALSE = {%s}",(yyval.B).True,(yyval.B).False);
	}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 544 "parser.y"
    {
		strcpy((yyval.OP),"EQ");
	}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 548 "parser.y"
    {
		strcpy((yyval.OP),"LE");
	}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 552 "parser.y"
    {
		strcpy((yyval.OP),"LT");
	}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 556 "parser.y"
    {
		strcpy((yyval.OP),"GE");
	}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 560 "parser.y"
    {
		strcpy((yyval.OP),"GT");
	}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 564 "parser.y"
    {
		strcpy((yyval.OP),"NE");
	}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 570 "parser.y"
    {
		//exp.addr=newTemp()
		//exp.code=exp1.code||exp2.code||Gen(exp1.addr OP exp2.addr)
		getNewTemp((yyval.EXP).ADDR);
		initCode(&((yyval.EXP).C));
		char type[20];
		int compR=compareType((yyvsp[(1) - (3)].EXP).type,(yyvsp[(3) - (3)].EXP).type,type);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char typeLine[200];
		char typeVar[20];
		typeLine[0]='\0';
		char line[200];
		if(compR!=1){
			// we need to do explicit type casting
			getNewTemp(typeVar);
			insertSymTab(typeVar,"float",&offset);
			insertSymTab((yyval.EXP).ADDR,"float",&offset);
			strcpy((yyval.EXP).type,"float");
			if(compR==10){
				sprintf(typeLine,"%s = (float) %s",typeVar,(yyvsp[(1) - (3)].EXP).ADDR);
				sprintf(line,"%s = %s + %s",(yyval.EXP).ADDR,typeVar,(yyvsp[(3) - (3)].EXP).ADDR);
			}
			if(compR==20){
				sprintf(typeLine,"%s = (float) %s",typeVar,(yyvsp[(3) - (3)].EXP).ADDR);
				sprintf(line,"%s = %s + %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,typeVar);
			}
		}else{
			insertSymTab((yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).type,&offset);
			strcpy((yyval.EXP).type,(yyvsp[(1) - (3)].EXP).type);
			sprintf(line,"%s = %s + %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,(yyvsp[(3) - (3)].EXP).ADDR);
		}
		concatCode(&((yyval.EXP).C),&((yyvsp[(1) - (3)].EXP)).C,&((yyvsp[(3) - (3)].EXP)).C);
		if(compR!=1){
			addCode(&((yyval.EXP).C),typeLine);
		}
		addCode(&((yyval.EXP).C),line);
	}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 611 "parser.y"
    {
		//exp.addr=newTemp()
		//exp.code=exp1.code||exp2.code||Gen(exp1.addr OP exp2.addr)
		getNewTemp((yyval.EXP).ADDR);
		char type[20];
		int compR=compareType((yyvsp[(1) - (3)].EXP).type,(yyvsp[(3) - (3)].EXP).type,type);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char typeLine[200];
		char typeVar[20];
		typeLine[0]='\0';
		char line[200];
		if(compR!=1){
			// we need to do explicit type casting
			getNewTemp(typeVar);
			insertSymTab(typeVar,"float",&offset);
			insertSymTab((yyval.EXP).ADDR,"float",&offset);
			strcpy((yyval.EXP).type,"float");
			if(compR==10){
				sprintf(typeLine,"%s = (float) %s",typeVar,(yyvsp[(1) - (3)].EXP).ADDR);
				sprintf(line,"%s = %s - %s",(yyval.EXP).ADDR,typeVar,(yyvsp[(3) - (3)].EXP).ADDR);
			}
			if(compR==20){
				sprintf(typeLine,"%s = (float) %s",typeVar,(yyvsp[(3) - (3)].EXP).ADDR);
				sprintf(line,"%s = %s - %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,typeVar);
			}
		}else{
			insertSymTab((yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).type,&offset);
			strcpy((yyval.EXP).type,(yyvsp[(1) - (3)].EXP).type);
			sprintf(line,"%s = %s - %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,(yyvsp[(3) - (3)].EXP).ADDR);
		}
		concatCode(&((yyval.EXP).C),&((yyvsp[(1) - (3)].EXP)).C,&((yyvsp[(3) - (3)].EXP)).C);
		if(compR!=1){
			addCode(&((yyval.EXP).C),typeLine);
		}
		addCode(&((yyval.EXP).C),line);
	}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 651 "parser.y"
    {
		getNewTemp((yyval.EXP).ADDR);
		char type[20];
		int compR=compareType((yyvsp[(1) - (3)].EXP).type,(yyvsp[(3) - (3)].EXP).type,type);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char typeLine[200];
		char typeVar[20];
		typeLine[0]='\0';
		char line[200];
		if(compR!=1){
			// we need to do explicit type casting
			getNewTemp(typeVar);
			insertSymTab(typeVar,"float",&offset);
			insertSymTab((yyval.EXP).ADDR,"float",&offset);
			strcpy((yyval.EXP).type,"float");
			if(compR==10){
				sprintf(typeLine,"%s = (float) %s",typeVar,(yyvsp[(1) - (3)].EXP).ADDR);
				sprintf(line,"%s = %s * %s",(yyval.EXP).ADDR,typeVar,(yyvsp[(3) - (3)].EXP).ADDR);
			}
			if(compR==20){
				sprintf(typeLine,"%s = (float) %s",typeVar,(yyvsp[(3) - (3)].EXP).ADDR);
				sprintf(line,"%s = %s * %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,typeVar);
			}
		}else{
			insertSymTab((yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).type,&offset);
			strcpy((yyval.EXP).type,(yyvsp[(1) - (3)].EXP).type);
			sprintf(line,"%s = %s * %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,(yyvsp[(3) - (3)].EXP).ADDR);
		}
		concatCode(&((yyval.EXP).C),&((yyvsp[(1) - (3)].EXP)).C,&((yyvsp[(3) - (3)].EXP)).C);
		if(compR!=1){
			addCode(&((yyval.EXP).C),typeLine);
		}
		addCode(&((yyval.EXP).C),line);
	}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 689 "parser.y"
    {
		getNewTemp((yyval.EXP).ADDR);
		char type[20];
		int compR=compareType((yyvsp[(1) - (3)].EXP).type,(yyvsp[(3) - (3)].EXP).type,type);
		if(compR==0){
			printf("\nERROR UNCOMPATABLE DATATYPE!!");
			exit(-1);
		}
		char typeLine[200];
		char typeVar[20];
		typeLine[0]='\0';
		char line[200];
		if(compR!=1){
			// we need to do explicit type casting
			getNewTemp(typeVar);
			insertSymTab(typeVar,"float",&offset);
			insertSymTab((yyval.EXP).ADDR,"float",&offset);
			strcpy((yyval.EXP).type,"float");
			if(compR==10){
				sprintf(typeLine,"%s = (float) %s",typeVar,(yyvsp[(1) - (3)].EXP).ADDR);
				sprintf(line,"%s = %s / %s",(yyval.EXP).ADDR,typeVar,(yyvsp[(3) - (3)].EXP).ADDR);
			}
			if(compR==20){
				sprintf(typeLine,"%s = (float) %s",typeVar,(yyvsp[(3) - (3)].EXP).ADDR);
				sprintf(line,"%s = %s / %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,typeVar);
			}
		}else{
			insertSymTab((yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).type,&offset);
			strcpy((yyval.EXP).type,(yyvsp[(1) - (3)].EXP).type);
			sprintf(line,"%s = %s / %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,(yyvsp[(3) - (3)].EXP).ADDR);
		}
		concatCode(&((yyval.EXP).C),&((yyvsp[(1) - (3)].EXP)).C,&((yyvsp[(3) - (3)].EXP)).C);
		if(compR!=1){
			addCode(&((yyval.EXP).C),typeLine);
		}
		addCode(&((yyval.EXP).C),line);
	}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 727 "parser.y"
    {
		getNewTemp((yyval.EXP).ADDR);
		insertSymTab((yyval.EXP).ADDR,"int",&offset);
		char type[20];
		char line[200];
		if(isEqual((yyvsp[(1) - (3)].EXP).type,"int")==0||isEqual((yyvsp[(3) - (3)].EXP).type,"int")==0){
			printf("\nERROR MOD can be done with integer only !!");
			exit(-1);
		}
		concatCode(&((yyval.EXP).C),&((yyvsp[(1) - (3)].EXP)).C,&((yyvsp[(3) - (3)].EXP)).C);
		sprintf(line,"%s = %s MOD %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,(yyvsp[(3) - (3)].EXP).ADDR);
		addCode(&((yyval.EXP).C),line);
		strcpy((yyval.EXP).type,"int");
	}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 742 "parser.y"
    {
		getNewTemp((yyval.EXP).ADDR);
		insertSymTab((yyval.EXP).ADDR,"int",&offset);
		char type[20];
		char line[200];
		if(isEqual((yyvsp[(1) - (3)].EXP).type,"int")==0||isEqual((yyvsp[(3) - (3)].EXP).type,"int")==0){
			printf("\nERROR MOD can be done with integer only !!");
			exit(-1);
		}
		concatCode(&((yyval.EXP).C),&((yyvsp[(1) - (3)].EXP)).C,&((yyvsp[(3) - (3)].EXP)).C);
		sprintf(line,"%s = %s DOT %s",(yyval.EXP).ADDR,(yyvsp[(1) - (3)].EXP).ADDR,(yyvsp[(3) - (3)].EXP).ADDR);
		addCode(&((yyval.EXP).C),line);
		strcpy((yyval.EXP).type,"int");
	}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 757 "parser.y"
    {
		initCode(&((yyval.EXP).C));
		moveCode(&((yyval.EXP).C),&((yyvsp[(2) - (3)].EXP).C));
		strcpy((yyval.EXP).ADDR,(yyvsp[(2) - (3)].EXP).ADDR);
		strcpy((yyval.EXP).type,(yyvsp[(2) - (3)].EXP).type);
	}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 764 "parser.y"
    {
		initCode(&((yyval.EXP).C));
		strcpy((yyval.EXP).ADDR,(yyvsp[(1) - (1)].id));
		if(getType((yyvsp[(1) - (1)].id),(yyval.EXP).type)!=SUCCESS){
			printf("\nERROR ACCESSING UNDEFINED VARIABLE!!\n");
			exit(-1);
		}
		//printf("\nID = {%s} TYPE = {%s}",$$.ADDR,$$.type);
	}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 774 "parser.y"
    {
		getNewTemp((yyval.EXP).ADDR);
		insertSymTab((yyval.EXP).ADDR,"float",&offset);
		char line[200];
		sprintf(line,"%s = %s",(yyval.EXP).ADDR,(yyvsp[(1) - (1)].number));
		//printf("\nLINE = {%s}",line);
		initCode(&((yyval.EXP).C));
		addCode(&((yyval.EXP).C),line);
		strcpy((yyval.EXP).type,"float");
		//printf("\nEXP TYPE = %s",$$.type);
		//printf("\nCODE = %s",(($$.C).head)->Line);
	}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 787 "parser.y"
    {
		getNewTemp((yyval.EXP).ADDR);
		insertSymTab((yyval.EXP).ADDR,"int",&offset);
		char line[200];
		sprintf(line,"%s = %s",(yyval.EXP).ADDR,(yyvsp[(1) - (1)].number));
		//printf("\nLINE = {%s}",line);
		//printf("\nHERE 3\n");
		initCode(&((yyval.EXP).C));
		//printf("\nCODE INIT LINE = {%s}\n",line);
		addCode(&((yyval.EXP).C),line);
		//printf("\nCODE ADDED!!\n");
		strcpy((yyval.EXP).type,"int");
		//printf("\nTEMP = %s %s",$$.ADDR,$1);
	}
    break;



/* Line 1806 of yacc.c  */
#line 2499 "parser.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 802 "parser.y"

int main(void) {
	//extern int yydebug;
	//yydebug = 1;
	initSymTab();
	yyparse();
	printf("\n");
	return 0;
}


