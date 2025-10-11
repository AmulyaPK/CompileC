/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "hello.y"

/* parser.y -- extensive C-like grammar integrated with array symbol and constant tables
   - Populates Symbol and Constant tables (array storage) with required columns.
   - Assumes lexer sets yylval.str for IDENTIFIER and literal tokens.
   - Designed to be fairly complete and extensible.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>

extern int yylineno;   /* maintained by lexer */
extern char *yytext;
extern FILE *yyin;
int yylex();
void yyerror(const char *fmt, ...);

/* Settings for table sizes */
#define MAX_SYMBOLS 1000
#define MAX_CONSTS  1000
#define MAX_REFS_PER_SYM 128
#define MAX_STRLEN 512

/* Symbol table entry (array) */
typedef struct {
    char name[MAX_STRLEN];
    char type[64];            /* declared type or return type */
    char class[32];           /* "variable","function","array","typedef","struct", etc */
    char boundaries[128];     /* optional boundaries list or textual bounds */
    int  array_dimension;     /* basic: number of dims or 0 */
    char params[512];         /* comma-separated parameter list */
    int  proc_def_flag;       /* 1 if function definition */
    int  nesting_level;       /* nesting (0 global) */
    int  declared_line;       /* where declared */
    int  frequency;           /* times referenced */
    int  ref_lines[MAX_REFS_PER_SYM];
    int  ref_count;
} SymbolEntry;

/* Constant table entry (array) */
typedef struct {
    char var_name[MAX_STRLEN];  /* lexeme */
    int  line_num;              /* first line */
    char value[MAX_STRLEN];     /* literal text */
    char type[32];              /* "int","float","char","string" */
    int  count;                 /* occurrences */
    int  ref_lines[128];
    int  ref_count;
} ConstEntry;

/* Global tables */
static SymbolEntry symtab[MAX_SYMBOLS];
static int sym_count = 0;

static ConstEntry consttab[MAX_CONSTS];
static int const_count = 0;

/* current nesting level (increment on entering compound statements) */
static int current_nesting = 0;

/* helper: duplicate string */
static char *xstrdup(const char *s){ if(!s) return NULL; char *r = malloc(strlen(s)+1); if(!r){perror("malloc"); exit(1);} strcpy(r,s); return r; }

/* find symbol index */
static int sym_find_index(const char *name){
    for(int i=0;i<sym_count;i++) if(strcmp(symtab[i].name,name)==0) return i;
    return -1;
}

/* insert or update symbol */
static void sym_insert_or_update(const char *name, const char *type, const char *class,
                                 const char *boundaries, int array_dimension, const char *params,
                                 int proc_def_flag, int nesting, int declared_line)
{
    int idx = sym_find_index(name);
    if(idx >= 0){
        SymbolEntry *s = &symtab[idx];
        s->frequency++;
        if(type && strcmp(type,"-")!=0) strncpy(s->type,type,sizeof(s->type)-1);
        if(class && strcmp(class,"-")!=0) strncpy(s->class,class,sizeof(s->class)-1);
        if(boundaries && strcmp(boundaries,"-")!=0) strncpy(s->boundaries,boundaries,sizeof(s->boundaries)-1);
        if(params && strcmp(params,"-")!=0) strncpy(s->params,params,sizeof(s->params)-1);
        if(proc_def_flag) s->proc_def_flag = 1;
        return;
    }
    if(sym_count >= MAX_SYMBOLS){ fprintf(stderr,"Symbol table overflow\n"); return; }
    SymbolEntry *s = &symtab[sym_count++];
    memset(s,0,sizeof(*s));
    strncpy(s->name,name,sizeof(s->name)-1);
    strncpy(s->type,type?type:"-",sizeof(s->type)-1);
    strncpy(s->class,class?class:"-",sizeof(s->class)-1);
    strncpy(s->boundaries,boundaries?boundaries:"-",sizeof(s->boundaries)-1);
    s->array_dimension = array_dimension;
    strncpy(s->params, params?params:"-", sizeof(s->params)-1);
    s->proc_def_flag = proc_def_flag;
    s->nesting_level = nesting;
    s->declared_line = declared_line;
    s->frequency = 1;
    s->ref_count = 0;
}

/* add reference for a symbol usage */
static void sym_add_reference(const char *name, int line){
    int idx = sym_find_index(name);
    if(idx < 0){
        /* insert placeholder */
        sym_insert_or_update(name,"-","-","-",0,"-",0,current_nesting,-1);
        idx = sym_find_index(name);
        if(idx < 0) return;
    }
    SymbolEntry *s = &symtab[idx];
    s->frequency++;
    if(s->ref_count < MAX_REFS_PER_SYM) s->ref_lines[s->ref_count++] = line;
}

/* constant table helpers */
static int const_find_index(const char *lexeme, const char *type){
    for(int i=0;i<const_count;i++) if(strcmp(consttab[i].var_name,lexeme)==0 && strcmp(consttab[i].type,type)==0) return i;
    return -1;
}

