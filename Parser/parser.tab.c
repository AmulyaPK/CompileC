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
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 1000
#define MAX_CONSTS 500
#define MAX_REFS_PER_SYM 128
#define MAX_STRLEN 512

void yyerror(const char *s);
int yylex();
extern int yylineno;
int current_nesting = 0;

/* Global flags to track if we are inside a struct/union definition */
static int g_in_struct_def = 0;
static int g_in_union_def = 0; /* NEW */

// ---------------- SYMBOL TABLE ----------------
typedef struct Symbol {
    char name[MAX_STRLEN];
    char token[64];
    char class[32];
    char boundaries[128];
    char array_dims[128];
    char params[256];
    int proc_def;
    int nesting_level;
    int declared_line;
    int frequency;
    int ref_lines[MAX_REFS_PER_SYM];
    int ref_count;
} Symbol;

static Symbol symtab[MAX_SYMBOLS];
static int sym_count = 0;

// ---------------- CONSTANT TABLE ----------------
typedef struct Constant {
    char name[MAX_STRLEN];
    int first_line;
    char value[MAX_STRLEN];
    char type[32];
    int count;
    int ref_lines[128];
    int ref_count;
} Constant;

static Constant consttab[MAX_CONSTS];
static int const_count = 0;

/* ---------------- Helpers ---------------- */
static char *xstrdup(const char *s) {
    if(!s) return NULL;
    char *p = malloc(strlen(s)+1);
    if(!p){ perror("malloc"); exit(1); }
    strcpy(p,s);
    return p;
}

/* find symbol index */
static int sym_find_index(const char *name) {
    for(int i = 0 ; i < sym_count ; i++) {
        if (strcmp(symtab[i].name, name) == 0)
            return i;
    }
    return -1;
}

/* add or update symbol */
static void sym_insert_or_update (const char *name, const char *token, const char *class,
                                 const char *boundaries, const char *array_dims, const char *params,
                                 int proc_def, int nesting, int declared_line)
{
    int idx = sym_find_index(name);
    if (idx >= 0) {
        Symbol *s = &symtab[idx];
        s->frequency++;
        if (token && strcmp(token, "-") != 0)
            strncpy(s->token, token, sizeof(s->token) - 1);
        if (class && strcmp(class, "-") != 0)
            strncpy(s->class, class, sizeof(s->class) - 1);
        if (boundaries && strcmp(boundaries, "-") != 0)
            strncpy(s->boundaries, boundaries, sizeof(s->boundaries) - 1);
        if (array_dims && strcmp(array_dims, "-") != 0)
            strncpy(s->array_dims, array_dims, sizeof(s->array_dims) - 1);
        if (params && strcmp(params, "-") != 0)
            strncpy(s->params, params, sizeof(s->params) - 1);
        if (proc_def)
            s->proc_def = 1;
        if (s->ref_count < MAX_REFS_PER_SYM && declared_line != -1)
            s->ref_lines[s->ref_count++] = declared_line;
        return;
    }
    if(sym_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Symbol table full, cannot insert %s\n", name);
        return;
    }
    Symbol *s = &symtab[sym_count++];
    memset(s, 0, sizeof(*s));
    strncpy(s->name, name, sizeof(s->name) - 1);
    strncpy(s->token, token ? token : "-", sizeof(s->token) - 1);
    strncpy(s->class, class ? class : "-", sizeof(s->class) - 1);
    strncpy(s->boundaries, boundaries ? boundaries : "-", sizeof(s->boundaries) - 1);
    strncpy(s->array_dims, array_dims ? array_dims : "-", sizeof(s->array_dims) - 1);
    strncpy(s->params, params ? params : "-", sizeof(s->params) - 1);
    s->proc_def = proc_def;
    s->nesting_level = nesting;
    s->declared_line = declared_line;
    s->frequency = 1;
    s->ref_count = 0;
    if (s->ref_count < MAX_REFS_PER_SYM && declared_line != -1)
        s->ref_lines[s->ref_count++] = declared_line;
}

/* add reference line */
static void sym_add_reference(const char *name, int line) {
    int idx = sym_find_index(name);
    if (idx < 0) {
        sym_insert_or_update(name, "identifier", "variable", NULL, NULL, NULL, 0, current_nesting, line);
        return;
    }
    Symbol *s = &symtab[idx];
    s->frequency++;
    if(s->ref_count < MAX_REFS_PER_SYM)
        s->ref_lines[s->ref_count++] = line;
}

/* constant table helpers */
static int const_find_index(const char *lexeme, const char *type) {
    for(int i = 0; i < const_count; i++){
        if(strcmp(consttab[i].name, lexeme) == 0 && strcmp(consttab[i].type, type) == 0)
            return i;
    }
    return -1;
}

static void const_insert_or_bump(const char *lexeme, const char *type, int line) {
    int idx = const_find_index(lexeme, type);
    if (idx >= 0) {
        consttab[idx].count++;
        if(consttab[idx].ref_count < 128)
            consttab[idx].ref_lines[consttab[idx].ref_count++] = line;
        return;
    }
    if (const_count >= MAX_CONSTS) {
        fprintf(stderr,"Constant table full\n");
        return;
    }
    Constant *c = &consttab[const_count++];
    memset(c, 0, sizeof(*c));
    strncpy(c->name, lexeme, sizeof(c->name) - 1);
    strncpy(c->value, lexeme, sizeof(c->value) - 1);
    strncpy(c->type, type, sizeof(c->type) - 1);
    c->first_line = line;
    c->count = 1;
    c->ref_count = 0;
    if(c->ref_count < 128)
        c->ref_lines[c->ref_count++] = line;
}

/* print symbol table */
static void print_symbol_table(void) {
    printf("\n================= SYMBOL TABLE =================\n");
    printf("%-18s %-14s %-12s %-14s %-12s %-20s %-4s %-6s %-6s %-6s %-s\n",
           "Name","Token","Class","Boundaries","ArrayDims","Parameters",
           "PDef","Nest","DeclLn","Freq","RefLines");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0 ; i < sym_count ; i++) {
        Symbol *s = &symtab[i];
        printf("%-18s %-14s %-12s %-14s %-12s %-20s %-4d %-6d %-6d %-6d ",
               s->name, s->token, s->class, s->boundaries, s->array_dims,
               s->params, s->proc_def, s->nesting_level, s->declared_line, s->frequency);
        if (s->ref_count == 0) {
            printf("-\n");
        } else {
            for (int j = 0 ; j < s->ref_count ; j++) {
                if (j) printf(",");
                printf("%d", s->ref_lines[j]);
            }
            printf("\n");
        }
    }
}

