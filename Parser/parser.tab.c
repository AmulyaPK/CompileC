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


#line 277 "parser.tab.c"

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
  YYSYMBOL_EQ = 23,                        /* EQ  */
  YYSYMBOL_NEQ = 24,                       /* NEQ  */
  YYSYMBOL_LT = 25,                        /* LT  */
  YYSYMBOL_GT = 26,                        /* GT  */
  YYSYMBOL_LE = 27,                        /* LE  */
  YYSYMBOL_GE = 28,                        /* GE  */
  YYSYMBOL_PLUS = 29,                      /* PLUS  */
  YYSYMBOL_MINUS = 30,                     /* MINUS  */
  YYSYMBOL_MULT = 31,                      /* MULT  */
  YYSYMBOL_DIVIDE = 32,                    /* DIVIDE  */
  YYSYMBOL_ASSIGN = 33,                    /* ASSIGN  */
  YYSYMBOL_OPAREN = 34,                    /* OPAREN  */
  YYSYMBOL_CPAREN = 35,                    /* CPAREN  */
  YYSYMBOL_OCURLY = 36,                    /* OCURLY  */
  YYSYMBOL_CCURLY = 37,                    /* CCURLY  */
  YYSYMBOL_SEMICOLON = 38,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 39,                     /* COMMA  */
  YYSYMBOL_COLON = 40,                     /* COLON  */
  YYSYMBOL_OBRACKET = 41,                  /* OBRACKET  */
  YYSYMBOL_CBRACKET = 42,                  /* CBRACKET  */
  YYSYMBOL_QUOTE = 43,                     /* QUOTE  */
  YYSYMBOL_UNMATCHEDSTRING = 44,           /* UNMATCHEDSTRING  */
  YYSYMBOL_UNMATCHEDCOMMENT = 45,          /* UNMATCHEDCOMMENT  */
  YYSYMBOL_IFX = 46,                       /* IFX  */
  YYSYMBOL_YYACCEPT = 47,                  /* $accept  */
  YYSYMBOL_p_oparen = 48,                  /* p_oparen  */
  YYSYMBOL_p_cparen = 49,                  /* p_cparen  */
  YYSYMBOL_p_ocurly = 50,                  /* p_ocurly  */
  YYSYMBOL_p_ccurly = 51,                  /* p_ccurly  */
  YYSYMBOL_p_semicolon = 52,               /* p_semicolon  */
  YYSYMBOL_p_comma = 53,                   /* p_comma  */
  YYSYMBOL_p_obracket = 54,                /* p_obracket  */
  YYSYMBOL_p_cbracket = 55,                /* p_cbracket  */
  YYSYMBOL_p_colon = 56,                   /* p_colon  */
  YYSYMBOL_op_assign = 57,                 /* op_assign  */
  YYSYMBOL_op_eq = 58,                     /* op_eq  */
  YYSYMBOL_op_neq = 59,                    /* op_neq  */
  YYSYMBOL_op_lt = 60,                     /* op_lt  */
  YYSYMBOL_op_gt = 61,                     /* op_gt  */
  YYSYMBOL_op_le = 62,                     /* op_le  */
  YYSYMBOL_op_ge = 63,                     /* op_ge  */
  YYSYMBOL_op_plus = 64,                   /* op_plus  */
  YYSYMBOL_op_minus = 65,                  /* op_minus  */
  YYSYMBOL_op_mult = 66,                   /* op_mult  */
  YYSYMBOL_op_divide = 67,                 /* op_divide  */
  YYSYMBOL_program = 68,                   /* program  */
  YYSYMBOL_preamble = 69,                  /* preamble  */
  YYSYMBOL_external_defs = 70,             /* external_defs  */
  YYSYMBOL_external_def = 71,              /* external_def  */
  YYSYMBOL_global_decl = 72,               /* global_decl  */
  YYSYMBOL_function_def = 73,              /* function_def  */
  YYSYMBOL_block = 74,                     /* block  */
  YYSYMBOL_75_1 = 75,                      /* $@1  */
  YYSYMBOL_76_2 = 76,                      /* $@2  */
  YYSYMBOL_decl_list = 77,                 /* decl_list  */
  YYSYMBOL_decl = 78,                      /* decl  */
  YYSYMBOL_type_specifier = 79,            /* type_specifier  */
  YYSYMBOL_declarator_var = 80,            /* declarator_var  */
  YYSYMBOL_declarator_array = 81,          /* declarator_array  */
  YYSYMBOL_declarator_func = 82,           /* declarator_func  */
  YYSYMBOL_params_list = 83,               /* params_list  */
  YYSYMBOL_param_decls = 84,               /* param_decls  */
  YYSYMBOL_param_decl = 85,                /* param_decl  */
  YYSYMBOL_stmt_list = 86,                 /* stmt_list  */
  YYSYMBOL_statement = 87,                 /* statement  */
  YYSYMBOL_if_stmt = 88,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 89,                /* while_stmt  */
  YYSYMBOL_for_stmt = 90,                  /* for_stmt  */
  YYSYMBOL_switch_stmt = 91,               /* switch_stmt  */
  YYSYMBOL_case_list = 92,                 /* case_list  */
  YYSYMBOL_case_block = 93,                /* case_block  */
  YYSYMBOL_case_labels = 94,               /* case_labels  */
  YYSYMBOL_case_label = 95,                /* case_label  */
  YYSYMBOL_break_stmt = 96,                /* break_stmt  */
  YYSYMBOL_for_init_stmt = 97,             /* for_init_stmt  */
  YYSYMBOL_optional_expr = 98,             /* optional_expr  */
  YYSYMBOL_printf_stmt = 99,               /* printf_stmt  */
  YYSYMBOL_return_stmt = 100,              /* return_stmt  */
  YYSYMBOL_expr_stmt = 101,                /* expr_stmt  */
  YYSYMBOL_expr = 102,                     /* expr  */
  YYSYMBOL_cond_expr = 103,                /* cond_expr  */
  YYSYMBOL_add_expr = 104,                 /* add_expr  */
  YYSYMBOL_mul_expr = 105,                 /* mul_expr  */
  YYSYMBOL_primary_expr = 106              /* primary_expr  */
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
#define YYLAST   250

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  60
/* YYNRULES -- Number of rules.  */
#define YYNRULES  118
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  195

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   301


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
      45,    46
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   235,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   260,   263,   265,   273,   282,   283,   287,   288,
     292,   298,   303,   307,   318,   318,   319,   319,   320,   323,
     324,   328,   334,   340,   345,   349,   350,   351,   352,   356,
     360,   368,   372,   382,   383,   388,   393,   394,   408,   419,
     420,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   439,   444,   453,   461,   469,   476,   478,
     482,   486,   487,   491,   495,   503,   511,   512,   513,   516,
     517,   521,   528,   536,   540,   547,   554,   558,   559,   563,
     569,   573,   574,   575,   576,   577,   578,   579,   583,   584,
     585,   589,   590,   591,   595,   597,   599,   601,   603
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
  "PRINTF", "SWITCH", "CASE", "DEFAULT", "BREAK", "EQ", "NEQ", "LT", "GT",
  "LE", "GE", "PLUS", "MINUS", "MULT", "DIVIDE", "ASSIGN", "OPAREN",
  "CPAREN", "OCURLY", "CCURLY", "SEMICOLON", "COMMA", "COLON", "OBRACKET",
  "CBRACKET", "QUOTE", "UNMATCHEDSTRING", "UNMATCHEDCOMMENT", "IFX",
  "$accept", "p_oparen", "p_cparen", "p_ocurly", "p_ccurly", "p_semicolon",
  "p_comma", "p_obracket", "p_cbracket", "p_colon", "op_assign", "op_eq",
  "op_neq", "op_lt", "op_gt", "op_le", "op_ge", "op_plus", "op_minus",
  "op_mult", "op_divide", "program", "preamble", "external_defs",
  "external_def", "global_decl", "function_def", "block", "$@1", "$@2",
  "decl_list", "decl", "type_specifier", "declarator_var",
  "declarator_array", "declarator_func", "params_list", "param_decls",
  "param_decl", "stmt_list", "statement", "if_stmt", "while_stmt",
  "for_stmt", "switch_stmt", "case_list", "case_block", "case_labels",
  "case_label", "break_stmt", "for_init_stmt", "optional_expr",
  "printf_stmt", "return_stmt", "expr_stmt", "expr", "cond_expr",
  "add_expr", "mul_expr", "primary_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-148)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-81)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -148,    36,    33,  -148,  -148,  -148,    73,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,    24,    15,    34,    15,    15,     6,
    -148,  -148,  -148,  -148,   128,    51,  -148,  -148,    20,  -148,
    -148,  -148,    39,    84,    41,    52,  -148,    64,    64,  -148,
    -148,  -148,   114,  -148,  -148,  -148,  -148,    73,  -148,  -148,
    -148,    73,    20,  -148,  -148,    91,   154,  -148,    15,   104,
     -14,    15,    28,   118,  -148,  -148,  -148,   119,   119,   119,
      76,   119,   119,    15,    17,  -148,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,    15,    97,
      88,   110,  -148,  -148,  -148,  -148,    17,  -148,  -148,    17,
      17,   212,    17,    15,    15,    15,    10,    17,  -148,    41,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,    66,    66,    66,
      66,    66,    66,  -148,  -148,    66,    66,  -148,  -148,    66,
      66,    15,  -148,    41,    15,  -148,  -148,    17,  -148,    41,
    -148,  -148,  -148,    41,     0,    41,  -148,  -148,    88,    88,
      88,    88,    88,    88,   110,   110,  -148,  -148,  -148,   192,
    -148,    15,  -148,   192,    15,    15,   149,   120,   140,    17,
    -148,  -148,  -148,    41,  -148,   192,    41,    15,    -3,  -148,
     192,  -148,    66,   121,  -148,  -148,   123,  -148,  -148,   121,
    -148,  -148,    92,  -148,  -148
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      23,     0,    26,     1,    24,    25,    22,    45,    46,    47,
      48,    27,    29,    28,     0,     0,    49,     0,     0,     0,
       6,    32,     2,     8,    53,     0,    30,    31,     0,     4,
      34,    33,    48,     0,     0,    55,    56,     0,     0,     5,
      38,    39,     0,    58,     3,    52,     7,     0,     9,    51,
      50,    59,     0,    57,    40,     0,     0,    37,     0,    49,
       0,     0,     0,   114,   115,   116,   117,     0,     0,     0,
       0,     0,     0,     0,     0,    35,    71,    70,    64,    60,
      65,    66,    67,    68,    69,    62,    63,    61,     0,   100,
     101,   108,   111,    44,    11,    41,     0,    43,    72,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    85,     0,
      97,    12,    13,    14,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    18,    19,     0,     0,    20,    21,     0,
       0,     0,    99,     0,     0,    88,    87,    89,    86,     0,
      96,    95,    94,     0,     0,     0,   118,   114,   106,   107,
     102,   103,   104,   105,   109,   110,   112,   113,    42,     0,
      98,     0,    90,     0,     0,     0,     0,     0,    73,    89,
      75,    93,    91,     0,    78,     0,     0,     0,     0,    74,
       0,    92,     0,     0,    77,    79,    59,    81,    76,     0,
      10,    84,     0,    82,    83
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -148,    -8,   -95,     1,   -51,   -15,    30,  -148,   137,   -12,
     115,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,   160,  -148,  -148,
    -148,   -45,     7,   125,   126,  -148,  -148,  -148,   135,    -2,
    -147,  -148,  -148,  -148,  -148,  -148,  -148,  -148,    -1,  -148,
    -148,    18,  -148,  -148,    82,   -70,  -148,    14,    22,  -120
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    74,    45,    30,    40,    76,    47,    25,    49,   191,
      96,   117,   118,   119,   120,   121,   122,   125,   126,   129,
     130,     1,     2,     6,    11,    12,    13,    77,    41,    42,
      51,    78,    55,    17,    18,    19,    34,    35,    36,    56,
      79,    80,    81,    82,    83,   178,   185,   186,   187,    84,
     137,   161,    85,    86,    87,    88,    89,    90,    91,    92
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      21,    57,    26,    27,   109,    75,    54,    28,    24,   156,
     157,   143,   168,    14,   146,   144,   170,   182,   183,    94,
      63,    64,    65,    66,    20,    15,   131,    16,   179,   132,
     133,    33,   139,   188,    39,    44,     3,   145,   159,    46,
       4,     5,    29,    93,   163,    95,    97,    98,   164,   165,
     167,    22,    37,    20,    33,    38,   136,    39,   108,   100,
     101,   102,   189,   106,   107,    39,    20,   162,    22,   147,
      64,    65,    66,   110,   -54,    23,    44,   103,   177,   104,
     105,   180,     7,     8,     9,    10,   135,    43,   140,   141,
     142,    46,    58,    62,    59,    63,    64,    65,    66,   162,
      22,     7,     8,     9,    10,    67,    48,    68,    69,    70,
      71,    72,   -80,   -80,    73,    52,   158,   123,   124,   160,
     111,   112,   113,   114,   115,   116,    22,   184,    29,   -80,
      20,   148,   149,   150,   151,   152,   153,     7,     8,     9,
      32,   127,   128,   182,   183,    23,   169,   154,   155,   171,
     172,    94,   173,    22,   175,    62,    29,    63,    64,    65,
      66,   190,   181,     7,     8,     9,    10,    67,   174,    68,
      69,    70,    71,    72,   166,    50,    73,   194,    99,    31,
      60,    61,    53,   138,   192,   193,     0,   176,    22,     0,
      29,    39,    20,    62,     0,    63,    64,    65,    66,     0,
       0,     7,     8,     9,    10,    67,     0,    68,    69,    70,
      71,    72,     0,   134,    73,    63,    64,    65,    66,     0,
       0,     7,     8,     9,    10,     0,    22,     0,    29,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
      20
};