static void const_insert_or_bump(const char *lexeme, const char *type, int line){
    int idx = const_find_index(lexeme,type);
    if(idx >= 0){
        ConstEntry *c = &consttab[idx];
        c->count++;
        if(c->ref_count < 128) c->ref_lines[c->ref_count++] = line;
        return;
    }
    if(const_count >= MAX_CONSTS){ fprintf(stderr,"Constant table overflow\n"); return; }
    ConstEntry *c = &consttab[const_count++];
    memset(c,0,sizeof(*c));
    strncpy(c->var_name, lexeme, sizeof(c->var_name)-1);
    strncpy(c->value, lexeme, sizeof(c->value)-1);
    strncpy(c->type, type, sizeof(c->type)-1);
    c->line_num = line;
    c->count = 1;
    c->ref_count = 0;
    if(c->ref_count < 128) c->ref_lines[c->ref_count++] = line;
}

/* printers */
static void print_symbol_table(void){
    printf("\n================ SYMBOL TABLE ================\n");
    printf("%-20s %-8s %-10s %-12s %-10s %-20s %-4s %-5s %-6s %-6s %s\n",
           "Name","Type","Class","Boundaries","ArrDim","Params","PDef","Nest","DeclLn","Freq","RefLines");
    printf("----------------------------------------------------------------------------------------------------------------\n");
    for(int i=0;i<sym_count;i++){
        SymbolEntry *s = &symtab[i];
        printf("%-20s %-8s %-10s %-12s %-10d %-20s %-4d %-5d %-6d %-6d ",
               s->name, s->type, s->class, s->boundaries, s->array_dimension, s->params, s->proc_def_flag, s->nesting_level, s->declared_line, s->frequency);
        if(s->ref_count==0) printf("-\n");
        else {
            for(int j=0;j<s->ref_count;j++){
                if(j) printf(",");
                printf("%d", s->ref_lines[j]);
            }
            printf("\n");
        }
    }
}

/* print constants */
static void print_const_table(void){
    printf("\n================ CONSTANT TABLE ================\n");
    printf("%-20s %-10s %-12s %-8s %-6s %s\n","VariableName","FirstLine","Value","Type","Count","RefLines");
    printf("-------------------------------------------------------------------------------\n");
    for(int i=0;i<const_count;i++){
        ConstEntry *c = &consttab[i];
        printf("%-20s %-10d %-12s %-8s %-6d ",
               c->var_name, c->line_num, c->value, c->type, c->count);
        if(c->ref_count==0) printf("-\n");
        else {
            for(int j=0;j<c->ref_count;j++){
                if(j) printf(",");
                printf("%d", c->ref_lines[j]);
            }
            printf("\n");
        }
    }
}