/* print constant table */
static void print_const_table(void) {
    printf("\n================= CONSTANT TABLE (Literals Only) =================\n");
    printf("%-22s %-10s %-12s %-10s %-6s %s\n",
           "Variable Name", "FirstLine", "Value", "Type", "Count", "RefLines");
    printf("------------------------------------------------------------------------------\n");
    for (int i = 0 ; i < const_count ; i++) {
        Constant *c = &consttab[i];
        printf("%-22s %-10d %-12s %-10s %-6d ",
               c->name, c->first_line, c->value, c->type, c->count);
        if (c->ref_count == 0) {
            printf("-\n");
        } else {
            for (int j = 0 ; j < c->ref_count ; j++) {
                if (j) printf(",");
                printf("%d", c->ref_lines[j]);
            }
            printf("\n");
        }
    }
}


#line 281 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_NUMBER = 4,                     /* NUMBER  */
  YYSYMBOL_STRING_LITERAL = 5,             /* STRING_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 6,               /* CHAR_LITERAL  */
  YYSYMBOL_INCLUDE = 7,                    /* INCLUDE  */
  YYSYMBOL_PREPROCESSOR = 8,               /* PREPROCESSOR  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_FLOAT = 10,                     /* FLOAT  */
  YYSYMBOL_CHAR = 11,                      /* CHAR  */
  YYSYMBOL_VOID = 12,                      /* VOID  */
  YYSYMBOL_IF = 13,                        /* IF  */
  YYSYMBOL_ELSE = 14,                      /* ELSE  */
  YYSYMBOL_FOR = 15,                       /* FOR  */
  YYSYMBOL_WHILE = 16,                     /* WHILE  */
  YYSYMBOL_RETURN = 17,                    /* RETURN  */
  YYSYMBOL_PRINTF = 18,                    /* PRINTF  */
  YYSYMBOL_SWITCH = 19,                    /* SWITCH  */
  YYSYMBOL_CASE = 20,                      /* CASE  */
  YYSYMBOL_DEFAULT = 21,                   /* DEFAULT  */
  YYSYMBOL_BREAK = 22,                     /* BREAK  */
  YYSYMBOL_STRUCT = 23,                    /* STRUCT  */
  YYSYMBOL_UNION = 24,                     /* UNION  */
  YYSYMBOL_INCREMENT = 25,                 /* INCREMENT  */
  YYSYMBOL_DECREMENT = 26,                 /* DECREMENT  */
  YYSYMBOL_EQ = 27,                        /* EQ  */
  YYSYMBOL_NEQ = 28,                       /* NEQ  */
  YYSYMBOL_LT = 29,                        /* LT  */
  YYSYMBOL_GT = 30,                        /* GT  */
  YYSYMBOL_LE = 31,                        /* LE  */
  YYSYMBOL_GE = 32,                        /* GE  */
  YYSYMBOL_PLUS = 33,                      /* PLUS  */
  YYSYMBOL_MINUS = 34,                     /* MINUS  */
  YYSYMBOL_MULT = 35,                      /* MULT  */
  YYSYMBOL_DIVIDE = 36,                    /* DIVIDE  */
  YYSYMBOL_ASSIGN = 37,                    /* ASSIGN  */
  YYSYMBOL_OPAREN = 38,                    /* OPAREN  */
  YYSYMBOL_CPAREN = 39,                    /* CPAREN  */
  YYSYMBOL_OCURLY = 40,                    /* OCURLY  */
  YYSYMBOL_CCURLY = 41,                    /* CCURLY  */
  YYSYMBOL_SEMICOLON = 42,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 43,                     /* COMMA  */
  YYSYMBOL_COLON = 44,                     /* COLON  */
  YYSYMBOL_OBRACKET = 45,                  /* OBRACKET  */
  YYSYMBOL_CBRACKET = 46,                  /* CBRACKET  */
  YYSYMBOL_QUOTE = 47,                     /* QUOTE  */
  YYSYMBOL_UNMATCHEDSTRING = 48,           /* UNMATCHEDSTRING  */
  YYSYMBOL_UNMATCHEDCOMMENT = 49,          /* UNMATCHEDCOMMENT  */
  YYSYMBOL_IFX = 50,                       /* IFX  */
  YYSYMBOL_YYACCEPT = 51,                  /* $accept  */
  YYSYMBOL_p_oparen = 52,                  /* p_oparen  */
  YYSYMBOL_p_cparen = 53,                  /* p_cparen  */
  YYSYMBOL_p_ocurly = 54,                  /* p_ocurly  */
  YYSYMBOL_p_ccurly = 55,                  /* p_ccurly  */
  YYSYMBOL_p_semicolon = 56,               /* p_semicolon  */
  YYSYMBOL_p_comma = 57,                   /* p_comma  */
  YYSYMBOL_p_obracket = 58,                /* p_obracket  */
  YYSYMBOL_p_cbracket = 59,                /* p_cbracket  */
  YYSYMBOL_p_colon = 60,                   /* p_colon  */
  YYSYMBOL_op_assign = 61,                 /* op_assign  */
  YYSYMBOL_op_eq = 62,                     /* op_eq  */
  YYSYMBOL_op_neq = 63,                    /* op_neq  */
  YYSYMBOL_op_lt = 64,                     /* op_lt  */
  YYSYMBOL_op_gt = 65,                     /* op_gt  */
  YYSYMBOL_op_le = 66,                     /* op_le  */
  YYSYMBOL_op_ge = 67,                     /* op_ge  */
  YYSYMBOL_op_plus = 68,                   /* op_plus  */
  YYSYMBOL_op_minus = 69,                  /* op_minus  */
  YYSYMBOL_op_mult = 70,                   /* op_mult  */
  YYSYMBOL_op_divide = 71,                 /* op_divide  */
  YYSYMBOL_program = 72,                   /* program  */
  YYSYMBOL_preamble = 73,                  /* preamble  */
  YYSYMBOL_external_defs = 74,             /* external_defs  */
  YYSYMBOL_external_def = 75,              /* external_def  */
  YYSYMBOL_struct_definition = 76,         /* struct_definition  */
  YYSYMBOL_77_1 = 77,                      /* $@1  */
  YYSYMBOL_78_2 = 78,                      /* $@2  */
  YYSYMBOL_union_definition = 79,          /* union_definition  */
  YYSYMBOL_80_3 = 80,                      /* $@3  */
  YYSYMBOL_81_4 = 81,                      /* $@4  */
  YYSYMBOL_global_decl = 82,               /* global_decl  */
  YYSYMBOL_function_def = 83,              /* function_def  */
  YYSYMBOL_block = 84,                     /* block  */
  YYSYMBOL_85_5 = 85,                      /* $@5  */
  YYSYMBOL_decl_list = 86,                 /* decl_list  */
  YYSYMBOL_decl = 87,                      /* decl  */
  YYSYMBOL_declarator_list = 88,           /* declarator_list  */
  YYSYMBOL_declarator = 89,                /* declarator  */
  YYSYMBOL_type_specifier = 90,            /* type_specifier  */
  YYSYMBOL_declarator_var = 91,            /* declarator_var  */
  YYSYMBOL_declarator_array = 92,          /* declarator_array  */
  YYSYMBOL_declarator_func = 93,           /* declarator_func  */
  YYSYMBOL_params_list = 94,               /* params_list  */
  YYSYMBOL_param_decls = 95,               /* param_decls  */
  YYSYMBOL_param_decl = 96,                /* param_decl  */
  YYSYMBOL_stmt_list = 97,                 /* stmt_list  */
  YYSYMBOL_statement = 98,                 /* statement  */
  YYSYMBOL_if_stmt = 99,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 100,               /* while_stmt  */
  YYSYMBOL_for_stmt = 101,                 /* for_stmt  */
  YYSYMBOL_switch_stmt = 102,              /* switch_stmt  */
  YYSYMBOL_case_list = 103,                /* case_list  */
  YYSYMBOL_case_block = 104,               /* case_block  */
  YYSYMBOL_case_labels = 105,              /* case_labels  */
  YYSYMBOL_case_label = 106,               /* case_label  */
  YYSYMBOL_break_stmt = 107,               /* break_stmt  */
  YYSYMBOL_for_init_stmt = 108,            /* for_init_stmt  */
  YYSYMBOL_optional_expr = 109,            /* optional_expr  */
  YYSYMBOL_printf_stmt = 110,              /* printf_stmt  */
  YYSYMBOL_return_stmt = 111,              /* return_stmt  */
  YYSYMBOL_expr_stmt = 112,                /* expr_stmt  */
  YYSYMBOL_expr = 113,                     /* expr  */
  YYSYMBOL_cond_expr = 114,                /* cond_expr  */
  YYSYMBOL_add_expr = 115,                 /* add_expr  */
  YYSYMBOL_mul_expr = 116,                 /* mul_expr  */
  YYSYMBOL_postfix_expr = 117,             /* postfix_expr  */
  YYSYMBOL_primary_expr = 118              /* primary_expr  */
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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   253

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  129
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  214

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   305


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
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   242,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   267,   270,   272,   280,   289,   290,   294,   295,
     296,   297,   301,   301,   301,   312,   312,   312,   322,   323,
     327,   338,   338,   339,   342,   344,   348,   349,   353,   354,
     358,   369,   380,   393,   394,   395,   396,   400,   404,   417,
     421,   431,   432,   437,   442,   443,   457,   468,   469,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   488,   493,   502,   510,   518,   525,   527,   531,   535,
     536,   540,   544,   551,   559,   560,   561,   564,   565,   569,
     576,   584,   588,   595,   602,   606,   607,   611,   617,   621,
     622,   623,   624,   625,   626,   627,   631,   632,   633,   637,
     638,   639,   643,   644,   651,   661,   663,   665,   667,   669
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
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER", "NUMBER",
  "STRING_LITERAL", "CHAR_LITERAL", "INCLUDE", "PREPROCESSOR", "INT",
  "FLOAT", "CHAR", "VOID", "IF", "ELSE", "FOR", "WHILE", "RETURN",
  "PRINTF", "SWITCH", "CASE", "DEFAULT", "BREAK", "STRUCT", "UNION",
  "INCREMENT", "DECREMENT", "EQ", "NEQ", "LT", "GT", "LE", "GE", "PLUS",
  "MINUS", "MULT", "DIVIDE", "ASSIGN", "OPAREN", "CPAREN", "OCURLY",
  "CCURLY", "SEMICOLON", "COMMA", "COLON", "OBRACKET", "CBRACKET", "QUOTE",
  "UNMATCHEDSTRING", "UNMATCHEDCOMMENT", "IFX", "$accept", "p_oparen",
  "p_cparen", "p_ocurly", "p_ccurly", "p_semicolon", "p_comma",
  "p_obracket", "p_cbracket", "p_colon", "op_assign", "op_eq", "op_neq",
  "op_lt", "op_gt", "op_le", "op_ge", "op_plus", "op_minus", "op_mult",
  "op_divide", "program", "preamble", "external_defs", "external_def",
  "struct_definition", "$@1", "$@2", "union_definition", "$@3", "$@4",
  "global_decl", "function_def", "block", "$@5", "decl_list", "decl",
  "declarator_list", "declarator", "type_specifier", "declarator_var",
  "declarator_array", "declarator_func", "params_list", "param_decls",
  "param_decl", "stmt_list", "statement", "if_stmt", "while_stmt",
  "for_stmt", "switch_stmt", "case_list", "case_block", "case_labels",
  "case_label", "break_stmt", "for_init_stmt", "optional_expr",
  "printf_stmt", "return_stmt", "expr_stmt", "expr", "cond_expr",
  "add_expr", "mul_expr", "postfix_expr", "primary_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-159)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-89)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -159,     9,    21,  -159,  -159,  -159,   229,  -159,  -159,  -159,
    -159,    19,    36,  -159,  -159,  -159,  -159,  -159,   132,    27,
      27,    37,   -24,   103,  -159,    50,  -159,    11,  -159,  -159,
    -159,  -159,  -159,  -159,  -159,    99,    85,  -159,  -159,    91,
    -159,    12,    60,  -159,  -159,  -159,  -159,    76,   123,   101,
     108,  -159,    84,    84,   120,  -159,     1,  -159,  -159,  -159,
      12,  -159,   163,   126,   128,  -159,  -159,  -159,  -159,  -159,
      59,    59,  -159,  -159,  -159,   145,  -159,  -159,  -159,  -159,
    -159,    12,   101,  -159,  -159,  -159,  -159,  -159,  -159,    28,
      28,    28,    28,    28,    28,  -159,  -159,    28,    28,  -159,
    -159,    28,    28,   145,  -159,  -159,   140,  -159,  -159,  -159,
    -159,   141,   126,   126,   126,   126,   126,   126,   128,   128,
    -159,  -159,   167,    37,    37,   103,    37,   146,   137,   137,
     137,   116,   137,   137,    37,  -159,  -159,  -159,  -159,  -159,
    -159,  -159,  -159,  -159,  -159,  -159,  -159,  -159,    37,  -159,
    -159,  -159,  -159,  -159,    12,    87,    12,    37,    37,    37,
      75,    12,  -159,  -159,   101,    37,  -159,  -159,    12,  -159,
     101,  -159,  -159,  -159,   101,    39,   101,   209,  -159,    37,
    -159,   209,    37,    37,   178,    27,   190,    12,  -159,  -159,
    -159,   101,  -159,   209,   101,    37,     4,  -159,   209,  -159,
      69,   162,  -159,  -159,   182,  -159,  -159,  -159,   162,  -159,
    -159,    43,  -159,  -159
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      23,     0,    26,     1,    24,    25,    22,    53,    54,    55,
      56,     0,     0,    27,    30,    31,    29,    28,     0,     0,
       0,     0,    57,     0,    48,    50,    52,     0,     4,    32,
      35,     6,    39,     2,     8,    61,     0,     7,    38,     0,
      11,     0,     0,    41,    40,    44,    44,    56,     0,     0,
      63,    64,     0,     0,    57,    49,   125,   126,   127,   128,
       0,    51,   108,   109,   116,   119,   122,     5,    43,    44,
       0,     0,    66,     3,    60,     0,     9,    59,    58,   123,
     124,     0,     0,    12,    13,    14,    15,    16,    17,     0,
       0,     0,     0,     0,     0,    18,    19,     0,     0,    20,
      21,     0,     0,    67,    33,    45,     0,    36,    65,   107,
     129,   125,   114,   115,   110,   111,   112,   113,   117,   118,
     120,   121,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    42,    79,    78,    72,    68,
      73,    74,    75,    76,    77,    70,    71,    69,     0,    34,
      47,    46,    37,    80,     0,     0,     0,     0,     0,     0,
       0,     0,    93,   105,     0,     0,    96,    95,    97,    94,
       0,   104,   103,   102,     0,     0,     0,     0,   106,     0,
      98,     0,     0,     0,     0,     0,    81,    97,    83,   101,
      99,     0,    86,     0,     0,     0,     0,    82,     0,   100,
       0,     0,    85,    87,    67,    89,    84,   125,     0,    10,
      92,     0,    90,    91
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -159,    20,   -52,   -16,   -65,   -21,   -43,  -159,   158,    15,
     173,  -159,  -159,  -159,  -159,  -159,  -159,  -159,  -159,  -159,
    -159,  -159,  -159,  -159,  -159,  -159,  -159,  -159,  -159,  -159,
    -159,  -159,  -159,   203,  -159,   -33,   -60,   129,   195,     2,
    -159,  -159,  -159,  -159,  -159,   161,    33,  -158,  -159,  -159,
    -159,  -159,  -159,  -159,  -159,    38,  -159,  -159,    56,  -159,
    -159,    89,   -40,  -159,   107,   119,   131,    45
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    60,    74,    43,    68,   136,    39,    36,    77,   210,
      41,    89,    90,    91,    92,    93,    94,    97,    98,   101,
     102,     1,     2,     6,    13,    14,    45,   123,    15,    46,
     126,    16,    17,   137,    69,    70,   138,    23,    24,   106,
      25,    26,    27,    49,    50,    51,   122,   139,   140,   141,
     142,   143,   196,   203,   204,   205,   144,   168,   179,   145,
     146,   147,   148,    62,    63,    64,    65,    66
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,    61,    38,    29,    30,   104,   107,    75,    18,     3,
     105,   105,    42,    71,    33,    56,    57,    58,    59,   186,
      82,    34,    19,   188,   200,   201,    79,    80,     4,     5,
     110,   111,    57,    58,    59,   197,   103,    48,    40,    20,
     206,   109,    35,   105,   127,    67,    56,    57,    58,    59,
      33,    28,     7,     8,     9,    10,   128,   135,   129,   130,
     131,   132,   133,   -88,   -88,   134,    33,    28,     7,     8,
       9,    10,   207,    57,    58,    59,   174,    48,    73,    31,
     175,    33,    37,    28,   -88,    31,    52,    40,   165,    53,
      56,    57,    58,    59,    54,   167,     7,     8,     9,    10,
      67,    67,   149,   150,   151,   152,   153,    33,     7,     8,
       9,    47,   177,   162,   164,   -62,   170,   157,   181,   158,
     159,   176,   182,   183,   185,    33,    72,   163,   180,    31,
      76,   202,   184,    21,   166,    22,   171,   172,   173,   195,
      73,   124,   198,    54,   178,    31,    37,   180,   154,   155,
     156,    37,   160,   161,     7,     8,     9,    10,   187,    95,
      96,   189,   190,    99,   100,    34,    79,    80,   127,   192,
      56,    57,    58,    59,   199,    33,     7,     8,     9,    10,
     128,   191,   129,   130,   131,   132,   133,    67,    31,   134,
      83,    84,    85,    86,    87,    88,   112,   113,   114,   115,
     116,   117,   200,   201,   193,    33,   209,    28,    67,    31,
     127,    78,    56,    57,    58,    59,   118,   119,     7,     8,
       9,    10,   128,   213,   129,   130,   131,   132,   133,    81,
      44,   134,   120,   121,    55,   125,   108,   211,     7,     8,
       9,    10,   212,   194,   169,   208,     0,    33,     0,    28,
       0,    31,    11,    12
};

