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

#define MAX_SYMBOLS 500
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
    char type[64];
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
static void sym_insert_or_update (const char *name, const char *type, const char *class,
                                 const char *boundaries, const char *array_dims, const char *params,
                                 int proc_def, int nesting, int declared_line)
{
    int idx = sym_find_index(name);
    if (idx >= 0) {
        Symbol *s = &symtab[idx];
        s->frequency++;
        if (type && strcmp(type, "-") != 0)
            strncpy(s->type, type, sizeof(s->type) - 1);
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
        return;
    }
    if(sym_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Symbol table full, cannot insert %s\n", name);
        return;
    }

    Symbol *s = &symtab[sym_count++];
    memset(s, 0, sizeof(*s));
    strncpy(s->name, name, sizeof(s->name) - 1);
    strncpy(s->type, type ? type : "-", sizeof(s->type) - 1);
    strncpy(s->class, class ? class : "-", sizeof(s->class) - 1);
    strncpy(s->boundaries, boundaries ? boundaries : "-", sizeof(s->boundaries) - 1);
    strncpy(s->array_dims, array_dims ? array_dims : "-", sizeof(s->array_dims) - 1);
    strncpy(s->params, params ? params : "-", sizeof(s->params) - 1);
    s->proc_def = proc_def;
    s->nesting_level = nesting;
    s->declared_line = declared_line;
    s->frequency = 1;
    s->ref_count = 0;
}

/* add reference line */
static void sym_add_reference(const char *name, int line) {
    int idx = sym_find_index(name);
    if (idx < 0) {
        sym_insert_or_update(name, "-", "-", "-", "-", "-", 0, current_nesting, -1);
        idx = sym_find_index(name);
        if(idx < 0) return;
    }
    Symbol *s = &symtab[idx];
    s->frequency++;
    if(s->ref_count < MAX_REFS_PER_SYM)
        s->ref_lines[s->ref_count++] = line;
}