#line 257 "hello.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


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
    INT_CONST = 259,               /* INT_CONST  */
    FLOAT_CONST = 260,             /* FLOAT_CONST  */
    STRING_LITERAL = 261,          /* STRING_LITERAL  */
    CHAR_CONST = 262,              /* CHAR_CONST  */
    INCLUDE = 263,                 /* INCLUDE  */
    PREPROCESSOR = 264,            /* PREPROCESSOR  */
    HEADER = 265,                  /* HEADER  */
    UNMATCHEDCHAR = 266,           /* UNMATCHEDCHAR  */
    UNMATCHEDSTRING = 267,         /* UNMATCHEDSTRING  */
    UNMATCHEDCOMMENT = 268,        /* UNMATCHEDCOMMENT  */
    KEYWORD = 269,                 /* KEYWORD  */
    FOR = 270,                     /* FOR  */
    IF = 271,                      /* IF  */
    UNION = 272,                   /* UNION  */
    STRUCT = 273,                  /* STRUCT  */
    PRINTF = 274,                  /* PRINTF  */
    const_expression_opt = 275,    /* const_expression_opt  */
    CONTINUE = 276,                /* CONTINUE  */
    RETURN = 277,                  /* RETURN  */
    ENUM = 278,                    /* ENUM  */
    WHILE = 279,                   /* WHILE  */
    BREAK = 280,                   /* BREAK  */
    ELLIPSIS = 281,                /* ELLIPSIS  */
    ELSE = 282,                    /* ELSE  */
    constant_expression_opt = 283, /* constant_expression_opt  */
    LBRACE = 284,                  /* LBRACE  */
    RBRACE = 285,                  /* RBRACE  */
    LPAREN = 286,                  /* LPAREN  */
    RPAREN = 287,                  /* RPAREN  */
    LBRACK = 288,                  /* LBRACK  */
    RBRACK = 289,                  /* RBRACK  */
    SEMI = 290,                    /* SEMI  */
    COMMA = 291,                   /* COMMA  */
    COLON = 292,                   /* COLON  */
    DOT = 293,                     /* DOT  */
    QMARK = 294,                   /* QMARK  */
    OP = 295,                      /* OP  */
    ASSIGN = 296,                  /* ASSIGN  */
    OP_ASSIGN = 297,               /* OP_ASSIGN  */
    RELOP = 298,                   /* RELOP  */
    LOGIC_OP = 299,                /* LOGIC_OP  */
    INCDEC = 300                   /* INCDEC  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 189 "hello.y"

    int num;
    char *str;

#line 354 "hello.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);



/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_INT_CONST = 4,                  /* INT_CONST  */
  YYSYMBOL_FLOAT_CONST = 5,                /* FLOAT_CONST  */
  YYSYMBOL_STRING_LITERAL = 6,             /* STRING_LITERAL  */
  YYSYMBOL_CHAR_CONST = 7,                 /* CHAR_CONST  */
  YYSYMBOL_INCLUDE = 8,                    /* INCLUDE  */
  YYSYMBOL_PREPROCESSOR = 9,               /* PREPROCESSOR  */
  YYSYMBOL_HEADER = 10,                    /* HEADER  */
  YYSYMBOL_UNMATCHEDCHAR = 11,             /* UNMATCHEDCHAR  */
  YYSYMBOL_UNMATCHEDSTRING = 12,           /* UNMATCHEDSTRING  */
  YYSYMBOL_UNMATCHEDCOMMENT = 13,          /* UNMATCHEDCOMMENT  */
  YYSYMBOL_KEYWORD = 14,                   /* KEYWORD  */
  YYSYMBOL_FOR = 15,                       /* FOR  */
  YYSYMBOL_IF = 16,                        /* IF  */
  YYSYMBOL_UNION = 17,                     /* UNION  */
  YYSYMBOL_STRUCT = 18,                    /* STRUCT  */
  YYSYMBOL_PRINTF = 19,                    /* PRINTF  */
  YYSYMBOL_const_expression_opt = 20,      /* const_expression_opt  */
  YYSYMBOL_CONTINUE = 21,                  /* CONTINUE  */
  YYSYMBOL_RETURN = 22,                    /* RETURN  */
  YYSYMBOL_ENUM = 23,                      /* ENUM  */
  YYSYMBOL_WHILE = 24,                     /* WHILE  */
  YYSYMBOL_BREAK = 25,                     /* BREAK  */
  YYSYMBOL_ELLIPSIS = 26,                  /* ELLIPSIS  */
  YYSYMBOL_ELSE = 27,                      /* ELSE  */
  YYSYMBOL_constant_expression_opt = 28,   /* constant_expression_opt  */
  YYSYMBOL_LBRACE = 29,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 30,                    /* RBRACE  */
  YYSYMBOL_LPAREN = 31,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 32,                    /* RPAREN  */
  YYSYMBOL_LBRACK = 33,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 34,                    /* RBRACK  */
  YYSYMBOL_SEMI = 35,                      /* SEMI  */
  YYSYMBOL_COMMA = 36,                     /* COMMA  */
  YYSYMBOL_COLON = 37,                     /* COLON  */
  YYSYMBOL_DOT = 38,                       /* DOT  */
  YYSYMBOL_QMARK = 39,                     /* QMARK  */
  YYSYMBOL_OP = 40,                        /* OP  */
  YYSYMBOL_ASSIGN = 41,                    /* ASSIGN  */
  YYSYMBOL_OP_ASSIGN = 42,                 /* OP_ASSIGN  */
  YYSYMBOL_RELOP = 43,                     /* RELOP  */
  YYSYMBOL_LOGIC_OP = 44,                  /* LOGIC_OP  */
  YYSYMBOL_INCDEC = 45,                    /* INCDEC  */
  YYSYMBOL_46_ = 46,                       /* '*'  */
  YYSYMBOL_YYACCEPT = 47,                  /* $accept  */
  YYSYMBOL_translation_unit = 48,          /* translation_unit  */
  YYSYMBOL_external_declaration_list = 49, /* external_declaration_list  */
  YYSYMBOL_external_declaration = 50,      /* external_declaration  */
  YYSYMBOL_preproc_directive = 51,         /* preproc_directive  */
  YYSYMBOL_declaration = 52,               /* declaration  */
  YYSYMBOL_declaration_specifiers = 53,    /* declaration_specifiers  */
  YYSYMBOL_storage_class_specifier = 54,   /* storage_class_specifier  */
  YYSYMBOL_type_qualifier = 55,            /* type_qualifier  */
  YYSYMBOL_type_specifier = 56,            /* type_specifier  */
  YYSYMBOL_declarator_opt = 57,            /* declarator_opt  */
  YYSYMBOL_struct_declaration_list = 58,   /* struct_declaration_list  */
  YYSYMBOL_struct_declaration = 59,        /* struct_declaration  */
  YYSYMBOL_struct_declarator_list = 60,    /* struct_declarator_list  */
  YYSYMBOL_struct_declarator = 61,         /* struct_declarator  */
  YYSYMBOL_init_declarator_list = 62,      /* init_declarator_list  */
  YYSYMBOL_init_declarator = 63,           /* init_declarator  */
  YYSYMBOL_declarator = 64,                /* declarator  */
  YYSYMBOL_pointer = 65,                   /* pointer  */
  YYSYMBOL_direct_declarator = 66,         /* direct_declarator  */
  YYSYMBOL_parameter_type_list = 67,       /* parameter_type_list  */
  YYSYMBOL_parameter_list = 68,            /* parameter_list  */
  YYSYMBOL_parameter_declaration = 69,     /* parameter_declaration  */
  YYSYMBOL_abstract_declarator = 70,       /* abstract_declarator  */
  YYSYMBOL_direct_abstract_declarator = 71, /* direct_abstract_declarator  */
  YYSYMBOL_initializer = 72,               /* initializer  */
  YYSYMBOL_initializer_list = 73,          /* initializer_list  */
  YYSYMBOL_function_definition = 74,       /* function_definition  */
  YYSYMBOL_statement = 75,                 /* statement  */
  YYSYMBOL_labeled_statement = 76,         /* labeled_statement  */
  YYSYMBOL_compound_statement = 77,        /* compound_statement  */
  YYSYMBOL_block_item_list = 78,           /* block_item_list  */
  YYSYMBOL_block_item = 79,                /* block_item  */
  YYSYMBOL_expression_statement = 80,      /* expression_statement  */
  YYSYMBOL_expression_opt = 81,            /* expression_opt  */
  YYSYMBOL_expression = 82,                /* expression  */
  YYSYMBOL_assignment_expression = 83,     /* assignment_expression  */
  YYSYMBOL_conditional_expression = 84,    /* conditional_expression  */
  YYSYMBOL_logical_or_expression = 85,     /* logical_or_expression  */
  YYSYMBOL_logical_and_expression = 86,    /* logical_and_expression  */
  YYSYMBOL_equality_expression = 87,       /* equality_expression  */
  YYSYMBOL_relational_expression = 88,     /* relational_expression  */
  YYSYMBOL_shift_expression = 89,          /* shift_expression  */
  YYSYMBOL_additive_expression = 90,       /* additive_expression  */
  YYSYMBOL_multiplicative_expression = 91, /* multiplicative_expression  */
  YYSYMBOL_cast_expression = 92,           /* cast_expression  */
  YYSYMBOL_unary_expression = 93,          /* unary_expression  */
  YYSYMBOL_postfix_expression = 94,        /* postfix_expression  */
  YYSYMBOL_primary_expression = 95,        /* primary_expression  */
  YYSYMBOL_argument_expression_list_opt = 96, /* argument_expression_list_opt  */
  YYSYMBOL_argument_expression_list = 97,  /* argument_expression_list  */
  YYSYMBOL_type_name = 98,                 /* type_name  */
  YYSYMBOL_specifier_qualifier_list = 99,  /* specifier_qualifier_list  */
  YYSYMBOL_abstract_declarator_opt = 100,  /* abstract_declarator_opt  */
  YYSYMBOL_io_statement = 101,             /* io_statement  */
  YYSYMBOL_selection_statement = 102,      /* selection_statement  */
  YYSYMBOL_iteration_statement = 103,      /* iteration_statement  */
  YYSYMBOL_jump_statement = 104,           /* jump_statement  */
  YYSYMBOL_constant_expression = 105       /* constant_expression  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   294

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  130
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  222

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   300


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    46,     2,     2,     2,     2,     2,     2,     2,
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
      45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   217,   217,   221,   222,   226,   227,   228,   232,   234,
     240,   244,   251,   252,   253,   254,   258,   263,   268,   270,
     274,   278,   282,   284,   288,   289,   293,   300,   301,   305,
     307,   312,   313,   317,   319,   324,   326,   331,   333,   338,
     340,   342,   344,   351,   352,   353,   357,   358,   362,   366,
     371,   372,   373,   378,   379,   384,   385,   389,   390,   395,
     405,   406,   407,   408,   409,   410,   411,   415,   419,   420,
     423,   425,   429,   430,   435,   438,   440,   444,   445,   449,
     450,   454,   455,   459,   460,   464,   465,   469,   470,   474,
     475,   479,   480,   484,   485,   489,   490,   494,   495,   499,
     500,   501,   505,   506,   507,   508,   512,   513,   514,   515,
     516,   517,   521,   523,   527,   528,   533,   538,   539,   543,
     545,   550,   551,   556,   557,   562,   563,   568,   569,   570,
     575
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "INT_CONST", "FLOAT_CONST", "STRING_LITERAL", "CHAR_CONST", "INCLUDE",
  "PREPROCESSOR", "HEADER", "UNMATCHEDCHAR", "UNMATCHEDSTRING",
  "UNMATCHEDCOMMENT", "KEYWORD", "FOR", "IF", "UNION", "STRUCT", "PRINTF",
  "const_expression_opt", "CONTINUE", "RETURN", "ENUM", "WHILE", "BREAK",
  "ELLIPSIS", "ELSE", "constant_expression_opt", "LBRACE", "RBRACE",
  "LPAREN", "RPAREN", "LBRACK", "RBRACK", "SEMI", "COMMA", "COLON", "DOT",
  "QMARK", "OP", "ASSIGN", "OP_ASSIGN", "RELOP", "LOGIC_OP", "INCDEC",
  "'*'", "$accept", "translation_unit", "external_declaration_list",
  "external_declaration", "preproc_directive", "declaration",
  "declaration_specifiers", "storage_class_specifier", "type_qualifier",
  "type_specifier", "declarator_opt", "struct_declaration_list",
  "struct_declaration", "struct_declarator_list", "struct_declarator",
  "init_declarator_list", "init_declarator", "declarator", "pointer",
  "direct_declarator", "parameter_type_list", "parameter_list",
  "parameter_declaration", "abstract_declarator",
  "direct_abstract_declarator", "initializer", "initializer_list",
  "function_definition", "statement", "labeled_statement",
  "compound_statement", "block_item_list", "block_item",
  "expression_statement", "expression_opt", "expression",
  "assignment_expression", "conditional_expression",
  "logical_or_expression", "logical_and_expression", "equality_expression",
  "relational_expression", "shift_expression", "additive_expression",
  "multiplicative_expression", "cast_expression", "unary_expression",
  "postfix_expression", "primary_expression",
  "argument_expression_list_opt", "argument_expression_list", "type_name",
  "specifier_qualifier_list", "abstract_declarator_opt", "io_statement",
  "selection_statement", "iteration_statement", "jump_statement",
  "constant_expression", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-136)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-22)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     238,  -136,  -136,  -136,    15,    33,    33,   110,   238,  -136,
    -136,  -136,    11,  -136,  -136,   154,  -136,  -136,    60,    84,
    -136,  -136,  -136,    17,  -136,    97,    36,  -136,    -6,     7,
      19,  -136,   154,   154,   103,  -136,  -136,    17,   117,   222,
    -136,    19,   154,   137,    17,   106,  -136,   115,  -136,  -136,
     136,  -136,   145,  -136,  -136,  -136,  -136,  -136,   222,   205,
      77,    77,  -136,  -136,  -136,    59,   134,   143,   146,   153,
     159,   162,  -136,   163,    74,  -136,    48,   174,   158,  -136,
     179,   118,  -136,   178,  -136,  -136,  -136,    10,   189,   193,
     199,   202,   228,   207,   204,  -136,  -136,    11,  -136,  -136,
    -136,  -136,  -136,   208,   206,  -136,  -136,  -136,  -136,  -136,
    -136,    56,     8,  -136,   200,    63,   212,     3,   228,  -136,
    -136,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,  -136,    48,  -136,    27,  -136,   215,  -136,   170,
    -136,  -136,    17,   228,   176,   228,   228,   234,  -136,   214,
     228,  -136,  -136,   228,  -136,   222,  -136,  -136,   228,     3,
     223,  -136,  -136,   119,  -136,  -136,   143,  -136,   153,  -136,
    -136,  -136,  -136,  -136,   225,   224,   105,   226,   215,   233,
    -136,  -136,  -136,  -136,  -136,   227,  -136,   230,    89,    95,
    -136,    98,  -136,  -136,  -136,   228,  -136,   228,  -136,  -136,
     229,   228,   176,   231,   266,   176,  -136,  -136,  -136,   235,
     244,  -136,   240,  -136,   228,   176,   239,   243,  -136,  -136,
     176,  -136
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    21,     8,     9,    16,    22,    22,     0,     2,     3,
       7,     6,     0,    14,    15,    12,     5,    23,     0,     0,
       1,     4,    42,     0,    11,    37,     0,    31,    33,     0,
      36,    13,     0,     0,     0,    38,    10,     0,    70,     0,
      59,    35,    45,     0,     0,     0,    24,     0,    41,    32,
      33,    68,    75,   106,   107,   108,   109,   110,     0,     0,
       0,     0,    34,    55,    79,    81,    83,    85,    87,    89,
      91,    93,    95,    97,    99,   102,     0,     0,    43,    46,
       0,     0,    27,    29,    20,    25,    19,   106,     0,     0,
       0,     0,    75,     0,     0,    69,    72,     0,    73,    60,
      61,    71,    62,     0,    76,    77,    66,    63,    64,    65,
      57,     0,   106,    18,   117,     0,     0,   119,     0,   101,
     100,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     112,     0,   105,     0,    48,    50,    49,    52,    40,     0,
      39,    26,     0,     0,    75,    75,     0,     0,   129,     0,
       0,   128,    74,     0,    56,     0,   118,   111,     0,     0,
      50,   120,   116,     0,    84,    97,    86,    88,    90,    92,
      94,    96,    80,   114,     0,   113,     0,     0,    51,     0,
      44,    47,    28,   130,    30,   106,    67,     0,     0,     0,
     127,     0,    78,    58,    98,     0,   104,     0,   103,    53,
       0,    75,    75,     0,     0,    75,    82,   115,    54,     0,
     123,   121,     0,   125,    75,    75,     0,     0,   124,   122,
      75,   126
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -136,  -136,  -136,   268,  -136,   232,     1,  -136,  -136,   -41,
     271,   245,    99,  -136,   138,  -136,   242,     0,   -21,   -24,
    -136,  -136,   142,   -55,   -34,   -51,  -136,  -136,  -127,  -136,
     254,  -136,  -136,  -136,   -86,   -44,   -39,  -135,  -136,   161,
     164,   165,   160,   166,   167,   -97,   -58,  -136,  -136,  -136,
    -136,  -136,   172,  -136,  -136,  -136,  -136,  -136,  -136
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     7,     8,     9,    10,    11,    44,    13,    14,    15,
      18,    45,    46,    81,    82,    26,    27,    34,    29,    30,
      77,    78,    79,   177,   137,    62,   111,    16,    98,    99,
     100,    52,   101,   102,   103,   104,   105,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,   174,
     175,   116,   117,   162,   106,   107,   108,   109,   184
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      63,    12,   119,   120,    35,    41,   149,   110,   183,    12,
      22,   -21,    28,   -21,    22,   115,    31,   186,   114,    63,
      22,   136,   -21,    38,   -21,   -21,   -21,   -21,   -21,   -18,
      22,   171,   -18,   -18,   159,    39,    17,    50,    23,   -21,
     -21,   -21,    23,    76,    83,   -21,    24,   144,    23,    25,
      42,    22,    43,    97,   -21,   135,   -21,    25,   133,   187,
     206,   194,   161,    25,   165,   165,   165,   165,   165,   165,
     165,    36,    37,   114,   115,   210,   134,   163,   213,   133,
      53,    54,    55,    56,    57,   165,   154,   176,   218,    32,
     172,   173,   155,   221,    25,   157,   160,    50,   121,   153,
     165,   178,   188,   122,   193,   130,   191,   131,   118,     1,
      20,    41,   135,    33,   192,   209,    63,    60,     1,   132,
       4,   202,    61,     5,     6,   153,   178,   203,   217,     4,
     205,   204,     5,     6,   153,    48,    84,   165,   160,   198,
      76,   153,    83,    25,    85,    86,    85,    51,    87,    54,
      55,    56,    57,   141,   142,   153,   195,     1,   207,     4,
      88,    89,     5,     6,    90,    80,    91,    92,     4,    93,
      94,     5,     6,     1,    38,    95,    59,    39,   123,   185,
      54,    55,    56,    57,     4,    60,   124,     5,     6,   125,
      61,    88,    89,   126,   139,    90,   180,    91,    92,   127,
      93,    94,   128,     1,   129,    38,   138,    59,   112,    54,
      55,    56,    57,   140,   113,   143,    60,     5,     6,   113,
     145,    61,     5,     6,   146,    53,    54,    55,    56,    57,
     147,    53,    54,    55,    56,    57,    59,   148,   150,   151,
     189,     1,   153,   152,   158,    60,     2,     3,   179,   190,
      61,    58,     4,    59,   159,     5,     6,   196,   199,    59,
     197,   200,    60,   208,   144,   201,   211,    61,    60,   212,
     214,   215,   216,    61,   219,   220,    21,    19,    47,    49,
     182,   181,    40,   164,    96,   168,   156,   166,     0,   167,
       0,     0,   169,     0,   170
};

static const yytype_int16 yycheck[] =
{
      39,     0,    60,    61,    25,    29,    92,    58,   143,     8,
       3,     3,    12,     3,     3,    59,    15,   144,    59,    58,
       3,    76,    14,    29,    14,    17,    18,    17,    18,    14,
       3,   128,    17,    18,    31,    41,     3,    37,    31,    31,
      32,    31,    31,    42,    44,    35,    35,    37,    31,    46,
      31,     3,    33,    52,    46,    76,    46,    46,    31,   145,
     195,   158,   117,    46,   122,   123,   124,   125,   126,   127,
     128,    35,    36,   114,   118,   202,    76,   121,   205,    31,
       3,     4,     5,     6,     7,   143,    30,   131,   215,    29,
     129,   130,    36,   220,    46,    32,   117,    97,    39,    36,
     158,   135,   146,    44,   155,    31,   150,    33,    31,     3,
       0,   135,   133,    29,   153,   201,   155,    40,     3,    45,
      14,    32,    45,    17,    18,    36,   160,    32,   214,    14,
      32,    36,    17,    18,    36,    32,    30,   195,   159,    34,
     139,    36,   142,    46,    45,    30,    47,    30,     3,     4,
       5,     6,     7,    35,    36,    36,    37,     3,   197,    14,
      15,    16,    17,    18,    19,    28,    21,    22,    14,    24,
      25,    17,    18,     3,    29,    30,    31,    41,    44,     3,
       4,     5,     6,     7,    14,    40,    43,    17,    18,    43,
      45,    15,    16,    40,    36,    19,    26,    21,    22,    40,
      24,    25,    40,     3,    41,    29,    32,    31,     3,     4,
       5,     6,     7,    34,    14,    37,    40,    17,    18,    14,
      31,    45,    17,    18,    31,     3,     4,     5,     6,     7,
      31,     3,     4,     5,     6,     7,    31,    35,    31,    35,
       6,     3,    36,    35,    32,    40,     8,     9,    33,    35,
      45,    29,    14,    31,    31,    17,    18,    32,    32,    31,
      36,    28,    40,    34,    37,    35,    35,    45,    40,     3,
      35,    27,    32,    45,    35,    32,     8,     6,    33,    37,
     142,   139,    28,   122,    52,   125,   114,   123,    -1,   124,
      -1,    -1,   126,    -1,   127
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     8,     9,    14,    17,    18,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    74,     3,    57,    57,
       0,    50,     3,    31,    35,    46,    62,    63,    64,    65,
      66,    53,    29,    29,    64,    65,    35,    36,    29,    41,
      77,    66,    31,    33,    53,    58,    59,    58,    32,    63,
      64,    30,    78,     3,     4,     5,     6,     7,    29,    31,
      40,    45,    72,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    53,    67,    68,    69,
      28,    60,    61,    64,    30,    59,    30,     3,    15,    16,
      19,    21,    22,    24,    25,    30,    52,    53,    75,    76,
      77,    79,    80,    81,    82,    83,   101,   102,   103,   104,
      72,    73,     3,    14,    56,    82,    98,    99,    31,    93,
      93,    39,    44,    44,    43,    43,    40,    40,    40,    41,
      31,    33,    45,    31,    64,    65,    70,    71,    32,    36,
      34,    35,    36,    37,    37,    31,    31,    31,    35,    81,
      31,    35,    35,    36,    30,    36,    99,    32,    32,    31,
      65,    70,   100,    82,    86,    93,    87,    88,    89,    90,
      91,    92,    83,    83,    96,    97,    82,    70,    71,    33,
      26,    69,    61,    84,   105,     3,    75,    81,    82,     6,
      35,    82,    83,    72,    92,    37,    32,    36,    34,    32,
      28,    35,    32,    32,    36,    32,    84,    83,    34,    81,
      75,    35,     3,    75,    35,    27,    32,    81,    75,    35,
      32,    75
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    47,    48,    49,    49,    50,    50,    50,    51,    51,
      52,    52,    53,    53,    53,    53,    54,    55,    56,    56,
      56,    56,    57,    57,    58,    58,    59,    60,    60,    61,
      61,    62,    62,    63,    63,    64,    64,    65,    65,    66,
      66,    66,    66,    67,    67,    67,    68,    68,    69,    69,
      70,    70,    70,    71,    71,    72,    72,    73,    73,    74,
      75,    75,    75,    75,    75,    75,    75,    76,    77,    77,
      78,    78,    79,    79,    80,    81,    81,    82,    82,    83,
      83,    84,    84,    85,    85,    86,    86,    87,    87,    88,
      88,    89,    89,    90,    90,    91,    91,    92,    92,    93,
      93,    93,    94,    94,    94,    94,    95,    95,    95,    95,
      95,    95,    96,    96,    97,    97,    98,    99,    99,   100,
     100,   101,   101,   102,   102,   103,   103,   104,   104,   104,
     105
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       3,     2,     1,     2,     1,     1,     1,     1,     1,     5,
       5,     1,     0,     1,     1,     2,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     2,     1,     1,     2,     4,
       4,     3,     1,     1,     3,     0,     1,     3,     2,     2,
       1,     2,     1,     3,     4,     1,     3,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     3,     2,     3,
       0,     2,     1,     1,     2,     0,     1,     1,     3,     1,
       3,     1,     5,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     4,     1,
       2,     2,     1,     4,     4,     2,     1,     1,     1,     1,
       1,     3,     0,     1,     1,     3,     2,     1,     2,     0,
       1,     5,     7,     5,     7,     5,     9,     3,     2,     2,
       1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 8: /* preproc_directive: INCLUDE  */
