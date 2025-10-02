
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
     IDENTIFIER = 258,
     NUMBER = 259,
     STRING_LITERAL = 260,
     CHAR_LITERAL = 261,
     INCLUDE = 262,
     INT = 263,
     FLOAT = 264,
     CHAR = 265,
     VOID = 266,
     IF = 267,
     ELSE = 268,
     FOR = 269,
     WHILE = 270,
     RETURN = 271,
     PRINTF = 272,
     EQ = 273,
     NEQ = 274,
     LT = 275,
     GT = 276,
     LE = 277,
     GE = 278,
     PLUS = 279,
     MINUS = 280,
     MULT = 281,
     DIVIDE = 282,
     ASSIGN = 283,
     OPAREN = 284,
     CPAREN = 285,
     OCURLY = 286,
     CCURLY = 287,
     SEMICOLON = 288,
     COMMA = 289,
     OBRACKET = 290,
     CBRACKET = 291,
     QUOTE = 292,
     UNMATCHEDSTRING = 293,
     PREPROCESSOR = 294,
     UNMATCHEDCOMMENT = 295
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 185 "parser.y"

    int num;
    char *str;



/* Line 1676 of yacc.c  */
#line 99 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


