
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
     INT_CONST = 259,
     FLOAT_CONST = 260,
     STRING_LITERAL = 261,
     CHAR_CONST = 262,
     INCLUDE = 263,
     PREPROCESSOR = 264,
     HEADER = 265,
     UNMATCHEDCHAR = 266,
     UNMATCHEDSTRING = 267,
     UNMATCHEDCOMMENT = 268,
     KEYWORD = 269,
     FOR = 270,
     IF = 271,
     UNION = 272,
     STRUCT = 273,
     PRINTF = 274,
     const_expression_opt = 275,
     CONTINUE = 276,
     RETURN = 277,
     ENUM = 278,
     WHILE = 279,
     BREAK = 280,
     ELLIPSIS = 281,
     ELSE = 282,
     constant_expression_opt = 283,
     LBRACE = 284,
     RBRACE = 285,
     LPAREN = 286,
     RPAREN = 287,
     LBRACK = 288,
     RBRACK = 289,
     SEMI = 290,
     COMMA = 291,
     COLON = 292,
     DOT = 293,
     QMARK = 294,
     OP = 295,
     ASSIGN = 296,
     OP_ASSIGN = 297,
     RELOP = 298,
     LOGIC_OP = 299,
     INCDEC = 300
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 197 "hello.y"

    int num;
    char *str;



/* Line 1676 of yacc.c  */
#line 104 "hello.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