#line 232 "hello.y"
                { /* include handled in lexer; nothing to do here */
        printf("[OK][Line %d] Parsed include\n", yylineno); }
#line 1630 "hello.tab.c"
    break;

  case 9: /* preproc_directive: PREPROCESSOR  */
#line 234 "hello.y"
                   { /* raw preprocessor line */ 
        printf("[OK][Line %d] Parsed preprocessor directive\n", yylineno);}
#line 1637 "hello.tab.c"
    break;

  case 10: /* declaration: declaration_specifiers init_declarator_list SEMI  */
#line 241 "hello.y"
        {
            printf("[OK][Line %d] Declaration with initializer list\n", yylineno);
        }
#line 1645 "hello.tab.c"
    break;

  case 11: /* declaration: declaration_specifiers SEMI  */
#line 245 "hello.y"
        {
            printf("[OK][Line %d] Declaration without initializer\n", yylineno);
        }
#line 1653 "hello.tab.c"
    break;

  case 16: /* storage_class_specifier: KEYWORD  */
#line 259 "hello.y"
        { printf("[OK][Line %d] Storage class specifier\n", yylineno); }
#line 1659 "hello.tab.c"
    break;

  case 17: /* type_qualifier: KEYWORD  */
#line 264 "hello.y"
        { printf("[OK][Line %d] Type qualifier\n", yylineno); }
