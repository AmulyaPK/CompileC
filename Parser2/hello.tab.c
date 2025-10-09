
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.4.1"

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

/* Line 189 of yacc.c  */
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

extern int yylex(void);
extern int yylineno;   /* maintained by lexer */
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

/* error helper */
void yyerror(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Parse error (line %d): ", yylineno);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}



/* Line 189 of yacc.c  */
#line 268 "hello.tab.c"

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

/* Line 214 of yacc.c  */
#line 197 "hello.y"

    int num;
    char *str;



/* Line 214 of yacc.c  */
#line 356 "hello.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 368 "hello.tab.c"

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
# if YYENABLE_NLS
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  23
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   308

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  132
/* YYNRULES -- Number of states.  */
#define YYNSTATES  224

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    14,    16,    18,
      20,    24,    27,    29,    32,    34,    36,    38,    40,    42,
      48,    54,    56,    57,    59,    61,    64,    68,    70,    74,
      76,    80,    82,    86,    88,    92,    95,    97,    99,   102,
     104,   109,   114,   118,   120,   124,   126,   130,   133,   136,
     138,   141,   143,   147,   152,   154,   158,   160,   164,   168,
     170,   172,   174,   176,   178,   180,   182,   186,   190,   191,
     194,   196,   198,   201,   202,   204,   206,   210,   212,   216,
     218,   224,   226,   230,   232,   236,   238,   242,   244,   248,
     250,   254,   256,   260,   262,   266,   268,   273,   275,   278,
     281,   283,   288,   293,   296,   298,   300,   302,   304,   306,
     310,   311,   313,   315,   319,   322,   324,   327,   329,   332,
     335,   338,   339,   341,   347,   355,   361,   369,   375,   385,
     389,   392,   395
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      48,     0,    -1,    49,    -1,    50,    -1,    49,    50,    -1,
      74,    -1,    52,    -1,    51,    -1,     8,    -1,     9,    -1,
      53,    62,    35,    -1,    53,    35,    -1,    56,    -1,    56,
      53,    -1,    54,    -1,    55,    -1,    14,    -1,    14,    -1,
      14,    -1,    18,    57,    29,    58,    30,    -1,    17,    57,
      29,    58,    30,    -1,     3,    -1,    -1,     3,    -1,    59,
      -1,    58,    59,    -1,    53,    60,    35,    -1,    61,    -1,
      60,    36,    61,    -1,    64,    -1,    64,    37,   105,    -1,
      63,    -1,    62,    36,    63,    -1,    64,    -1,    64,    41,
      72,    -1,    65,    66,    -1,    66,    -1,    46,    -1,    46,
      65,    -1,     3,    -1,    66,    33,    28,    34,    -1,    66,
      31,    67,    32,    -1,    31,    64,    32,    -1,    68,    -1,
      68,    36,    26,    -1,    69,    -1,    68,    36,    69,    -1,
      53,    64,    -1,    53,    70,    -1,    65,    -1,    65,    71,
      -1,    71,    -1,    31,    70,    32,    -1,    71,    33,    28,
      34,    -1,    83,    -1,    29,    73,    30,    -1,    72,    -1,
      73,    36,    72,    -1,    53,    64,    77,    -1,    76,    -1,
      77,    -1,    80,    -1,   102,    -1,   103,    -1,   104,    -1,
     101,    -1,     3,    37,    75,    -1,    29,    78,    30,    -1,
      -1,    78,    79,    -1,    52,    -1,    75,    -1,    81,    35,
      -1,    -1,    82,    -1,    83,    -1,    82,    36,    83,    -1,
      84,    -1,    93,    41,    83,    -1,    85,    -1,    85,    39,
      82,    37,    84,    -1,    86,    -1,    85,    44,    86,    -1,
      87,    -1,    86,    44,    87,    -1,    88,    -1,    87,    43,
      88,    -1,    89,    -1,    88,    43,    89,    -1,    90,    -1,
      89,    40,    90,    -1,    91,    -1,    90,    40,    91,    -1,
      92,    -1,    91,    40,    92,    -1,    93,    -1,    31,    98,
      32,    92,    -1,    94,    -1,    45,    93,    -1,    40,    93,
      -1,    95,    -1,    94,    33,    82,    34,    -1,    94,    31,
      96,    32,    -1,    94,    45,    -1,     3,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,    31,    82,    32,    -1,
      -1,    97,    -1,    83,    -1,    97,    36,    83,    -1,    99,
     100,    -1,    56,    -1,    56,    99,    -1,    14,    -1,    18,
       3,    -1,    17,     3,    -1,    23,     3,    -1,    -1,    70,
      -1,    19,    31,     6,    32,    35,    -1,    19,    31,     6,
      36,     3,    32,    35,    -1,    16,    31,    82,    32,    75,
      -1,    16,    31,    82,    32,    75,    27,    75,    -1,    24,
      31,    82,    32,    75,    -1,    15,    31,    81,    35,    81,
      35,    81,    32,    75,    -1,    22,    81,    35,    -1,    25,
      35,    -1,    21,    35,    -1,    84,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   225,   225,   229,   230,   234,   235,   236,   240,   241,
     246,   250,   254,   255,   256,   257,   263,   267,   272,   273,
     277,   278,   282,   284,   289,   290,   294,   298,   299,   303,
     304,   309,   310,   314,   315,   323,   324,   328,   329,   334,
     341,   345,   349,   354,   355,   359,   360,   364,   368,   373,
     374,   375,   380,   381,   386,   387,   391,   392,   397,   406,
     407,   408,   409,   410,   411,   412,   416,   420,   423,   425,
     429,   430,   435,   438,   440,   444,   445,   449,   450,   454,
     455,   459,   460,   464,   465,   469,   470,   474,   475,   479,
     480,   484,   485,   489,   490,   494,   495,   499,   500,   501,
     505,   506,   507,   508,   512,   513,   514,   515,   516,   517,
     521,   523,   527,   528,   533,   538,   539,   544,   545,   546,
     547,   551,   553,   558,   559,   564,   565,   570,   571,   576,
     577,   578,   583
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "INT_CONST", "FLOAT_CONST",
  "STRING_LITERAL", "CHAR_CONST", "INCLUDE", "PREPROCESSOR", "HEADER",
  "UNMATCHEDCHAR", "UNMATCHEDSTRING", "UNMATCHEDCOMMENT", "KEYWORD", "FOR",
  "IF", "UNION", "STRUCT", "PRINTF", "const_expression_opt", "CONTINUE",
  "RETURN", "ENUM", "WHILE", "BREAK", "ELLIPSIS", "ELSE",
  "constant_expression_opt", "LBRACE", "RBRACE", "LPAREN", "RPAREN",
  "LBRACK", "RBRACK", "SEMI", "COMMA", "COLON", "DOT", "QMARK", "OP",
  "ASSIGN", "OP_ASSIGN", "RELOP", "LOGIC_OP", "INCDEC", "'*'", "$accept",
  "translation_unit", "external_declaration_list", "external_declaration",
  "preproc_directive", "declaration", "declaration_specifiers",
  "storage_class_specifier", "type_qualifier", "type_specifier",
  "declarator_opt", "struct_declaration_list", "struct_declaration",
  "struct_declarator_list", "struct_declarator", "init_declarator_list",
  "init_declarator", "declarator", "pointer", "direct_declarator",
  "parameter_type_list", "parameter_list", "parameter_declaration",
  "abstract_declarator", "direct_abstract_declarator", "initializer",
  "initializer_list", "function_definition", "statement",
  "labeled_statement", "compound_statement", "block_item_list",
  "block_item", "expression_statement", "expression_opt", "expression",
  "assignment_expression", "conditional_expression",
  "logical_or_expression", "logical_and_expression", "equality_expression",
  "relational_expression", "shift_expression", "additive_expression",
  "multiplicative_expression", "cast_expression", "unary_expression",
  "postfix_expression", "primary_expression",
  "argument_expression_list_opt", "argument_expression_list", "type_name",
  "specifier_qualifier_list", "abstract_declarator_opt", "io_statement",
  "selection_statement", "iteration_statement", "jump_statement",
  "constant_expression", 0
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
     295,   296,   297,   298,   299,   300,    42
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    49,    49,    50,    50,    50,    51,    51,
      52,    52,    53,    53,    53,    53,    54,    55,    56,    56,
      56,    56,    57,    57,    58,    58,    59,    60,    60,    61,
      61,    62,    62,    63,    63,    64,    64,    65,    65,    66,
      66,    66,    66,    67,    67,    68,    68,    69,    69,    70,
      70,    70,    71,    71,    72,    72,    73,    73,    74,    75,
      75,    75,    75,    75,    75,    75,    76,    77,    78,    78,
      79,    79,    80,    81,    81,    82,    82,    83,    83,    84,
      84,    85,    85,    86,    86,    87,    87,    88,    88,    89,
      89,    90,    90,    91,    91,    92,    92,    93,    93,    93,
      94,    94,    94,    94,    95,    95,    95,    95,    95,    95,
      96,    96,    97,    97,    98,    99,    99,    56,    56,    56,
      56,   100,   100,   101,   101,   102,   102,   103,   103,   104,
     104,   104,   105
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       3,     2,     1,     2,     1,     1,     1,     1,     1,     5,
       5,     1,     0,     1,     1,     2,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     2,     1,     1,     2,     1,
       4,     4,     3,     1,     3,     1,     3,     2,     2,     1,
       2,     1,     3,     4,     1,     3,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     0,     2,
       1,     1,     2,     0,     1,     1,     3,     1,     3,     1,
       5,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     4,     1,     2,     2,
       1,     4,     4,     2,     1,     1,     1,     1,     1,     3,
       0,     1,     1,     3,     2,     1,     2,     1,     2,     2,
       2,     0,     1,     5,     7,     5,     7,     5,     9,     3,
       2,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    21,     8,     9,    16,    22,    22,     0,     0,     2,
       3,     7,     6,     0,    14,    15,    12,     5,   119,     0,
     118,     0,   120,     1,     4,    39,     0,    11,    37,     0,
      31,    33,     0,    36,    13,     0,     0,     0,    38,    10,
       0,    68,     0,    58,    35,     0,     0,     0,     0,    24,
       0,    42,    32,    33,    73,   104,   105,   106,   107,   108,
       0,     0,     0,     0,    34,    54,    77,    79,    81,    83,
      85,    87,    89,    91,    93,    95,    97,   100,     0,     0,
      43,    45,     0,     0,    27,    29,    20,    25,    19,    21,
       0,     0,     0,     0,    73,     0,     0,    67,    70,     0,
      71,    59,    60,    69,    61,     0,    74,    75,    65,    62,
      63,    64,    56,     0,    21,    18,   115,     0,     0,   121,
       0,    99,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,     0,   103,     0,    47,    49,    48,    51,
      41,     0,    40,    26,     0,     0,    73,    73,     0,     0,
     131,     0,     0,   130,    72,     0,    55,     0,   116,   109,
       0,     0,    49,   122,   114,     0,    82,    95,    84,    86,
      88,    90,    92,    94,    78,   112,     0,   111,     0,     0,
      50,     0,    44,    46,    28,   132,    30,   104,    66,     0,
       0,     0,   129,     0,    76,    57,    96,     0,   102,     0,
     101,    52,     0,    73,    73,     0,     0,    73,    80,   113,
      53,     0,   125,   123,     0,   127,    73,    73,     0,     0,
     126,   124,    73,   128
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,     9,    10,    11,    12,    47,    14,    15,    16,
      19,    48,    49,    83,    84,    29,    30,    37,    32,    33,
      79,    80,    81,   179,   139,    64,   113,    17,   100,   101,
     102,    54,   103,   104,   105,   106,   107,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,   176,
     177,   118,   119,   164,   108,   109,   110,   111,   186
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -140
static const yytype_int16 yypact[] =
{
     251,  -140,  -140,  -140,   154,    24,    27,    44,    53,   251,
    -140,  -140,  -140,     8,  -140,  -140,    69,  -140,    72,    75,
      72,    78,  -140,  -140,  -140,  -140,    10,  -140,    15,    93,
    -140,    34,    19,    46,  -140,    69,    69,    56,  -140,  -140,
      10,  -140,   210,  -140,    46,    69,   110,    10,    94,  -140,
     234,  -140,  -140,    80,   129,  -140,  -140,  -140,  -140,  -140,
     210,   189,   218,   218,  -140,  -140,  -140,    -8,    86,    98,
     106,   130,   133,   138,  -140,   139,     7,  -140,    13,   156,
     150,  -140,   163,   120,  -140,   153,  -140,  -140,  -140,   249,
     167,   168,   170,   169,   239,   171,   173,  -140,  -140,     8,
    -140,  -140,  -140,  -140,  -140,   174,   175,  -140,  -140,  -140,
    -140,  -140,  -140,    -4,   262,  -140,   102,    74,   178,     2,
     239,  -140,  -140,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,  -140,    13,  -140,    20,  -140,   185,
    -140,    11,  -140,  -140,    10,   239,   160,   239,   239,   213,
    -140,   191,   239,  -140,  -140,   239,  -140,   210,  -140,  -140,
     239,     2,   196,  -140,  -140,   125,  -140,  -140,    98,  -140,
     130,  -140,  -140,  -140,  -140,  -140,   198,   192,    59,   199,
     185,   204,  -140,  -140,  -140,  -140,  -140,   201,  -140,   200,
      90,    91,  -140,   151,  -140,  -140,  -140,   239,  -140,   239,
    -140,  -140,   202,   239,   160,   205,   230,   160,  -140,  -140,
    -140,   212,   226,  -140,   224,  -140,   239,   160,   227,   229,
    -140,  -140,   160,  -140
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -140,  -140,  -140,   257,  -140,   217,     1,  -140,  -140,   -40,
     261,   236,    32,  -140,   131,  -140,   233,    -5,   -21,   -28,
    -140,  -140,   135,   -59,  -113,   -55,  -140,  -140,  -126,  -140,
     246,  -140,  -140,  -140,   -85,   -49,   -42,  -139,  -140,   157,
     155,   152,   164,   159,   179,  -115,   -60,  -140,  -140,  -140,
    -140,  -140,   180,  -140,  -140,  -140,  -140,  -140,  -140
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -105
static const yytype_int16 yytable[] =
{
      65,    13,   121,   122,    44,   112,   185,    38,    31,   151,
      13,    25,   117,    25,     1,   173,    25,    34,    65,   138,
     188,   116,    25,    25,   180,     4,   156,    18,     5,     6,
      20,   123,   157,   161,     7,    53,   124,   182,   132,    26,
     133,    26,    85,    27,   135,   196,    78,    22,    28,   180,
      26,   135,   134,    23,    28,    99,    28,   137,   208,    28,
     163,    28,   189,    41,   167,   167,   167,   167,   167,   167,
     167,   117,     1,   136,   165,    42,   116,    45,   212,    46,
      87,   215,    87,     4,   178,   167,     5,     6,    51,   174,
     175,   220,     7,   200,    53,   155,   223,     1,   162,   190,
     167,   -23,   195,   193,    35,     1,   159,    36,     4,    44,
     155,     5,     6,   194,   137,    65,   115,     7,   211,     5,
       6,    42,   204,   205,    86,     7,   155,   206,    39,    40,
     125,   219,    89,    56,    57,    58,    59,   167,    82,    85,
     162,   126,    78,     4,    90,    91,     5,     6,    92,   127,
      93,    94,     7,    95,    96,   143,   144,   209,    41,    97,
      61,   155,   197,   187,    56,    57,    58,    59,   -18,    62,
     128,   -18,   -18,   129,    63,    90,    91,   -18,   130,    92,
     131,    93,    94,   207,    95,    96,   141,   155,   140,    41,
     145,    61,   114,    56,    57,    58,    59,   142,   147,   148,
      62,   149,   152,   115,   150,    63,     5,     6,   153,   154,
     160,   155,     7,    55,    56,    57,    58,    59,   181,   191,
      61,    55,    56,    57,    58,    59,   192,   161,   199,    62,
     198,   201,   202,   214,    63,   203,   210,     1,   146,    60,
     213,    61,    55,    56,    57,    58,    59,   216,     4,   120,
      62,     5,     6,   217,     1,    63,   218,     7,    62,     2,
       3,   222,   221,    63,    88,     4,    24,    21,     5,     6,
      61,    98,    50,    52,     7,   184,   183,    43,   169,    62,
     168,   166,  -104,     0,    63,  -104,   146,   171,  -104,  -104,
    -104,   170,  -104,  -104,  -104,  -104,   158,     0,  -104,     0,
       0,  -104,  -104,  -104,     0,  -104,  -104,  -104,   172
};

static const yytype_int16 yycheck[] =
{
      42,     0,    62,    63,    32,    60,   145,    28,    13,    94,
       9,     3,    61,     3,     3,   130,     3,    16,    60,    78,
     146,    61,     3,     3,   137,    14,    30,     3,    17,    18,
       3,    39,    36,    31,    23,    40,    44,    26,    31,    31,
      33,    31,    47,    35,    31,   160,    45,     3,    46,   162,
      31,    31,    45,     0,    46,    54,    46,    78,   197,    46,
     119,    46,   147,    29,   124,   125,   126,   127,   128,   129,
     130,   120,     3,    78,   123,    41,   116,    31,   204,    33,
      48,   207,    50,    14,   133,   145,    17,    18,    32,   131,
     132,   217,    23,    34,    99,    36,   222,     3,   119,   148,
     160,    29,   157,   152,    29,     3,    32,    29,    14,   137,
      36,    17,    18,   155,   135,   157,    14,    23,   203,    17,
      18,    41,    32,    32,    30,    23,    36,    36,    35,    36,
      44,   216,     3,     4,     5,     6,     7,   197,    28,   144,
     161,    43,   141,    14,    15,    16,    17,    18,    19,    43,
      21,    22,    23,    24,    25,    35,    36,   199,    29,    30,
      31,    36,    37,     3,     4,     5,     6,     7,    14,    40,
      40,    17,    18,    40,    45,    15,    16,    23,    40,    19,
      41,    21,    22,    32,    24,    25,    36,    36,    32,    29,
      37,    31,     3,     4,     5,     6,     7,    34,    31,    31,
      40,    31,    31,    14,    35,    45,    17,    18,    35,    35,
      32,    36,    23,     3,     4,     5,     6,     7,    33,     6,
      31,     3,     4,     5,     6,     7,    35,    31,    36,    40,
      32,    32,    28,     3,    45,    35,    34,     3,    37,    29,
      35,    31,     3,     4,     5,     6,     7,    35,    14,    31,
      40,    17,    18,    27,     3,    45,    32,    23,    40,     8,
       9,    32,    35,    45,    30,    14,     9,     6,    17,    18,
      31,    54,    36,    40,    23,   144,   141,    31,   126,    40,
     125,   124,    33,    -1,    45,    36,    37,   128,    39,    40,
      41,   127,    43,    44,    45,    33,   116,    -1,    36,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    45,   129
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,     9,    14,    17,    18,    23,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    74,     3,    57,
       3,    57,     3,     0,    50,     3,    31,    35,    46,    62,
      63,    64,    65,    66,    53,    29,    29,    64,    65,    35,
      36,    29,    41,    77,    66,    31,    33,    53,    58,    59,
      58,    32,    63,    64,    78,     3,     4,     5,     6,     7,
      29,    31,    40,    45,    72,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    53,    67,
      68,    69,    28,    60,    61,    64,    30,    59,    30,     3,
      15,    16,    19,    21,    22,    24,    25,    30,    52,    53,
      75,    76,    77,    79,    80,    81,    82,    83,   101,   102,
     103,   104,    72,    73,     3,    14,    56,    82,    98,    99,
      31,    93,    93,    39,    44,    44,    43,    43,    40,    40,
      40,    41,    31,    33,    45,    31,    64,    65,    70,    71,
      32,    36,    34,    35,    36,    37,    37,    31,    31,    31,
      35,    81,    31,    35,    35,    36,    30,    36,    99,    32,
      32,    31,    65,    70,   100,    82,    86,    93,    87,    88,
      89,    90,    91,    92,    83,    83,    96,    97,    82,    70,
      71,    33,    26,    69,    61,    84,   105,     3,    75,    81,
      82,     6,    35,    82,    83,    72,    92,    37,    32,    36,
      34,    32,    28,    35,    32,    32,    36,    32,    84,    83,
      34,    81,    75,    35,     3,    75,    35,    27,    32,    81,
      75,    35,    32,    75
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
  if (yyn == YYPACT_NINF)
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
        case 8:

/* Line 1455 of yacc.c  */
#line 240 "hello.y"
    { /* include handled in lexer; nothing to do here */ ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 241 "hello.y"
    { /* raw preprocessor line */ ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 247 "hello.y"
    {
            /* each init_declarator handled in its action */
        ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 274 "hello.y"
    {
            /* struct type declared; not fully expanded here */
        ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 316 "hello.y"
    {
            /* initializer may contain constants/expressions; declarator action already recorded symbol */
        ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 335 "hello.y"
    {
            /* plain identifier: actual insertion must be done in parent rule where type present */
            /* we just pass the identifier string using $$ */
            /* in actions above we will combine the type and identifier to fill symbol table */
            /* store lexeme pointer in parser stack */
        ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 342 "hello.y"
    {
            /* array declarator; constant_expression_opt may provide dimension */
        ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 346 "hello.y"
    {
            /* function declarator with prototyped params */
        ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 365 "hello.y"
    {
            /* add param to parameter list for function symbol when assembling the function - simplified below */
        ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 398 "hello.y"
    {
            /* This is where we take the declarator and the type and insert a symbol with proc_def_flag=1 */
            /* To keep code modular, concrete insertion occurs in the declarator actions below when we have type + id */
        ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 512 "hello.y"
    { sym_add_reference(yylex, yylineno); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 513 "hello.y"
    { const_insert_or_bump(yylex, "int", yylineno); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 514 "hello.y"
    { const_insert_or_bump(yylex, "float", yylineno); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 515 "hello.y"
    { const_insert_or_bump(yylex, "string", yylineno); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 516 "hello.y"
    { const_insert_or_bump(yylex, "char", yylineno); ;}
    break;



/* Line 1455 of yacc.c  */
#line 1932 "hello.tab.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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
      if (yyn != YYPACT_NINF)
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
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



/* Line 1675 of yacc.c  */
#line 593 "hello.y"


/* ---------------- epilogue ---------------- */

int main(int argc, char **argv){
    if(argc < 2){ fprintf(stderr,"Usage: %s <source.c>\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1],"r");
    if(!f){ perror("fopen"); return 1; }
    FILE *yyin = f;
    if(yyparse() == 0){
        printf("Parsing succeeded.\n");
        print_symbol_table();
        print_const_table();
    } else {
        fprintf(stderr,"Parsing failed.\n");
    }
    fclose(f);
    return 0;
}

/* helper wrappers for printing (expose the static print functions above) */
// void print_symbol_table(void){ print_symbol_table(); }     /* alias to static function above */
// void print_const_table(void){ print_const_table(); }       /* alias to static function above */