static const yytype_int16 yycheck[] =
{
      21,    41,    23,    19,    20,    70,    71,    50,     6,     0,
      70,    71,     1,    46,    38,     3,     4,     5,     6,   177,
      60,    45,     3,   181,    20,    21,    25,    26,     7,     8,
      82,     3,     4,     5,     6,   193,    69,    35,    37,     3,
     198,    81,    22,   103,     1,    41,     3,     4,     5,     6,
      38,    40,     9,    10,    11,    12,    13,   122,    15,    16,
      17,    18,    19,    20,    21,    22,    38,    40,     9,    10,
      11,    12,     3,     4,     5,     6,     1,    75,    39,    42,
       5,    38,    43,    40,    41,    42,     1,    37,     1,     4,
       3,     4,     5,     6,     3,   155,     9,    10,    11,    12,
      41,    41,   123,   124,   125,   126,   127,    38,     9,    10,
      11,    12,   164,   134,   154,    39,   156,     1,   170,     3,
       4,   161,   174,   175,   176,    38,     3,   148,   168,    42,
      46,   196,   175,     1,   155,     3,   157,   158,   159,   191,
      39,     1,   194,     3,   165,    42,    43,   187,   128,   129,
     130,    43,   132,   133,     9,    10,    11,    12,   179,    33,
      34,   182,   183,    35,    36,    45,    25,    26,     1,   185,
       3,     4,     5,     6,   195,    38,     9,    10,    11,    12,
      13,     3,    15,    16,    17,    18,    19,    41,    42,    22,
      27,    28,    29,    30,    31,    32,    89,    90,    91,    92,
      93,    94,    20,    21,    14,    38,    44,    40,    41,    42,
       1,    53,     3,     4,     5,     6,    97,    98,     9,    10,
      11,    12,    13,   208,    15,    16,    17,    18,    19,    56,
      27,    22,   101,   102,    39,   106,    75,   204,     9,    10,
      11,    12,   204,   187,   155,   200,    -1,    38,    -1,    40,
      -1,    42,    23,    24
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    72,    73,     0,     7,     8,    74,     9,    10,    11,
      12,    23,    24,    75,    76,    79,    82,    83,    90,     3,
       3,     1,     3,    88,    89,    91,    92,    93,    40,    54,
      54,    42,    56,    38,    45,    52,    58,    43,    56,    57,
      37,    61,     1,    54,    84,    77,    80,    12,    90,    94,
      95,    96,     1,     4,     3,    89,     3,     4,     5,     6,
      52,   113,   114,   115,   116,   117,   118,    41,    55,    85,
      86,    86,     3,    39,    53,    57,    46,    59,    59,    25,
      26,    61,   113,    27,    28,    29,    30,    31,    32,    62,
      63,    64,    65,    66,    67,    33,    34,    68,    69,    35,
      36,    70,    71,    86,    55,    87,    90,    55,    96,   113,
      53,     3,   115,   115,   115,   115,   115,   115,   116,   116,
     117,   117,    97,    78,     1,    88,    81,     1,    13,    15,
      16,    17,    18,    19,    22,    55,    56,    84,    87,    98,
      99,   100,   101,   102,   107,   110,   111,   112,   113,    56,
      56,    56,    56,    56,    52,    52,    52,     1,     3,     4,
      52,    52,    56,    56,   113,     1,    56,    87,   108,   112,
     113,    56,    56,    56,     1,     5,   113,    53,    56,   109,
     113,    53,    53,    53,    57,    53,    98,    56,    98,    56,
      56,     3,    54,    14,   109,    53,   103,    98,    53,    56,
      20,    21,    55,   104,   105,   106,    98,     3,   118,    44,
      60,    97,   106,    60
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    73,    73,    74,    74,    75,    75,
      75,    75,    77,    78,    76,    80,    81,    79,    82,    82,
      83,    85,    84,    84,    86,    86,    87,    87,    88,    88,
      89,    89,    89,    90,    90,    90,    90,    91,    92,    92,
      93,    94,    94,    94,    95,    95,    96,    97,    97,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    99,    99,   100,   101,   102,   103,   103,   104,   105,
     105,   106,   106,   107,   108,   108,   108,   109,   109,   110,
     110,   110,   111,   111,   111,   112,   112,   113,   113,   114,
     114,   114,   114,   114,   114,   114,   115,   115,   115,   116,
     116,   116,   117,   117,   117,   118,   118,   118,   118,   118
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     0,     2,     2,     0,     2,     1,     1,
       1,     1,     0,     0,     8,     0,     0,     8,     3,     3,
       3,     0,     5,     2,     0,     2,     3,     3,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     0,     1,     1,     1,     3,     2,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     5,     7,     5,     8,     7,     0,     2,     2,     1,
       2,     3,     2,     2,     1,     1,     1,     0,     1,     5,
       7,     5,     3,     3,     3,     2,     2,     3,     1,     1,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     2,     2,     1,     1,     1,     1,     3
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
  case 2: /* p_oparen: OPAREN  */
#line 242 "parser.y"
                       { sym_insert_or_update("(", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1557 "parser.tab.c"
    break;

  case 3: /* p_cparen: CPAREN  */
#line 243 "parser.y"
                       { sym_insert_or_update(")", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1563 "parser.tab.c"
    break;

  case 4: /* p_ocurly: OCURLY  */
#line 244 "parser.y"
                       { sym_insert_or_update("{", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1569 "parser.tab.c"
    break;

  case 5: /* p_ccurly: CCURLY  */
#line 245 "parser.y"
                       { sym_insert_or_update("}", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1575 "parser.tab.c"
    break;

  case 6: /* p_semicolon: SEMICOLON  */
#line 246 "parser.y"
                       { sym_insert_or_update(";", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1581 "parser.tab.c"
    break;

  case 7: /* p_comma: COMMA  */
#line 247 "parser.y"
                       { sym_insert_or_update(",", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1587 "parser.tab.c"
    break;

  case 8: /* p_obracket: OBRACKET  */
#line 248 "parser.y"
                       { sym_insert_or_update("[", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1593 "parser.tab.c"
    break;

  case 9: /* p_cbracket: CBRACKET  */
#line 249 "parser.y"
                       { sym_insert_or_update("]", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1599 "parser.tab.c"
    break;

  case 10: /* p_colon: COLON  */
#line 250 "parser.y"
                       { sym_insert_or_update(":", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1605 "parser.tab.c"
    break;

  case 11: /* op_assign: ASSIGN  */
#line 252 "parser.y"
                       { sym_insert_or_update("=", "operator", "assignment", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1611 "parser.tab.c"
    break;

  case 12: /* op_eq: EQ  */
#line 253 "parser.y"
                       { sym_insert_or_update("==", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1617 "parser.tab.c"
    break;

  case 13: /* op_neq: NEQ  */
#line 254 "parser.y"
                       { sym_insert_or_update("!=", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1623 "parser.tab.c"
    break;

  case 14: /* op_lt: LT  */
#line 255 "parser.y"
                       { sym_insert_or_update("<", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1629 "parser.tab.c"
    break;

  case 15: /* op_gt: GT  */
#line 256 "parser.y"
                       { sym_insert_or_update(">", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1635 "parser.tab.c"
    break;

  case 16: /* op_le: LE  */
#line 257 "parser.y"
                       { sym_insert_or_update("<=", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1641 "parser.tab.c"
    break;

  case 17: /* op_ge: GE  */
#line 258 "parser.y"
                       { sym_insert_or_update(">=", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1647 "parser.tab.c"
    break;

  case 18: /* op_plus: PLUS  */
#line 259 "parser.y"
                       { sym_insert_or_update("+", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1653 "parser.tab.c"
    break;

  case 19: /* op_minus: MINUS  */
#line 260 "parser.y"
                       { sym_insert_or_update("-", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1659 "parser.tab.c"
    break;

  case 20: /* op_mult: MULT  */
#line 261 "parser.y"
                       { sym_insert_or_update("*", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1665 "parser.tab.c"
    break;

  case 21: /* op_divide: DIVIDE  */
#line 262 "parser.y"
                       { sym_insert_or_update("/", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1671 "parser.tab.c"
    break;

  case 24: /* preamble: preamble INCLUDE  */
#line 273 "parser.y"
        {
            if ((yyvsp[0].str)) {
                sym_insert_or_update("#include", "preprocessor", "directive", NULL, NULL, NULL, 0, 0, yylineno);
                sym_insert_or_update((yyvsp[0].str), "header", "file", NULL, NULL, NULL, 0, 0, yylineno);
                free((yyvsp[0].str));
            }
        }
#line 1683 "parser.tab.c"
    break;

  case 25: /* preamble: preamble PREPROCESSOR  */
#line 281 "parser.y"
        {
             if ((yyvsp[0].str)) {
                sym_insert_or_update((yyvsp[0].str), "preprocessor", "directive", NULL, NULL, NULL, 0, 0, yylineno);
                free((yyvsp[0].str));
            }
        }
#line 1694 "parser.tab.c"
    break;

  case 32: /* $@1: %empty  */
#line 301 "parser.y"
                               { g_in_struct_def = 1; current_nesting++; }
#line 1700 "parser.tab.c"
    break;

  case 33: /* $@2: %empty  */
#line 301 "parser.y"
                                                                                              { g_in_struct_def = 0; current_nesting--; }
#line 1706 "parser.tab.c"
    break;

  case 34: /* struct_definition: STRUCT IDENTIFIER p_ocurly $@1 decl_list p_ccurly $@2 p_semicolon  */
#line 302 "parser.y"
        {
            sym_insert_or_update("struct", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);
            sym_insert_or_update((yyvsp[-6].str), "struct_tag", "type", NULL, NULL, NULL, 0, current_nesting, yylineno);
            printf("Line %d: Parsed a struct definition for 'struct %s'.\n", yylineno, (yyvsp[-6].str));
            free((yyvsp[-6].str));
        }
#line 1717 "parser.tab.c"
    break;

  case 35: /* $@3: %empty  */
#line 312 "parser.y"
                              { g_in_union_def = 1; current_nesting++; }
#line 1723 "parser.tab.c"
    break;

  case 36: /* $@4: %empty  */
#line 312 "parser.y"
                                                                                            { g_in_union_def = 0; current_nesting--; }
#line 1729 "parser.tab.c"
    break;

  case 37: /* union_definition: UNION IDENTIFIER p_ocurly $@3 decl_list p_ccurly $@4 p_semicolon  */
#line 313 "parser.y"
        {
            sym_insert_or_update("union", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);
            sym_insert_or_update((yyvsp[-6].str), "union_tag", "type", NULL, NULL, NULL, 0, current_nesting, yylineno);
            printf("Line %d: Parsed a union definition for 'union %s'.\n", yylineno, (yyvsp[-6].str));
            free((yyvsp[-6].str));
        }
#line 1740 "parser.tab.c"
    break;

  case 38: /* global_decl: type_specifier declarator_list p_semicolon  */
#line 322 "parser.y"
                                                 { free((yyvsp[-2].str)); }
#line 1746 "parser.tab.c"
    break;

  case 39: /* global_decl: type_specifier error p_semicolon  */
#line 323 "parser.y"
                                       { yyerrok; free((yyvsp[-2].str)); }
#line 1752 "parser.tab.c"
    break;

  case 40: /* function_def: type_specifier declarator_func block  */
#line 328 "parser.y"
    {
        printf("Line %d: Parsed a function definition for '%s'.\n", yylineno, (yyvsp[-1].str));
        if((yyvsp[-1].str)) { 
            sym_insert_or_update((yyvsp[-1].str), (yyvsp[-2].str), "function", NULL, NULL, yylval.str, 1, current_nesting, yylineno);
            free((yyvsp[-1].str));
        }
        free((yyvsp[-2].str));
    }
#line 1765 "parser.tab.c"
    break;

  case 41: /* $@5: %empty  */
#line 338 "parser.y"
             { current_nesting++; }
#line 1771 "parser.tab.c"
    break;

  case 42: /* block: p_ocurly $@5 decl_list stmt_list p_ccurly  */
#line 338 "parser.y"
                                                                 { current_nesting--; }
#line 1777 "parser.tab.c"
    break;

  case 43: /* block: error p_ccurly  */
#line 339 "parser.y"
                   { yyerrok; }
#line 1783 "parser.tab.c"
    break;

  case 46: /* decl: type_specifier declarator_list p_semicolon  */
#line 348 "parser.y"
                                                 { free((yyvsp[-2].str)); }
#line 1789 "parser.tab.c"
    break;

  case 47: /* decl: type_specifier error p_semicolon  */
#line 349 "parser.y"
                                       { yyerrok; free((yyvsp[-2].str)); }
#line 1795 "parser.tab.c"
    break;

  case 50: /* declarator: declarator_var  */
#line 359 "parser.y"
        {
            const char* class;
            if (g_in_struct_def) class = "struct_member";
            else if (g_in_union_def) class = "union_member"; /* NEW */
            else class = "variable";

            printf("Line %d: Parsed a declaration for '%s'.\n", yylineno, (yyvsp[0].str));
            sym_insert_or_update((yyvsp[0].str), "identifier", class, NULL, NULL, NULL, 0, current_nesting, yylineno);
            free((yyvsp[0].str));
        }
#line 1810 "parser.tab.c"
    break;

  case 51: /* declarator: declarator_var op_assign expr  */
#line 370 "parser.y"
        {
            const char* class = g_in_struct_def ? "struct_member" : "variable";
             if (g_in_struct_def) class = "struct_member";
            else if (g_in_union_def) class = "union_member"; /* NEW */
            else class = "variable";

            printf("Line %d: Parsed a declaration with initialization for '%s'.\n", yylineno, (yyvsp[-2].str));
            sym_insert_or_update((yyvsp[-2].str), "identifier", class, NULL, NULL, NULL, 0, current_nesting, yylineno);
            free((yyvsp[-2].str));
        }
#line 1825 "parser.tab.c"
    break;

  case 52: /* declarator: declarator_array  */
#line 381 "parser.y"
        {
            printf("Line %d: Parsed an array declarator for '%s'.\n", yylineno, (yyvsp[0].str));
            if (g_in_struct_def) { 
                sym_insert_or_update((yyvsp[0].str), "array", "struct_member", NULL, NULL, NULL, 0, 0, -1);
            } else if (g_in_union_def) { /* NEW */
                sym_insert_or_update((yyvsp[0].str), "array", "union_member", NULL, NULL, NULL, 0, 0, -1);
            }
            free((yyvsp[0].str));
        }
#line 1839 "parser.tab.c"
    break;

  case 53: /* type_specifier: INT  */
#line 393 "parser.y"
            { sym_insert_or_update("int", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);   (yyval.str) = xstrdup("int"); }
#line 1845 "parser.tab.c"
    break;

  case 54: /* type_specifier: FLOAT  */
#line 394 "parser.y"
            { sym_insert_or_update("float", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno); (yyval.str) = xstrdup("float"); }
#line 1851 "parser.tab.c"
    break;

  case 55: /* type_specifier: CHAR  */
#line 395 "parser.y"
            { sym_insert_or_update("char", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);  (yyval.str) = xstrdup("char"); }
#line 1857 "parser.tab.c"
    break;

  case 56: /* type_specifier: VOID  */
#line 396 "parser.y"
            { sym_insert_or_update("void", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);  (yyval.str) = xstrdup("void"); }
#line 1863 "parser.tab.c"
    break;

  case 57: /* declarator_var: IDENTIFIER  */
#line 400 "parser.y"
                 { (yyval.str) = (yyvsp[0].str); }
#line 1869 "parser.tab.c"
    break;

  case 58: /* declarator_array: IDENTIFIER p_obracket NUMBER p_cbracket  */
#line 405 "parser.y"
        {
            char dims[128]; snprintf(dims,sizeof(dims),"[%s]", (yyvsp[-1].str));
            const char* class;
            if (g_in_struct_def) class = "struct_member";
            else if (g_in_union_def) class = "union_member"; /* NEW */
            else class = "variable";

            sym_insert_or_update((yyvsp[-3].str), "array", class, NULL, dims, NULL, 0, current_nesting, yylineno);
            const_insert_or_bump((yyvsp[-1].str), "int", yylineno);
            free((yyvsp[-1].str));
            (yyval.str) = (yyvsp[-3].str);
        }
#line 1886 "parser.tab.c"
    break;

  case 59: /* declarator_array: IDENTIFIER p_obracket error p_cbracket  */
#line 417 "parser.y"
                                             { yyerrok; (yyval.str) = (yyvsp[-3].str); }
#line 1892 "parser.tab.c"
    break;

  case 60: /* declarator_func: IDENTIFIER p_oparen params_list p_cparen  */
#line 422 "parser.y"
        {
            sym_insert_or_update((yyvsp[-3].str), "function", "function", NULL, NULL, (yyvsp[-1].str), 0, current_nesting, yylineno);
            (yyval.str) = (yyvsp[-3].str);         
            yylval.str = (yyvsp[-1].str); 
        }
#line 1902 "parser.tab.c"
    break;

  case 61: /* params_list: %empty  */
#line 431 "parser.y"
      { (yyval.str) = xstrdup(""); }
#line 1908 "parser.tab.c"
    break;

  case 62: /* params_list: VOID  */
#line 433 "parser.y"
      {
          sym_insert_or_update("void", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);
          (yyval.str) = xstrdup("void");
      }
#line 1917 "parser.tab.c"
    break;

  case 63: /* params_list: param_decls  */
#line 438 "parser.y"
      { (yyval.str) = (yyvsp[0].str); }
#line 1923 "parser.tab.c"
    break;

  case 65: /* param_decls: param_decls p_comma param_decl  */
#line 444 "parser.y"
      {
            char *a = (yyvsp[-2].str); char *b = (yyvsp[0].str);
            if(!a) { (yyval.str) = b; } else {
                size_t n = strlen(a) + strlen(b) + 4;
                char *out = malloc(n);
                snprintf(out, n, "%s, %s", a, b);
                free(a); free(b);
                (yyval.str) = out;
            }
      }
#line 1938 "parser.tab.c"
    break;

  case 66: /* param_decl: type_specifier IDENTIFIER  */
#line 458 "parser.y"
        {
            sym_insert_or_update((yyvsp[0].str), "identifier", "parameter", NULL, NULL, NULL, 0, current_nesting+1, yylineno); 
            size_t n = strlen((yyvsp[-1].str)) + strlen((yyvsp[0].str)) + 2;
            char *s = malloc(n);
            snprintf(s, n, "%s %s", (yyvsp[-1].str), (yyvsp[0].str));
            free((yyvsp[-1].str)); free((yyvsp[0].str));
            (yyval.str) = s;
        }
#line 1951 "parser.tab.c"
    break;

  case 80: /* statement: error p_semicolon  */
#line 484 "parser.y"
                        { yyerrok; }
#line 1957 "parser.tab.c"
    break;

  case 81: /* if_stmt: IF p_oparen expr p_cparen statement  */
#line 489 "parser.y"
        { 
            sym_insert_or_update("if", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed an if statement.\n", yylineno);
        }
#line 1966 "parser.tab.c"
    break;

  case 82: /* if_stmt: IF p_oparen expr p_cparen statement ELSE statement  */
#line 494 "parser.y"
        { 
            sym_insert_or_update("if", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            sym_insert_or_update("else", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed an if-else statement.\n", yylineno);
        }
#line 1976 "parser.tab.c"
    break;

  case 83: /* while_stmt: WHILE p_oparen expr p_cparen statement  */
#line 503 "parser.y"
        { 
            sym_insert_or_update("while", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a while loop.\n", yylineno);
        }
#line 1985 "parser.tab.c"
    break;

  case 84: /* for_stmt: FOR p_oparen for_init_stmt optional_expr p_semicolon optional_expr p_cparen statement  */
#line 511 "parser.y"
        { 
            sym_insert_or_update("for", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a for loop.\n", yylineno);
        }
#line 1994 "parser.tab.c"
    break;

  case 85: /* switch_stmt: SWITCH p_oparen expr p_cparen p_ocurly case_list p_ccurly  */
#line 519 "parser.y"
        {
            sym_insert_or_update("switch", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a switch statement.\n", yylineno);
        }
#line 2003 "parser.tab.c"
    break;

  case 91: /* case_label: CASE primary_expr p_colon  */
#line 541 "parser.y"
        {
            sym_insert_or_update("case", "keyword", "label", NULL, NULL, NULL, 0, 0, yylineno);
        }
#line 2011 "parser.tab.c"
    break;

  case 92: /* case_label: DEFAULT p_colon  */
#line 545 "parser.y"
        {
            sym_insert_or_update("default", "keyword", "label", NULL, NULL, NULL, 0, 0, yylineno);
        }
#line 2019 "parser.tab.c"
    break;

  case 93: /* break_stmt: BREAK p_semicolon  */
#line 552 "parser.y"
        {
            sym_insert_or_update("break", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a break statement.\n", yylineno);
        }
#line 2028 "parser.tab.c"
    break;

  case 99: /* printf_stmt: PRINTF p_oparen STRING_LITERAL p_cparen p_semicolon  */
#line 570 "parser.y"
        { 
            sym_insert_or_update("printf", "keyword", "function", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a printf statement.\n", yylineno);
            const_insert_or_bump((yyvsp[-2].str), "string", yylineno);
            free((yyvsp[-2].str));
        }
#line 2039 "parser.tab.c"
    break;

  case 100: /* printf_stmt: PRINTF p_oparen STRING_LITERAL p_comma IDENTIFIER p_cparen p_semicolon  */
#line 577 "parser.y"
        { 
            sym_insert_or_update("printf", "keyword", "function", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a printf statement with variable '%s'.\n", yylineno, (yyvsp[-2].str));
            const_insert_or_bump((yyvsp[-4].str), "string", yylineno);
            sym_add_reference((yyvsp[-2].str), yylineno);
            free((yyvsp[-4].str)); free((yyvsp[-2].str));
        }
#line 2051 "parser.tab.c"
    break;

  case 101: /* printf_stmt: PRINTF p_oparen error p_cparen p_semicolon  */
#line 584 "parser.y"
                                                 { yyerrok; }
#line 2057 "parser.tab.c"
    break;

  case 102: /* return_stmt: RETURN NUMBER p_semicolon  */
#line 589 "parser.y"
        { 
            sym_insert_or_update("return", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a return statement with value %s.\n", yylineno, (yyvsp[-1].str));
            const_insert_or_bump((yyvsp[-1].str), "int", yylineno);
            free((yyvsp[-1].str));
        }
#line 2068 "parser.tab.c"
    break;

  case 103: /* return_stmt: RETURN IDENTIFIER p_semicolon  */
#line 596 "parser.y"
        { 
            sym_insert_or_update("return", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a return statement with variable '%s'.\n", yylineno, (yyvsp[-1].str));
            sym_add_reference((yyvsp[-1].str), yylineno);
            free((yyvsp[-1].str));
        }
#line 2079 "parser.tab.c"
    break;

  case 104: /* return_stmt: RETURN error p_semicolon  */
#line 602 "parser.y"
                               { yyerrok; }
#line 2085 "parser.tab.c"
    break;

  case 106: /* expr_stmt: error p_semicolon  */
#line 607 "parser.y"
                        { yyerrok; }
#line 2091 "parser.tab.c"
    break;

  case 107: /* expr: IDENTIFIER op_assign expr  */
#line 612 "parser.y"
        { 
            printf("Line %d: Parsed an assignment to variable '%s'.\n", yylineno, (yyvsp[-2].str));
            sym_add_reference((yyvsp[-2].str), yylineno);
            free((yyvsp[-2].str));
        }
#line 2101 "parser.tab.c"
    break;

  case 123: /* postfix_expr: IDENTIFIER INCREMENT  */
#line 645 "parser.y"
        {
            sym_add_reference((yyvsp[-1].str), yylineno);
            sym_insert_or_update("++", "operator", "postfix", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a post-increment operation on '%s'.\n", yylineno, (yyvsp[-1].str));
            free((yyvsp[-1].str));
        }
#line 2112 "parser.tab.c"
    break;

  case 124: /* postfix_expr: IDENTIFIER DECREMENT  */
#line 652 "parser.y"
        {
            sym_add_reference((yyvsp[-1].str), yylineno);
            sym_insert_or_update("--", "operator", "postfix", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a post-decrement operation on '%s'.\n", yylineno, (yyvsp[-1].str));
            free((yyvsp[-1].str));
        }
#line 2123 "parser.tab.c"
    break;

  case 125: /* primary_expr: IDENTIFIER  */
#line 662 "parser.y"
        { sym_add_reference((yyvsp[0].str), yylineno); free((yyvsp[0].str)); }
#line 2129 "parser.tab.c"
    break;

  case 126: /* primary_expr: NUMBER  */
#line 664 "parser.y"
        { const_insert_or_bump((yyvsp[0].str), "int", yylineno); free((yyvsp[0].str)); }
#line 2135 "parser.tab.c"
    break;

  case 127: /* primary_expr: STRING_LITERAL  */
#line 666 "parser.y"
        { const_insert_or_bump((yyvsp[0].str), "string", yylineno); free((yyvsp[0].str)); }
#line 2141 "parser.tab.c"
    break;

  case 128: /* primary_expr: CHAR_LITERAL  */
#line 668 "parser.y"
        { const_insert_or_bump((yyvsp[0].str), "char", yylineno); free((yyvsp[0].str)); }
#line 2147 "parser.tab.c"
    break;


#line 2151 "parser.tab.c"

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

#line 672 "parser.y"


/* ---------------- yyerror & main ---------------- */
void yyerror(const char *msg) {
    extern char *yytext;
    if (yytext && *yytext)
        fprintf(stderr, "Syntax error at line %d near '%s': %s\n",
                yylineno, yytext, msg ? msg : "invalid syntax");
    else
        fprintf(stderr, "Syntax error at line %d: %s\n",
                yylineno, msg ? msg : "invalid syntax");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <source.c>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    extern FILE *yyin;
    yyin = file;

    if (yyparse() == 0) {
        printf("\nParsing successfully completed\n");
        print_symbol_table();
        print_const_table();
    }
    else {
        fprintf(stderr, "Parsing failed!\n");
    }

    fclose(file);
    return 0;
}