#line 1665 "hello.tab.c"
    break;

  case 18: /* type_specifier: KEYWORD  */
#line 269 "hello.y"
        { printf("[OK][Line %d] Basic type specifier\n", yylineno); }
#line 1671 "hello.tab.c"
    break;

  case 19: /* type_specifier: STRUCT declarator_opt LBRACE struct_declaration_list RBRACE  */
#line 271 "hello.y"
        {
            printf("[OK][Line %d] Struct type specifier\n", yylineno);
        }
#line 1679 "hello.tab.c"
    break;

  case 20: /* type_specifier: UNION declarator_opt LBRACE struct_declaration_list RBRACE  */
#line 275 "hello.y"
        {
            printf("[OK][Line %d] Union type specifier\n", yylineno);
        }
#line 1687 "hello.tab.c"
    break;

  case 21: /* type_specifier: IDENTIFIER  */
#line 279 "hello.y"
        { printf("[OK][Line %d] Typedef or identifier as type specifier\n", yylineno); }
#line 1693 "hello.tab.c"
    break;

  case 26: /* struct_declaration: declaration_specifiers struct_declarator_list SEMI  */
#line 294 "hello.y"
        {
            printf("[OK][Line %d] Struct field declaration\n", yylineno);
        }
#line 1701 "hello.tab.c"
    break;

  case 29: /* struct_declarator: declarator  */