/* constant table */
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
    printf("%-18s %-8s %-10s %-14s %-12s %-20s %-4s %-6s %-6s %-6s %-s\n",
           "Name","Type","Class","Boundaries","ArrayDims","Parameters",
           "PDef","Nest","DeclLn","Freq","RefLines");
    printf("--------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0 ; i < sym_count ; i++) {
        Symbol *s = &symtab[i];
        printf("%-18s %-8s %-10s %-14s %-12s %-20s %-4d %-6d %-6d %-6d ",
               s->name, s->type, s->class, s->boundaries, s->array_dims,
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
    printf("\n================= CONSTANT TABLE =================\n");
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


#line 274 "parser.tab.c"

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
  YYSYMBOL_INT = 8,                        /* INT  */
  YYSYMBOL_FLOAT = 9,                      /* FLOAT  */
  YYSYMBOL_CHAR = 10,                      /* CHAR  */
  YYSYMBOL_VOID = 11,                      /* VOID  */
  YYSYMBOL_IF = 12,                        /* IF  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_FOR = 14,                       /* FOR  */
  YYSYMBOL_WHILE = 15,                     /* WHILE  */
  YYSYMBOL_RETURN = 16,                    /* RETURN  */
  YYSYMBOL_PRINTF = 17,                    /* PRINTF  */
  YYSYMBOL_EQ = 18,                        /* EQ  */
  YYSYMBOL_NEQ = 19,                       /* NEQ  */
  YYSYMBOL_LT = 20,                        /* LT  */
  YYSYMBOL_GT = 21,                        /* GT  */
  YYSYMBOL_LE = 22,                        /* LE  */
  YYSYMBOL_GE = 23,                        /* GE  */
  YYSYMBOL_PLUS = 24,                      /* PLUS  */
  YYSYMBOL_MINUS = 25,                     /* MINUS  */
  YYSYMBOL_MULT = 26,                      /* MULT  */
  YYSYMBOL_DIVIDE = 27,                    /* DIVIDE  */
  YYSYMBOL_ASSIGN = 28,                    /* ASSIGN  */
  YYSYMBOL_OPAREN = 29,                    /* OPAREN  */
  YYSYMBOL_CPAREN = 30,                    /* CPAREN  */
  YYSYMBOL_OCURLY = 31,                    /* OCURLY  */
  YYSYMBOL_CCURLY = 32,                    /* CCURLY  */
  YYSYMBOL_SEMICOLON = 33,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 34,                     /* COMMA  */
  YYSYMBOL_OBRACKET = 35,                  /* OBRACKET  */
  YYSYMBOL_CBRACKET = 36,                  /* CBRACKET  */
  YYSYMBOL_QUOTE = 37,                     /* QUOTE  */
  YYSYMBOL_UNMATCHEDSTRING = 38,           /* UNMATCHEDSTRING  */
  YYSYMBOL_PREPROCESSOR = 39,              /* PREPROCESSOR  */
  YYSYMBOL_UNMATCHEDCOMMENT = 40,          /* UNMATCHEDCOMMENT  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_program = 42,                   /* program  */
  YYSYMBOL_includes = 43,                  /* includes  */
  YYSYMBOL_external_defs = 44,             /* external_defs  */
  YYSYMBOL_external_def = 45,              /* external_def  */
  YYSYMBOL_global_decl = 46,               /* global_decl  */
  YYSYMBOL_function_def = 47,              /* function_def  */
  YYSYMBOL_block = 48,                     /* block  */
  YYSYMBOL_49_1 = 49,                      /* $@1  */
  YYSYMBOL_50_2 = 50,                      /* $@2  */
  YYSYMBOL_decl_list = 51,                 /* decl_list  */
  YYSYMBOL_decl = 52,                      /* decl  */
  YYSYMBOL_type_specifier = 53,            /* type_specifier  */
  YYSYMBOL_declarator_var = 54,            /* declarator_var  */
  YYSYMBOL_declarator_array = 55,          /* declarator_array  */
  YYSYMBOL_declarator_func = 56,           /* declarator_func  */
  YYSYMBOL_params_list = 57,               /* params_list  */
  YYSYMBOL_param_decl = 58,                /* param_decl  */
  YYSYMBOL_stmt_list = 59,                 /* stmt_list  */
  YYSYMBOL_statement = 60,                 /* statement  */
  YYSYMBOL_local_decl = 61,                /* local_decl  */
  YYSYMBOL_printf_stmt = 62,               /* printf_stmt  */
  YYSYMBOL_return_stmt = 63,               /* return_stmt  */
  YYSYMBOL_expr_stmt = 64,                 /* expr_stmt  */
  YYSYMBOL_expr = 65                       /* expr  */
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
typedef yytype_int8 yy_state_t;

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
#define YYLAST   97

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  100

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   227,   227,   230,   232,   236,   237,   241,   242,   246,
     251,   256,   264,   276,   276,   277,   277,   283,   290,   292,
     296,   301,   306,   315,   316,   317,   318,   323,   327,   335,
     346,   353,   363,   364,   365,   378,   387,   398,   400,   404,
     405,   406,   407,   408,   417,   422,   427,   431,   440,   445,
     451,   460,   465,   470,   479,   480,   488,   493,   498,   503
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
  "STRING_LITERAL", "CHAR_LITERAL", "INCLUDE", "INT", "FLOAT", "CHAR",
  "VOID", "IF", "ELSE", "FOR", "WHILE", "RETURN", "PRINTF", "EQ", "NEQ",
  "LT", "GT", "LE", "GE", "PLUS", "MINUS", "MULT", "DIVIDE", "ASSIGN",
  "OPAREN", "CPAREN", "OCURLY", "CCURLY", "SEMICOLON", "COMMA", "OBRACKET",
  "CBRACKET", "QUOTE", "UNMATCHEDSTRING", "PREPROCESSOR",
  "UNMATCHEDCOMMENT", "$accept", "program", "includes", "external_defs",
  "external_def", "global_decl", "function_def", "block", "$@1", "$@2",
  "decl_list", "decl", "type_specifier", "declarator_var",
  "declarator_array", "declarator_func", "params_list", "param_decl",
  "stmt_list", "statement", "local_decl", "printf_stmt", "return_stmt",
  "expr_stmt", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-65)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-33)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -65,    18,    -2,   -65,   -65,    41,   -65,   -65,   -65,   -65,
     -65,   -65,   -65,    30,    -8,   -22,    -7,    13,     1,   -65,
       0,    37,   -65,   -65,    22,   -65,   -65,    28,    59,     5,
     -65,    29,    31,   -65,   -65,    63,   -65,   -65,   -65,    41,
     -65,   -65,    41,    34,   -65,   -65,    44,    11,   -65,    36,
      33,    38,    39,    40,    42,   -65,    52,    45,   -65,    60,
     -65,   -65,   -65,   -65,   -65,    43,   -65,   -65,   -65,   -65,
      56,    46,    47,    48,    35,    49,    -4,    50,   -65,   -65,
     -65,   -65,   -65,   -65,    54,    14,   -65,    56,   -65,   -65,
      53,    55,    72,    57,   -65,   -65,    61,   -65,    62,   -65
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     6,     1,     4,     2,    23,    24,    25,    26,
       5,     8,     7,     0,     0,    27,     0,     0,     0,    11,
       0,     0,     9,    10,     0,    13,    12,    35,     0,     0,
      33,     0,     0,    17,    18,     0,    31,    36,    30,     0,
      29,    28,    37,     0,    34,    19,     0,     0,    16,     0,
      27,     0,     0,     0,    58,    57,     0,     0,    14,     0,
      38,    42,    40,    41,    39,     0,    22,    20,    21,    43,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    59,
      56,    53,    52,    51,     0,     0,    47,     0,    44,    46,
       0,     0,     0,     0,    50,    48,     0,    45,     0,    49
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,
     -65,   -65,    -5,   -43,   -42,   -65,   -65,    58,   -65,   -65,
     -65,   -65,   -65,   -65,   -64
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     5,    10,    11,    12,    26,    34,    35,
      42,    45,    28,    16,    17,    18,    29,    30,    47,    60,
      61,    62,    63,    64,    65
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      13,    27,    24,    51,    52,     4,    80,    20,     6,     7,
       8,     9,    53,    21,    54,    55,    76,    77,     3,     6,
       7,     8,     9,    93,    87,    19,    22,    56,    57,    88,
     -32,    14,    25,    15,   -32,    38,    84,    46,    31,    39,
      85,    32,    59,    58,    91,    49,    23,    50,    92,     6,
       7,     8,     9,    71,    33,    72,    73,    79,    36,    54,
      55,    75,    37,    50,    43,    40,    48,    41,    21,    66,
      70,    67,    68,    69,    74,    96,    78,     0,     0,    81,
      82,    83,    86,    89,    90,     0,    94,     0,    95,     0,
      97,    98,     0,     0,     0,    99,     0,    44
};

