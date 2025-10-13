/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    NUMBER = 259,                  /* NUMBER  */
    STRING_LITERAL = 260,          /* STRING_LITERAL  */
    CHAR_LITERAL = 261,            /* CHAR_LITERAL  */
    INCLUDE = 262,                 /* INCLUDE  */
    PREPROCESSOR = 263,            /* PREPROCESSOR  */
    INT = 264,                     /* INT  */
    FLOAT = 265,                   /* FLOAT  */
    CHAR = 266,                    /* CHAR  */
    VOID = 267,                    /* VOID  */
    IF = 268,                      /* IF  */
    ELSE = 269,                    /* ELSE  */
    FOR = 270,                     /* FOR  */
    WHILE = 271,                   /* WHILE  */
    RETURN = 272,                  /* RETURN  */
    PRINTF = 273,                  /* PRINTF  */
    SWITCH = 274,                  /* SWITCH  */
    CASE = 275,                    /* CASE  */
    DEFAULT = 276,                 /* DEFAULT  */
    BREAK = 277,                   /* BREAK  */
    STRUCT = 278,                  /* STRUCT  */
    UNION = 279,                   /* UNION  */
    INCREMENT = 280,               /* INCREMENT  */
    DECREMENT = 281,               /* DECREMENT  */
    EQ = 282,                      /* EQ  */
    NEQ = 283,                     /* NEQ  */
    LT = 284,                      /* LT  */
    GT = 285,                      /* GT  */
    LE = 286,                      /* LE  */
    GE = 287,                      /* GE  */
    PLUS = 288,                    /* PLUS  */
    MINUS = 289,                   /* MINUS  */
    MULT = 290,                    /* MULT  */
    DIVIDE = 291,                  /* DIVIDE  */
    ASSIGN = 292,                  /* ASSIGN  */
    OPAREN = 293,                  /* OPAREN  */
    CPAREN = 294,                  /* CPAREN  */
    OCURLY = 295,                  /* OCURLY  */
    CCURLY = 296,                  /* CCURLY  */
    SEMICOLON = 297,               /* SEMICOLON  */
    COMMA = 298,                   /* COMMA  */
    COLON = 299,                   /* COLON  */
    OBRACKET = 300,                /* OBRACKET  */
    CBRACKET = 301,                /* CBRACKET  */
    QUOTE = 302,                   /* QUOTE  */
    UNMATCHEDSTRING = 303,         /* UNMATCHEDSTRING  */
    UNMATCHEDCOMMENT = 304,        /* UNMATCHEDCOMMENT  */
    IFX = 305                      /* IFX  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 211 "parser.y"

    int num;
    char *str;

#line 119 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