#line 306 "hello.y"
        { printf("[OK][Line %d] Struct field declarator\n", yylineno); }
#line 1707 "hello.tab.c"
    break;

  case 30: /* struct_declarator: declarator COLON constant_expression  */
#line 308 "hello.y"
        { printf("[OK][Line %d] Struct bit-field declarator\n", yylineno); }
#line 1713 "hello.tab.c"
    break;

  case 33: /* init_declarator: declarator  */
#line 318 "hello.y"
        { printf("[OK][Line %d] Declarator without initializer\n", yylineno); }
#line 1719 "hello.tab.c"
    break;

  case 34: /* init_declarator: declarator ASSIGN initializer  */
#line 320 "hello.y"
        { printf("[OK][Line %d] Declarator with initializer\n", yylineno); }
#line 1725 "hello.tab.c"
    break;

  case 35: /* declarator: pointer direct_declarator  */
#line 325 "hello.y"
        { printf("[OK][Line %d] Pointer declarator\n", yylineno); }
#line 1731 "hello.tab.c"
    break;

  case 36: /* declarator: direct_declarator  */
#line 327 "hello.y"
        { printf("[OK][Line %d] Direct declarator\n", yylineno); }
#line 1737 "hello.tab.c"
    break;

  case 37: /* pointer: '*'  */