static const yytype_int8 yycheck[] =
{
       5,     1,     1,    46,    46,     7,    70,    29,     8,     9,
      10,    11,     1,    35,     3,     4,    59,    59,     0,     8,
       9,    10,    11,    87,    28,    33,    33,    16,    17,    33,
      30,     1,    31,     3,    34,    30,     1,    42,     1,    34,
       5,     4,    47,    32,    30,     1,    33,     3,    34,     8,
       9,    10,    11,     1,    32,     3,     4,     1,    30,     3,
       4,     1,     3,     3,     1,    36,    32,    36,    35,    33,
      28,    33,    33,    33,    29,     3,    33,    -1,    -1,    33,
      33,    33,    33,    33,    30,    -1,    33,    -1,    33,    -1,
      33,    30,    -1,    -1,    -1,    33,    -1,    39
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    42,    43,     0,     7,    44,     8,     9,    10,    11,
      45,    46,    47,    53,     1,     3,    54,    55,    56,    33,
      29,    35,    33,    33,     1,    31,    48,     1,    53,    57,
      58,     1,     4,    32,    49,    50,    30,     3,    30,    34,
      36,    36,    51,     1,    58,    52,    53,    59,    32,     1,
       3,    54,    55,     1,     3,     4,    16,    17,    32,    53,
      60,    61,    62,    63,    64,    65,    33,    33,    33,    33,
      28,     1,     3,     4,    29,     1,    54,    55,    33,     1,
      65,    33,    33,    33,     1,     5,    33,    28,    33,    33,
      30,    30,    34,    65,    33,    33,     3,    33,    30,    33
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    45,    45,    46,
      46,    46,    47,    49,    48,    50,    48,    48,    51,    51,
      52,    52,    52,    53,    53,    53,    53,    54,    55,    55,
      56,    56,    57,    57,    57,    57,    58,    59,    59,    60,
      60,    60,    60,    60,    61,    61,    61,    61,    62,    62,
      62,    63,    63,    63,    64,    64,    65,    65,    65,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     2,     2,     0,     1,     1,     3,
       3,     3,     3,     0,     5,     0,     4,     2,     0,     2,
       3,     3,     3,     1,     1,     1,     1,     1,     4,     4,
       4,     4,     0,     1,     3,     1,     2,     0,     2,     1,
       1,     1,     1,     2,     3,     5,     3,     3,     5,     7,
       5,     3,     3,     3,     2,     2,     3,     1,     1,     1
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
  case 4: /* includes: includes INCLUDE  */
#line 232 "parser.y"
                                 { /* include token possibly with yylval.str from lexer */ }
#line 1390 "parser.tab.c"
    break;

  case 9: /* global_decl: type_specifier declarator_var SEMICOLON  */
#line 247 "parser.y"
        {
            sym_insert_or_update((yyvsp[-1].str), (yyvsp[-2].str), "variable", "-", "-", "-", 0, current_nesting, yylineno);
            free((yyvsp[-2].str)); free((yyvsp[-1].str));
        }
#line 1399 "parser.tab.c"
    break;

  case 10: /* global_decl: type_specifier declarator_array SEMICOLON  */
#line 252 "parser.y"
        {
            /* declarator_array inserted symbol already inside its action */
            free((yyvsp[-2].str));
        }
#line 1408 "parser.tab.c"
    break;

  case 11: /* global_decl: type_specifier error SEMICOLON  */
#line 257 "parser.y"
        {
            fprintf(stderr, "Error at line %d: invalid global declaration\n", yylineno);
            yyerrok;
        }
#line 1417 "parser.tab.c"
    break;

  case 12: /* function_def: type_specifier declarator_func block  */
#line 265 "parser.y"
        {
            if((yyvsp[-1].str)){
                sym_insert_or_update((yyvsp[-1].str), (yyvsp[-2].str), "function", "-", "-", "-", 1, current_nesting, yylineno);
                free((yyvsp[-1].str));
            }
            free((yyvsp[-2].str));
        }
#line 1429 "parser.tab.c"
    break;

  case 13: /* $@1: %empty  */
#line 276 "parser.y"
           { current_nesting++; }
#line 1435 "parser.tab.c"
    break;

  case 14: /* block: OCURLY $@1 decl_list stmt_list CCURLY  */
#line 276 "parser.y"
                                                             { current_nesting--; }
#line 1441 "parser.tab.c"
    break;

  case 15: /* $@2: %empty  */
#line 277 "parser.y"
           { current_nesting++; }
#line 1447 "parser.tab.c"
    break;

  case 16: /* block: OCURLY $@2 error CCURLY  */
#line 278 "parser.y"
        {
            fprintf(stderr, "Error at line %d: invalid block contents\n", yylineno);
            yyerrok;
            current_nesting--;
        }
#line 1457 "parser.tab.c"
    break;

  case 17: /* block: error CCURLY  */
#line 284 "parser.y"
        {
            fprintf(stderr, "Error at line %d: missing '{' before block\n", yylineno);
            yyerrok;
        }
#line 1466 "parser.tab.c"
    break;

  case 20: /* decl: type_specifier declarator_var SEMICOLON  */
#line 297 "parser.y"
        {
            sym_insert_or_update((yyvsp[-1].str), (yyvsp[-2].str), "variable", "-", "-", "-", 0, current_nesting, yylineno);
            free((yyvsp[-2].str)); free((yyvsp[-1].str));
        }
#line 1475 "parser.tab.c"
    break;

  case 21: /* decl: type_specifier declarator_array SEMICOLON  */
#line 302 "parser.y"
        {
            /* declarator_array handled in its action */
            free((yyvsp[-2].str));
        }
#line 1484 "parser.tab.c"
    break;

  case 22: /* decl: type_specifier error SEMICOLON  */
#line 307 "parser.y"
        {
            fprintf(stderr, "Error at line %d: invalid local declaration\n", yylineno);
            yyerrok;
        }
#line 1493 "parser.tab.c"
    break;

  case 23: /* type_specifier: INT  */
#line 315 "parser.y"
            { (yyval.str) = xstrdup("int"); }
#line 1499 "parser.tab.c"
    break;

  case 24: /* type_specifier: FLOAT  */
#line 316 "parser.y"
            { (yyval.str) = xstrdup("float"); }
#line 1505 "parser.tab.c"
    break;

  case 25: /* type_specifier: CHAR  */
#line 317 "parser.y"
            { (yyval.str) = xstrdup("char"); }
#line 1511 "parser.tab.c"
    break;

  case 26: /* type_specifier: VOID  */
#line 318 "parser.y"
            { (yyval.str) = xstrdup("void"); }
#line 1517 "parser.tab.c"
    break;

  case 27: /* declarator_var: IDENTIFIER  */
#line 323 "parser.y"
                 { (yyval.str) = (yyvsp[0].str); }
#line 1523 "parser.tab.c"
    break;

  case 28: /* declarator_array: IDENTIFIER OBRACKET NUMBER CBRACKET  */
#line 328 "parser.y"
        {
            char dims[128]; snprintf(dims,sizeof(dims),"[%s]", (yyvsp[-1].str));
            sym_insert_or_update((yyvsp[-3].str), "-", "array", "-", dims, "-", 0, current_nesting, yylineno);
            const_insert_or_bump((yyvsp[-1].str), "int", yylineno);
            free((yyvsp[-1].str));
            (yyval.str) = (yyvsp[-3].str);
        }
#line 1535 "parser.tab.c"
    break;

  case 29: /* declarator_array: IDENTIFIER OBRACKET error CBRACKET  */
#line 336 "parser.y"
        {
            /* malformed array size */
            fprintf(stderr, "Error at line %d: invalid array size in declaration of '%s'\n", yylineno, (yyvsp[-3].str));
            yyerrok;
            (yyval.str) = (yyvsp[-3].str);
        }
#line 1546 "parser.tab.c"
    break;

  case 30: /* declarator_func: IDENTIFIER OPAREN params_list CPAREN  */
#line 347 "parser.y"
        {
            char *params = (yyvsp[-1].str) ? (yyvsp[-1].str) : xstrdup("-");
            sym_insert_or_update((yyvsp[-3].str), "-", "function", "-", "-", params, 0, current_nesting, yylineno);
            free(params);
            (yyval.str) = (yyvsp[-3].str);  /* pass function name up (caller will free) */
        }
#line 1557 "parser.tab.c"
    break;

  case 31: /* declarator_func: IDENTIFIER OPAREN error CPAREN  */
#line 354 "parser.y"
        {
            fprintf(stderr, "Error at line %d: malformed parameter list for function '%s'\n", yylineno, (yyvsp[-3].str));
            yyerrok;
            (yyval.str) = (yyvsp[-3].str);
        }
#line 1567 "parser.tab.c"
    break;

  case 32: /* params_list: %empty  */
#line 363 "parser.y"
                          { (yyval.str) = NULL; }
#line 1573 "parser.tab.c"
    break;

  case 33: /* params_list: param_decl  */
#line 364 "parser.y"
                           { (yyval.str) = (yyvsp[0].str); }
#line 1579 "parser.tab.c"
    break;

  case 34: /* params_list: params_list COMMA param_decl  */
#line 366 "parser.y"
        {
            char *a = (yyvsp[-2].str);
            char *b = (yyvsp[0].str);
            if(!a) { (yyval.str) = b; }
            else {
                size_t n = strlen(a) + strlen(b) + 4;
                char *out = malloc(n);
                snprintf(out,n,"%s, %s", a, b);
                free(a); free(b);
                (yyval.str) = out;
            }
        }
#line 1596 "parser.tab.c"
    break;

  case 35: /* params_list: error  */
#line 379 "parser.y"
        {
            fprintf(stderr, "Error at line %d: invalid parameters in parameter list\n", yylineno);
            yyerrok;
            (yyval.str) = NULL;
        }
#line 1606 "parser.tab.c"
    break;

  case 36: /* param_decl: type_specifier IDENTIFIER  */
#line 388 "parser.y"
        {
            size_t n = strlen((yyvsp[-1].str)) + strlen((yyvsp[0].str)) + 4;
            char *s = malloc(n);
            snprintf(s,n,"%s %s",(yyvsp[-1].str),(yyvsp[0].str));
            free((yyvsp[-1].str)); free((yyvsp[0].str));
            (yyval.str) = s;
        }
#line 1618 "parser.tab.c"
    break;

  case 43: /* statement: error SEMICOLON  */
#line 409 "parser.y"
        {
            fprintf(stderr, "Error at line %d: invalid statement\n", yylineno);
            yyerrok;
        }
#line 1627 "parser.tab.c"
    break;

  case 44: /* local_decl: type_specifier declarator_var SEMICOLON  */
#line 418 "parser.y"
        {
            sym_insert_or_update((yyvsp[-1].str), (yyvsp[-2].str), "variable", "-", "-", "-", 0, current_nesting, yylineno);
            free((yyvsp[-2].str)); free((yyvsp[-1].str));
        }
#line 1636 "parser.tab.c"
    break;

  case 45: /* local_decl: type_specifier declarator_var ASSIGN expr SEMICOLON  */
#line 423 "parser.y"
        {
            sym_insert_or_update((yyvsp[-3].str), (yyvsp[-4].str), "variable", "-", "-", "-", 0, current_nesting, yylineno);
            free((yyvsp[-4].str)); free((yyvsp[-3].str));
        }
#line 1645 "parser.tab.c"
    break;

  case 46: /* local_decl: type_specifier declarator_array SEMICOLON  */
#line 428 "parser.y"
        {
            free((yyvsp[-2].str));
        }
#line 1653 "parser.tab.c"
    break;

  case 47: /* local_decl: type_specifier error SEMICOLON  */
#line 432 "parser.y"
        {
            fprintf(stderr, "Error at line %d: invalid local declaration\n", yylineno);
            yyerrok;
        }
#line 1662 "parser.tab.c"
    break;

  case 48: /* printf_stmt: PRINTF OPAREN STRING_LITERAL CPAREN SEMICOLON  */
#line 441 "parser.y"
        {
            const_insert_or_bump((yyvsp[-2].str), "string", yylineno);
            free((yyvsp[-2].str));
        }
#line 1671 "parser.tab.c"
    break;

  case 49: /* printf_stmt: PRINTF OPAREN STRING_LITERAL COMMA IDENTIFIER CPAREN SEMICOLON  */
#line 446 "parser.y"
        {
            const_insert_or_bump((yyvsp[-4].str), "string", yylineno);
            sym_add_reference((yyvsp[-2].str), yylineno);
            free((yyvsp[-4].str)); free((yyvsp[-2].str));
        }
#line 1681 "parser.tab.c"
    break;

  case 50: /* printf_stmt: PRINTF OPAREN error CPAREN SEMICOLON  */
#line 452 "parser.y"
        {
            fprintf(stderr, "Error at line %d: malformed printf arguments\n", yylineno);
            yyerrok;
        }
#line 1690 "parser.tab.c"
    break;

  case 51: /* return_stmt: RETURN NUMBER SEMICOLON  */
#line 461 "parser.y"
        {
            const_insert_or_bump((yyvsp[-1].str),"int", yylineno);
            free((yyvsp[-1].str));
        }
#line 1699 "parser.tab.c"
    break;

  case 52: /* return_stmt: RETURN IDENTIFIER SEMICOLON  */
#line 466 "parser.y"
        {
            sym_add_reference((yyvsp[-1].str), yylineno);
            free((yyvsp[-1].str));
        }
#line 1708 "parser.tab.c"
    break;

  case 53: /* return_stmt: RETURN error SEMICOLON  */
#line 471 "parser.y"
        {
            fprintf(stderr, "Error at line %d: invalid return value\n", yylineno);
            yyerrok;
        }
#line 1717 "parser.tab.c"
    break;

  case 55: /* expr_stmt: error SEMICOLON  */
#line 481 "parser.y"
        {
            fprintf(stderr, "Error at line %d: invalid expression\n", yylineno);
            yyerrok;
        }
#line 1726 "parser.tab.c"
    break;

  case 56: /* expr: IDENTIFIER ASSIGN expr  */
#line 489 "parser.y"
        {
            sym_add_reference((yyvsp[-2].str), yylineno);
            free((yyvsp[-2].str));
        }
#line 1735 "parser.tab.c"
    break;

  case 57: /* expr: NUMBER  */
#line 494 "parser.y"
        {
            const_insert_or_bump((yyvsp[0].str),"int", yylineno);
            free((yyvsp[0].str));
        }
#line 1744 "parser.tab.c"
    break;

  case 58: /* expr: IDENTIFIER  */
#line 499 "parser.y"
        {
            sym_add_reference((yyvsp[0].str), yylineno);
            free((yyvsp[0].str));
        }
#line 1753 "parser.tab.c"
    break;

  case 59: /* expr: error  */
#line 504 "parser.y"
        {
            fprintf(stderr, "Error at line %d: incomplete or invalid expression\n", yylineno);
            yyerrok;
        }
#line 1762 "parser.tab.c"
    break;


#line 1766 "parser.tab.c"

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

#line 510 "parser.y"


/* ---------------- Improved single-line yyerror ---------------- */
void yyerror(const char *msg) {
    extern char *yytext;
    if (yytext && *yytext)
        fprintf(stderr, "Syntax error at line %d near '%s': %s\n",
                yylineno, yytext, msg ? msg : "invalid syntax");
    else
        fprintf(stderr, "Syntax error at line %d: %s\n",
                yylineno, msg ? msg : "invalid syntax");
}

/* ---------------- main (unchanged except using improved yyerror) ---------------- */
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