static const yytype_int16 yycheck[] =
{
      15,    52,    17,    18,    74,    56,    51,     1,    16,   129,
     130,     1,   159,     6,   109,     5,   163,    20,    21,    33,
       3,     4,     5,     6,    38,     1,    96,     3,   175,    99,
     100,    24,   102,   180,    37,    35,     0,   107,   133,    39,
       7,     8,    36,    58,   139,    60,    61,    62,   143,   144,
     145,    34,     1,    38,    47,     4,   101,    37,    73,    67,
      68,    69,   182,    71,    72,    37,    38,   137,    34,     3,
       4,     5,     6,    88,    35,    41,    35,     1,   173,     3,
       4,   176,     9,    10,    11,    12,   101,     3,   103,   104,
     105,    39,     1,     1,     3,     3,     4,     5,     6,   169,
      34,     9,    10,    11,    12,    13,    42,    15,    16,    17,
      18,    19,    20,    21,    22,     1,   131,    29,    30,   134,
      23,    24,    25,    26,    27,    28,    34,   178,    36,    37,
      38,   117,   118,   119,   120,   121,   122,     9,    10,    11,
      12,    31,    32,    20,    21,    41,   161,   125,   126,   164,
     165,    33,     3,    34,    14,     1,    36,     3,     4,     5,
       6,    40,   177,     9,    10,    11,    12,    13,   167,    15,
      16,    17,    18,    19,   144,    38,    22,   189,    63,    19,
      55,    55,    47,   101,   186,   186,    -1,   169,    34,    -1,
      36,    37,    38,     1,    -1,     3,     4,     5,     6,    -1,
      -1,     9,    10,    11,    12,    13,    -1,    15,    16,    17,
      18,    19,    -1,     1,    22,     3,     4,     5,     6,    -1,
      -1,     9,    10,    11,    12,    -1,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      38
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    68,    69,     0,     7,     8,    70,     9,    10,    11,
      12,    71,    72,    73,    79,     1,     3,    80,    81,    82,
      38,    52,    34,    41,    48,    54,    52,    52,     1,    36,
      50,    74,    12,    79,    83,    84,    85,     1,     4,    37,
      51,    75,    76,     3,    35,    49,    39,    53,    42,    55,
      55,    77,     1,    85,    78,    79,    86,    51,     1,     3,
      80,    81,     1,     3,     4,     5,     6,    13,    15,    16,
      17,    18,    19,    22,    48,    51,    52,    74,    78,    87,
      88,    89,    90,    91,    96,    99,   100,   101,   102,   103,
     104,   105,   106,    52,    33,    52,    57,    52,    52,    57,
      48,    48,    48,     1,     3,     4,    48,    48,    52,   102,
      52,    23,    24,    25,    26,    27,    28,    58,    59,    60,
      61,    62,    63,    29,    30,    64,    65,    31,    32,    66,
      67,   102,   102,   102,     1,    52,    78,    97,   101,   102,
      52,    52,    52,     1,     5,   102,    49,     3,   104,   104,
     104,   104,   104,   104,   105,   105,   106,   106,    52,    49,
      52,    98,   102,    49,    49,    49,    53,    49,    87,    52,
      87,    52,    52,     3,    50,    14,    98,    49,    92,    87,
      49,    52,    20,    21,    51,    93,    94,    95,    87,   106,
      40,    56,    86,    95,    56
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    69,    69,    70,    70,    71,    71,
      72,    72,    72,    73,    75,    74,    76,    74,    74,    77,
      77,    78,    78,    78,    78,    79,    79,    79,    79,    80,
      81,    81,    82,    83,    83,    83,    84,    84,    85,    86,
      86,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    88,    88,    89,    90,    91,    92,    92,
      93,    94,    94,    95,    95,    96,    97,    97,    97,    98,
      98,    99,    99,    99,   100,   100,   100,   101,   101,   102,
     102,   103,   103,   103,   103,   103,   103,   103,   104,   104,
     104,   105,   105,   105,   106,   106,   106,   106,   106
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     0,     2,     2,     0,     2,     1,     1,
       3,     3,     3,     3,     0,     5,     0,     4,     2,     0,
       2,     3,     5,     3,     3,     1,     1,     1,     1,     1,
       4,     4,     4,     0,     1,     1,     1,     3,     2,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     5,     7,     5,     8,     7,     0,     2,
       2,     1,     2,     3,     2,     2,     1,     1,     1,     0,
       1,     5,     7,     5,     3,     3,     3,     2,     2,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     1,     1,     1,     3
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
#line 235 "parser.y"
                       { sym_insert_or_update("(", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1528 "parser.tab.c"
    break;

  case 3: /* p_cparen: CPAREN  */
#line 236 "parser.y"
                       { sym_insert_or_update(")", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1534 "parser.tab.c"
    break;

  case 4: /* p_ocurly: OCURLY  */
#line 237 "parser.y"
                       { sym_insert_or_update("{", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1540 "parser.tab.c"
    break;

  case 5: /* p_ccurly: CCURLY  */
#line 238 "parser.y"
                       { sym_insert_or_update("}", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1546 "parser.tab.c"
    break;

  case 6: /* p_semicolon: SEMICOLON  */
#line 239 "parser.y"
                       { sym_insert_or_update(";", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1552 "parser.tab.c"
    break;

  case 7: /* p_comma: COMMA  */
#line 240 "parser.y"
                       { sym_insert_or_update(",", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1558 "parser.tab.c"
    break;

  case 8: /* p_obracket: OBRACKET  */
#line 241 "parser.y"
                       { sym_insert_or_update("[", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1564 "parser.tab.c"
    break;

  case 9: /* p_cbracket: CBRACKET  */
#line 242 "parser.y"
                       { sym_insert_or_update("]", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1570 "parser.tab.c"
    break;

  case 10: /* p_colon: COLON  */
#line 243 "parser.y"
                       { sym_insert_or_update(":", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1576 "parser.tab.c"
    break;

  case 11: /* op_assign: ASSIGN  */
#line 245 "parser.y"
                       { sym_insert_or_update("=", "operator", "assignment", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1582 "parser.tab.c"
    break;

  case 12: /* op_eq: EQ  */
#line 246 "parser.y"
                       { sym_insert_or_update("==", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1588 "parser.tab.c"
    break;

  case 13: /* op_neq: NEQ  */
#line 247 "parser.y"
                       { sym_insert_or_update("!=", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1594 "parser.tab.c"
    break;

  case 14: /* op_lt: LT  */
#line 248 "parser.y"
                       { sym_insert_or_update("<", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1600 "parser.tab.c"
    break;

  case 15: /* op_gt: GT  */
#line 249 "parser.y"
                       { sym_insert_or_update(">", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1606 "parser.tab.c"
    break;

  case 16: /* op_le: LE  */
#line 250 "parser.y"
                       { sym_insert_or_update("<=", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1612 "parser.tab.c"
    break;

  case 17: /* op_ge: GE  */
#line 251 "parser.y"
                       { sym_insert_or_update(">=", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1618 "parser.tab.c"
    break;

  case 18: /* op_plus: PLUS  */
#line 252 "parser.y"
                       { sym_insert_or_update("+", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1624 "parser.tab.c"
    break;

  case 19: /* op_minus: MINUS  */
#line 253 "parser.y"
                       { sym_insert_or_update("-", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1630 "parser.tab.c"
    break;

  case 20: /* op_mult: MULT  */
#line 254 "parser.y"
                       { sym_insert_or_update("*", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1636 "parser.tab.c"
    break;

  case 21: /* op_divide: DIVIDE  */
#line 255 "parser.y"
                       { sym_insert_or_update("/", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); }
#line 1642 "parser.tab.c"
    break;

  case 24: /* preamble: preamble INCLUDE  */
#line 266 "parser.y"
        {
            if ((yyvsp[0].str)) {
                sym_insert_or_update("#include", "preprocessor", "directive", NULL, NULL, NULL, 0, 0, yylineno);
                sym_insert_or_update((yyvsp[0].str), "header", "file", NULL, NULL, NULL, 0, 0, yylineno);
                free((yyvsp[0].str));
            }
        }
#line 1654 "parser.tab.c"
    break;

  case 25: /* preamble: preamble PREPROCESSOR  */
#line 274 "parser.y"
        {
             if ((yyvsp[0].str)) {
                sym_insert_or_update((yyvsp[0].str), "preprocessor", "directive", NULL, NULL, NULL, 0, 0, yylineno);
                free((yyvsp[0].str));
            }
        }
#line 1665 "parser.tab.c"
    break;

  case 30: /* global_decl: type_specifier declarator_var p_semicolon  */
#line 293 "parser.y"
        {
            printf("Line %d: Parsed a global variable declaration for '%s'.\n", yylineno, (yyvsp[-1].str));
            sym_insert_or_update((yyvsp[-1].str), "identifier", "variable", NULL, NULL, NULL, 0, current_nesting, yylineno); 
            free((yyvsp[-2].str)); free((yyvsp[-1].str));
        }
#line 1675 "parser.tab.c"
    break;

  case 31: /* global_decl: type_specifier declarator_array p_semicolon  */
#line 299 "parser.y"
        {
            printf("Line %d: Parsed a global array declaration.\n", yylineno);
            free((yyvsp[-2].str));
        }
#line 1684 "parser.tab.c"
    break;

  case 32: /* global_decl: type_specifier error p_semicolon  */
#line 303 "parser.y"
                                       { yyerrok; }
#line 1690 "parser.tab.c"
    break;

  case 33: /* function_def: type_specifier declarator_func block  */
#line 308 "parser.y"
    {
        printf("Line %d: Parsed a function definition for '%s'.\n", yylineno, (yyvsp[-1].str));
        if((yyvsp[-1].str)) { 
            sym_insert_or_update((yyvsp[-1].str), (yyvsp[-2].str), "function", NULL, NULL, yylval.str, 1, current_nesting, yylineno);
            free((yyvsp[-1].str));
        }
        free((yyvsp[-2].str));
    }
#line 1703 "parser.tab.c"
    break;

  case 34: /* $@1: %empty  */
#line 318 "parser.y"
             { current_nesting++; }
#line 1709 "parser.tab.c"
    break;

  case 35: /* block: p_ocurly $@1 decl_list stmt_list p_ccurly  */
#line 318 "parser.y"
                                                                 { current_nesting--; }
#line 1715 "parser.tab.c"
    break;

  case 36: /* $@2: %empty  */
#line 319 "parser.y"
             { current_nesting++; }
#line 1721 "parser.tab.c"
    break;

  case 37: /* block: p_ocurly $@2 error p_ccurly  */
#line 319 "parser.y"
                                                   { yyerrok; current_nesting--; }
#line 1727 "parser.tab.c"
    break;

  case 38: /* block: error p_ccurly  */
#line 320 "parser.y"
                   { yyerrok; }
#line 1733 "parser.tab.c"
    break;

  case 41: /* decl: type_specifier declarator_var p_semicolon  */
#line 329 "parser.y"
        {
            printf("Line %d: Parsed a local variable declaration for '%s'.\n", yylineno, (yyvsp[-1].str));
            sym_insert_or_update((yyvsp[-1].str), "identifier", "variable", NULL, NULL, NULL, 0, current_nesting, yylineno);
            free((yyvsp[-2].str)); free((yyvsp[-1].str));
        }
#line 1743 "parser.tab.c"
    break;

  case 42: /* decl: type_specifier declarator_var op_assign expr p_semicolon  */
#line 335 "parser.y"
        {
            printf("Line %d: Parsed a local variable declaration with initialization for '%s'.\n", yylineno, (yyvsp[-3].str));
            sym_insert_or_update((yyvsp[-3].str), "identifier", "variable", NULL, NULL, NULL, 0, current_nesting, yylineno);
            free((yyvsp[-4].str)); free((yyvsp[-3].str));
        }
#line 1753 "parser.tab.c"
    break;

  case 43: /* decl: type_specifier declarator_array p_semicolon  */
#line 341 "parser.y"
        { 
            printf("Line %d: Parsed a local array declaration.\n", yylineno);
            /* Action handled in declarator_array */ 
        }
#line 1762 "parser.tab.c"
    break;

  case 44: /* decl: type_specifier error p_semicolon  */
#line 345 "parser.y"
                                       { yyerrok; }
#line 1768 "parser.tab.c"
    break;

  case 45: /* type_specifier: INT  */
#line 349 "parser.y"
            { sym_insert_or_update("int", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);   (yyval.str) = xstrdup("int"); }
#line 1774 "parser.tab.c"
    break;

  case 46: /* type_specifier: FLOAT  */
#line 350 "parser.y"
            { sym_insert_or_update("float", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno); (yyval.str) = xstrdup("float"); }
#line 1780 "parser.tab.c"
    break;

  case 47: /* type_specifier: CHAR  */
#line 351 "parser.y"
            { sym_insert_or_update("char", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);  (yyval.str) = xstrdup("char"); }
#line 1786 "parser.tab.c"
    break;

  case 48: /* type_specifier: VOID  */
#line 352 "parser.y"
            { sym_insert_or_update("void", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);  (yyval.str) = xstrdup("void"); }
#line 1792 "parser.tab.c"
    break;

  case 49: /* declarator_var: IDENTIFIER  */
#line 356 "parser.y"
                 { (yyval.str) = (yyvsp[0].str); }
#line 1798 "parser.tab.c"
    break;

  case 50: /* declarator_array: IDENTIFIER p_obracket NUMBER p_cbracket  */
#line 361 "parser.y"
        {
            char dims[128]; snprintf(dims,sizeof(dims),"[%s]", (yyvsp[-1].str));
            sym_insert_or_update((yyvsp[-3].str), "array", "variable", NULL, dims, NULL, 0, current_nesting, yylineno);
            const_insert_or_bump((yyvsp[-1].str), "int", yylineno);
            free((yyvsp[-1].str));
            (yyval.str) = (yyvsp[-3].str);
        }
#line 1810 "parser.tab.c"
    break;

  case 51: /* declarator_array: IDENTIFIER p_obracket error p_cbracket  */
#line 368 "parser.y"
                                             { yyerrok; (yyval.str) = (yyvsp[-3].str); }
#line 1816 "parser.tab.c"
    break;

  case 52: /* declarator_func: IDENTIFIER p_oparen params_list p_cparen  */
#line 373 "parser.y"
        {
            sym_insert_or_update((yyvsp[-3].str), "function", "function", NULL, NULL, (yyvsp[-1].str), 0, current_nesting, yylineno);
            (yyval.str) = (yyvsp[-3].str);         
            yylval.str = (yyvsp[-1].str); 
        }
#line 1826 "parser.tab.c"
    break;

  case 53: /* params_list: %empty  */
#line 382 "parser.y"
      { (yyval.str) = xstrdup(""); }
#line 1832 "parser.tab.c"
    break;

  case 54: /* params_list: VOID  */
#line 384 "parser.y"
      {
          sym_insert_or_update("void", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);
          (yyval.str) = xstrdup("void");
      }
#line 1841 "parser.tab.c"
    break;

  case 55: /* params_list: param_decls  */
#line 389 "parser.y"
      { (yyval.str) = (yyvsp[0].str); }
#line 1847 "parser.tab.c"
    break;

  case 57: /* param_decls: param_decls p_comma param_decl  */
#line 395 "parser.y"
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
#line 1862 "parser.tab.c"
    break;

  case 58: /* param_decl: type_specifier IDENTIFIER  */
#line 409 "parser.y"
        {
            sym_insert_or_update((yyvsp[0].str), "identifier", "parameter", NULL, NULL, NULL, 0, current_nesting+1, yylineno); 
            size_t n = strlen((yyvsp[-1].str)) + strlen((yyvsp[0].str)) + 2;
            char *s = malloc(n);
            snprintf(s, n, "%s %s", (yyvsp[-1].str), (yyvsp[0].str));
            free((yyvsp[-1].str)); free((yyvsp[0].str));
            (yyval.str) = s;
        }
#line 1875 "parser.tab.c"
    break;

  case 72: /* statement: error p_semicolon  */
#line 435 "parser.y"
                        { yyerrok; }
#line 1881 "parser.tab.c"
    break;

  case 73: /* if_stmt: IF p_oparen expr p_cparen statement  */
#line 440 "parser.y"
        { 
            sym_insert_or_update("if", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed an if statement.\n", yylineno);
        }
#line 1890 "parser.tab.c"
    break;

  case 74: /* if_stmt: IF p_oparen expr p_cparen statement ELSE statement  */
#line 445 "parser.y"
        { 
            sym_insert_or_update("if", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            sym_insert_or_update("else", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed an if-else statement.\n", yylineno);
        }
#line 1900 "parser.tab.c"
    break;

  case 75: /* while_stmt: WHILE p_oparen expr p_cparen statement  */
#line 454 "parser.y"
        { 
            sym_insert_or_update("while", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a while loop.\n", yylineno);
        }
#line 1909 "parser.tab.c"
    break;

  case 76: /* for_stmt: FOR p_oparen for_init_stmt optional_expr p_semicolon optional_expr p_cparen statement  */
#line 462 "parser.y"
        { 
            sym_insert_or_update("for", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a for loop.\n", yylineno);
        }
#line 1918 "parser.tab.c"
    break;

  case 77: /* switch_stmt: SWITCH p_oparen expr p_cparen p_ocurly case_list p_ccurly  */
#line 470 "parser.y"
        {
            sym_insert_or_update("switch", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a switch statement.\n", yylineno);
        }
#line 1927 "parser.tab.c"
    break;

  case 83: /* case_label: CASE primary_expr p_colon  */
#line 492 "parser.y"
        {
            sym_insert_or_update("case", "keyword", "label", NULL, NULL, NULL, 0, 0, yylineno);
        }
#line 1935 "parser.tab.c"
    break;

  case 84: /* case_label: DEFAULT p_colon  */
#line 496 "parser.y"
        {
            sym_insert_or_update("default", "keyword", "label", NULL, NULL, NULL, 0, 0, yylineno);
        }
#line 1943 "parser.tab.c"
    break;

  case 85: /* break_stmt: BREAK p_semicolon  */
#line 504 "parser.y"
        {
            sym_insert_or_update("break", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a break statement.\n", yylineno);
        }
#line 1952 "parser.tab.c"
    break;

  case 91: /* printf_stmt: PRINTF p_oparen STRING_LITERAL p_cparen p_semicolon  */
#line 522 "parser.y"
        { 
            sym_insert_or_update("printf", "keyword", "function", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a printf statement.\n", yylineno);
            const_insert_or_bump((yyvsp[-2].str), "string", yylineno);
            free((yyvsp[-2].str));
        }
#line 1963 "parser.tab.c"
    break;

  case 92: /* printf_stmt: PRINTF p_oparen STRING_LITERAL p_comma IDENTIFIER p_cparen p_semicolon  */
#line 529 "parser.y"
        { 
            sym_insert_or_update("printf", "keyword", "function", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a printf statement with variable '%s'.\n", yylineno, (yyvsp[-2].str));
            const_insert_or_bump((yyvsp[-4].str), "string", yylineno);
            sym_add_reference((yyvsp[-2].str), yylineno);
            free((yyvsp[-4].str)); free((yyvsp[-2].str));
        }
#line 1975 "parser.tab.c"
    break;

  case 93: /* printf_stmt: PRINTF p_oparen error p_cparen p_semicolon  */
#line 536 "parser.y"
                                                 { yyerrok; }
#line 1981 "parser.tab.c"
    break;

  case 94: /* return_stmt: RETURN NUMBER p_semicolon  */
#line 541 "parser.y"
        { 
            sym_insert_or_update("return", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a return statement with value %s.\n", yylineno, (yyvsp[-1].str));
            const_insert_or_bump((yyvsp[-1].str), "int", yylineno);
            free((yyvsp[-1].str));
        }
#line 1992 "parser.tab.c"
    break;

  case 95: /* return_stmt: RETURN IDENTIFIER p_semicolon  */
#line 548 "parser.y"
        { 
            sym_insert_or_update("return", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a return statement with variable '%s'.\n", yylineno, (yyvsp[-1].str));
            sym_add_reference((yyvsp[-1].str), yylineno);
            free((yyvsp[-1].str));
        }
#line 2003 "parser.tab.c"
    break;

  case 96: /* return_stmt: RETURN error p_semicolon  */
#line 554 "parser.y"
                               { yyerrok; }
#line 2009 "parser.tab.c"
    break;

  case 98: /* expr_stmt: error p_semicolon  */
#line 559 "parser.y"
                        { yyerrok; }
#line 2015 "parser.tab.c"
    break;

  case 99: /* expr: IDENTIFIER op_assign expr  */
#line 564 "parser.y"
        { 
            printf("Line %d: Parsed an assignment to variable '%s'.\n", yylineno, (yyvsp[-2].str));
            sym_add_reference((yyvsp[-2].str), yylineno);
            free((yyvsp[-2].str));
        }
#line 2025 "parser.tab.c"
    break;

  case 114: /* primary_expr: IDENTIFIER  */
#line 596 "parser.y"
        { sym_add_reference((yyvsp[0].str), yylineno); free((yyvsp[0].str)); }
#line 2031 "parser.tab.c"
    break;

  case 115: /* primary_expr: NUMBER  */
#line 598 "parser.y"
        { const_insert_or_bump((yyvsp[0].str), "int", yylineno); free((yyvsp[0].str)); }
#line 2037 "parser.tab.c"
    break;

  case 116: /* primary_expr: STRING_LITERAL  */
#line 600 "parser.y"
        { const_insert_or_bump((yyvsp[0].str), "string", yylineno); free((yyvsp[0].str)); }
#line 2043 "parser.tab.c"
    break;

  case 117: /* primary_expr: CHAR_LITERAL  */
#line 602 "parser.y"
        { const_insert_or_bump((yyvsp[0].str), "char", yylineno); free((yyvsp[0].str)); }
#line 2049 "parser.tab.c"
    break;


#line 2053 "parser.tab.c"

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

#line 606 "parser.y"


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