#line 332 "hello.y"
        { printf("[OK][Line %d] Single pointer *\n", yylineno); }
#line 1743 "hello.tab.c"
    break;

  case 38: /* pointer: '*' pointer  */
#line 334 "hello.y"
        { printf("[OK][Line %d] Multiple pointers ** or more\n", yylineno); }
#line 1749 "hello.tab.c"
    break;

  case 39: /* direct_declarator: direct_declarator LBRACK constant_expression_opt RBRACK  */
#line 339 "hello.y"
        { printf("[OK][Line %d] Array declarator\n", yylineno); }
#line 1755 "hello.tab.c"
    break;

  case 40: /* direct_declarator: direct_declarator LPAREN parameter_type_list RPAREN  */
#line 341 "hello.y"
        { printf("[OK][Line %d] Function declarator with parameters\n", yylineno); }
#line 1761 "hello.tab.c"
    break;

  case 41: /* direct_declarator: LPAREN declarator RPAREN  */
#line 343 "hello.y"
        { printf("[OK][Line %d] Parenthesized declarator\n", yylineno); }
#line 1767 "hello.tab.c"
    break;

  case 42: /* direct_declarator: IDENTIFIER  */
#line 345 "hello.y"
        { printf("[OK][Line %d] Identifier declarator\n", yylineno); }
