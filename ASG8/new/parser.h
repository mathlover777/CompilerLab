/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
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
     FORMAT = 297
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 13 "parser.y"

	char	*sval;



/* Line 2068 of yacc.c  */
#line 98 "parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