#line 1773 "hello.tab.c"
    break;

  case 48: /* parameter_declaration: declaration_specifiers declarator  */
#line 363 "hello.y"
        {
            /* add param to parameter list for function symbol when assembling the function - simplified below */
        }
#line 1781 "hello.tab.c"
    break;

  case 59: /* function_definition: declaration_specifiers declarator compound_statement  */
#line 396 "hello.y"
        {
            /* This is where we take the declarator and the type and insert a symbol with proc_def_flag=1 */
            /* To keep code modular, concrete insertion occurs in the declarator actions below when we have type + id */
            printf("[OK][Line %d] Function definition recognized\n", yylineno);
        }
#line 1791 "hello.tab.c"
    break;

  case 106: /* primary_expression: IDENTIFIER  */
#line 512 "hello.y"
                 { sym_add_reference(yytext, yylineno); }
#line 1797 "hello.tab.c"
    break;

  case 107: /* primary_expression: INT_CONST  */
#line 513 "hello.y"
                  { const_insert_or_bump(yytext, "int", yylineno); }
#line 1803 "hello.tab.c"
    break;

  case 108: /* primary_expression: FLOAT_CONST  */
#line 514 "hello.y"
                  { const_insert_or_bump(yytext, "float", yylineno); }
#line 1809 "hello.tab.c"
    break;

  case 109: /* primary_expression: STRING_LITERAL  */
#line 515 "hello.y"
                     { const_insert_or_bump(yytext, "string", yylineno); }
#line 1815 "hello.tab.c"
    break;

  case 110: /* primary_expression: CHAR_CONST  */
#line 516 "hello.y"
                 { const_insert_or_bump(yytext, "char", yylineno); }
#line 1821 "hello.tab.c"
    break;


#line 1825 "hello.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 585 "hello.y"


/* ---------------- epilogue ---------------- */

/* error helper */
void yyerror(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Parse error (line %d): ", yylineno);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

int main(int argc, char **argv){
    if(argc < 2){ fprintf(stderr,"Usage: %s <source.c>\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1],"r");
    if(!f){ perror("fopen"); return 1; }
    yyin = f;

    if(yyparse() == 0){
        printf("Parsing succeeded.\n");
        print_symbol_table();
        print_const_table();
    } else {
        fprintf(stderr,"Parsing failed.\n");
    }

    // yyparse();
    fclose(f);
    return 0;
}

/* helper wrappers for printing (expose the static print functions above) */
// void print_symbol_table(void){ print_symbol_table(); }     /* alias to static function above */
// void print_const_table(void){ print_const_table(); }       /* alias to static function above */
