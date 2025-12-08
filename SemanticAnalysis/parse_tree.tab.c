
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
#line 1 "parse_tree.y"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void yyerror (const char *s);
int yylex ();
int yywrap ();
void add (char c);
void insert_type ();
int search (char *);
void print_tree (struct node** nd) {
    printf("Inorder traversal of the parse tree: \n");
    print_inorder(&nd);
    printf("\n\n");
    return;
}


void print_inorder (struct node** nd) {
    if (!&nd) return;
    if ((&nd).left) print_inorder((*nd)->left);
    if (*nd->token) printf("%s, ", *nd->token);
    if (*nd->right) print_inorder(*nd->right);
    return;
}
struct node* make_node (struct node* left, struct node* right, char *token);

struct node {
    struct node* left;
    struct node* right;
    char* token;
};

struct symbol {
    char* id_name;
    char* data_type;
    char* type;
    int line_no;
} symbol_table [100];

int ind = 0;
int q;
char type[20];
extern int count;
struct node* head;



/* Line 189 of yacc.c  */
#line 124 "parse_tree.tab.c"

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
     VOID = 258,
     INT = 259,
     FLOAT = 260,
     CHAR = 261,
     IF = 262,
     ELSE = 263,
     FOR = 264,
     WHILE = 265,
     RETURN = 266,
     SWITCH = 267,
     CASE = 268,
     DEFAULT = 269,
     BREAK = 270,
     STRUCT = 271,
     UNION = 272,
     PRINTF = 273,
     INCLUDE = 274,
     TRUE = 275,
     FALSE = 276,
     UNARY = 277,
     NUMBER = 278,
     IDENTIFIER = 279,
     STRING_LITERAL = 280,
     CHAR_LITERAL = 281,
     EQ = 282,
     NEQ = 283,
     LE = 284,
     GE = 285,
     LT = 286,
     GT = 287,
     PLUS = 288,
     MINUS = 289,
     MULT = 290,
     DIVIDE = 291,
     ASSIGN = 292,
     OPAREN = 293,
     CPAREN = 294,
     OCURLY = 295,
     CCURLY = 296,
     SEMICOLON = 297,
     COMMA = 298,
     OBRACKET = 299,
     CBRACKET = 300,
     COLON = 301,
     UNMATCHED_COMMENT = 302,
     PREPROCESSOR = 303,
     DO = 304
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 51 "parse_tree.y"

    struct var_name {
        char name[100];
        struct node* nd;
    } nd_obj;



/* Line 214 of yacc.c  */
#line 218 "parse_tree.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 230 "parse_tree.tab.c"

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   271

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNRULES -- Number of states.  */
#define YYNSTATES  213

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

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
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    12,    13,    16,    17,    19,
      21,    23,    25,    32,    39,    43,    46,    47,    52,    57,
      60,    63,    64,    68,    72,    74,    78,    80,    83,    85,
      88,    92,    94,    95,    97,   101,   103,   105,   107,   109,
     111,   116,   121,   126,   127,   129,   131,   133,   137,   140,
     141,   144,   146,   148,   150,   152,   154,   156,   158,   160,
     162,   164,   166,   168,   171,   179,   191,   199,   209,   220,
     228,   229,   232,   235,   237,   240,   244,   247,   250,   252,
     254,   256,   257,   259,   265,   273,   279,   283,   287,   291,
     293,   296,   299,   301,   303,   307,   311,   315,   319,   323,
     327,   329,   333,   337,   339,   343,   347,   349,   352,   354,
     356,   358,   360,   362
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      51,     0,    -1,    52,    53,    -1,    52,    19,    -1,    52,
      48,    -1,    -1,    53,    54,    -1,    -1,    59,    -1,    57,
      -1,    55,    -1,    56,    -1,    16,    24,    40,    61,    41,
      42,    -1,    17,    24,    40,    61,    41,    42,    -1,    68,
      63,    42,    -1,    68,    42,    -1,    -1,    68,    71,    58,
      60,    -1,    40,    61,    75,    41,    -1,     1,    41,    -1,
      61,    62,    -1,    -1,    68,    63,    42,    -1,    68,     1,
      42,    -1,    64,    -1,    63,    43,    64,    -1,    69,    -1,
      69,    92,    -1,    70,    -1,    70,    65,    -1,    40,    66,
      41,    -1,    67,    -1,    -1,    92,    -1,    67,    43,    92,
      -1,     4,    -1,     5,    -1,     6,    -1,     3,    -1,    24,
      -1,    24,    44,    23,    45,    -1,    24,    44,     1,    45,
      -1,    24,    38,    72,    39,    -1,    -1,     3,    -1,    73,
      -1,    74,    -1,    73,    43,    74,    -1,    68,    24,    -1,
      -1,    75,    76,    -1,    91,    -1,    89,    -1,    90,    -1,
      62,    -1,    77,    -1,    78,    -1,    79,    -1,    80,    -1,
      81,    -1,    86,    -1,    60,    -1,    42,    -1,     1,    42,
      -1,     7,    38,    92,    39,    40,    76,    41,    -1,     7,
      38,    92,    39,    40,    76,    41,     8,    40,    76,    41,
      -1,    10,    38,    92,    39,    40,    76,    41,    -1,    49,
      40,    76,    41,    10,    38,    92,    39,    42,    -1,     9,
      38,    87,    88,    42,    88,    39,    40,    76,    41,    -1,
      12,    38,    92,    39,    40,    82,    41,    -1,    -1,    82,
      83,    -1,    84,    75,    -1,    85,    -1,    84,    85,    -1,
      13,    97,    46,    -1,    14,    46,    -1,    15,    42,    -1,
      91,    -1,    62,    -1,    42,    -1,    -1,    92,    -1,    18,
      38,    25,    39,    42,    -1,    18,    38,    25,    43,    24,
      39,    42,    -1,    18,    38,     1,    39,    42,    -1,    11,
      23,    42,    -1,    11,    24,    42,    -1,    11,     1,    42,
      -1,    92,    -1,     1,    42,    -1,    24,    92,    -1,    93,
      -1,    94,    -1,    93,    31,    94,    -1,    93,    32,    94,
      -1,    93,    29,    94,    -1,    93,    30,    94,    -1,    93,
      27,    94,    -1,    93,    28,    94,    -1,    95,    -1,    94,
      33,    95,    -1,    94,    34,    95,    -1,    96,    -1,    95,
      35,    96,    -1,    95,    36,    96,    -1,    97,    -1,    24,
      22,    -1,    24,    -1,    23,    -1,     5,    -1,    25,    -1,
      26,    -1,    38,    92,    39,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    66,    66,    70,    71,    72,    76,    77,    81,    82,
      83,    84,    88,    96,   104,   106,   111,   115,   122,   124,
     129,   130,   134,   136,   141,   142,   146,   147,   148,   149,
     153,   157,   158,   162,   163,   167,   168,   169,   170,   174,
     178,   180,   185,   190,   191,   192,   196,   197,   201,   205,
     206,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   226,   228,   233,   238,   243,   248,
     253,   254,   258,   262,   263,   267,   268,   272,   276,   277,
     278,   282,   283,   287,   289,   291,   296,   297,   298,   302,
     303,   307,   308,   312,   313,   314,   315,   316,   317,   318,
     322,   323,   324,   328,   329,   330,   334,   335,   339,   340,
     341,   342,   343,   344
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "VOID", "INT", "FLOAT", "CHAR", "IF",
  "ELSE", "FOR", "WHILE", "RETURN", "SWITCH", "CASE", "DEFAULT", "BREAK",
  "STRUCT", "UNION", "PRINTF", "INCLUDE", "TRUE", "FALSE", "UNARY",
  "NUMBER", "IDENTIFIER", "STRING_LITERAL", "CHAR_LITERAL", "EQ", "NEQ",
  "LE", "GE", "LT", "GT", "PLUS", "MINUS", "MULT", "DIVIDE", "ASSIGN",
  "OPAREN", "CPAREN", "OCURLY", "CCURLY", "SEMICOLON", "COMMA", "OBRACKET",
  "CBRACKET", "COLON", "UNMATCHED_COMMENT", "PREPROCESSOR", "DO",
  "$accept", "program", "preamble", "external_defs", "external_def",
  "struct_definition", "union_definition", "global_decl", "save_params",
  "function_definition", "block", "decl_list", "decl", "declarator_list",
  "declarator", "initializer_list", "optional_expression_list",
  "expression_list", "type_specifier", "declarator_var",
  "declarator_array", "declarator_func", "params_list", "param_decls",
  "param_decl", "stmt_list", "statement", "if_stmt", "while_stmt",
  "do_while_stmt", "for_stmt", "switch_stmt", "case_list", "case_block",
  "case_labels", "case_label", "break_stmt", "for_init_stmt",
  "optional_expr", "printf_stmt", "return_stmt", "expr_stmt", "expr",
  "cond_expr", "add_expr", "mult_expr", "postfix_expr", "primary_expr", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    52,    52,    52,    53,    53,    54,    54,
      54,    54,    55,    56,    57,    57,    58,    59,    60,    60,
      61,    61,    62,    62,    63,    63,    64,    64,    64,    64,
      65,    66,    66,    67,    67,    68,    68,    68,    68,    69,
      70,    70,    71,    72,    72,    72,    73,    73,    74,    75,
      75,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    77,    77,    78,    79,    80,    81,
      82,    82,    83,    84,    84,    85,    85,    86,    87,    87,
      87,    88,    88,    89,    89,    89,    90,    90,    90,    91,
      91,    92,    92,    93,    93,    93,    93,    93,    93,    93,
      94,    94,    94,    95,    95,    95,    96,    96,    97,    97,
      97,    97,    97,    97
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     0,     2,     0,     1,     1,
       1,     1,     6,     6,     3,     2,     0,     4,     4,     2,
       2,     0,     3,     3,     1,     3,     1,     2,     1,     2,
       3,     1,     0,     1,     3,     1,     1,     1,     1,     1,
       4,     4,     4,     0,     1,     1,     1,     3,     2,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     7,    11,     7,     9,    10,     7,
       0,     2,     2,     1,     2,     3,     2,     2,     1,     1,
       1,     0,     1,     5,     7,     5,     3,     3,     3,     1,
       2,     2,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     2,     1,     1,
       1,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     7,     1,     3,     4,     2,    38,    35,    36,
      37,     0,     0,     6,    10,    11,     9,     8,     0,     0,
       0,    39,    15,     0,    24,    26,    28,    16,    21,    21,
      43,     0,    14,     0,   110,   109,   108,   111,   112,     0,
      27,    92,    93,   100,   103,   106,    32,    29,     0,     0,
       0,    38,     0,     0,    45,    46,     0,     0,    39,    25,
     107,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    31,    33,     0,    21,    17,     0,
      20,     0,     0,    48,    42,     0,    41,    40,   113,   108,
      98,    99,    96,    97,    94,    95,   101,   102,   104,   105,
      30,     0,    19,    49,    12,     0,     0,    13,    47,    34,
       0,    23,    22,     0,   110,     0,     0,     0,     0,     0,
       0,     0,    18,    62,     0,    61,    54,    50,    55,    56,
      57,    58,    59,    60,    52,    53,    51,    89,    63,     0,
       0,     0,     0,     0,     0,     0,    77,     0,     0,     0,
       0,    80,    79,    81,    78,     0,    88,    86,    87,     0,
       0,     0,     0,     0,    90,     0,    82,     0,     0,     0,
       0,     0,     0,     0,    81,     0,    70,    85,    83,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,    66,
       0,     0,    69,    71,    49,    73,    84,     0,     0,     0,
     108,     0,    76,     0,    74,     0,     0,     0,    75,    67,
       0,    68,    65
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     6,    13,    14,    15,    16,    48,    17,
     125,    49,   126,    23,    24,    47,    73,    74,    81,    25,
      26,    27,    53,    54,    55,   110,   127,   128,   129,   130,
     131,   132,   184,   193,   194,   195,   133,   153,   165,   134,
     135,   136,   137,    41,    42,    43,    44,    45
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -144
static const yytype_int16 yypact[] =
{
    -144,    19,   -11,  -144,  -144,  -144,    12,  -144,  -144,  -144,
    -144,    -2,    33,  -144,  -144,  -144,  -144,  -144,    28,    22,
      29,   -18,  -144,    58,  -144,   171,    32,  -144,  -144,  -144,
      75,    48,  -144,    40,  -144,  -144,   155,  -144,  -144,   171,
    -144,   205,    71,    76,  -144,  -144,   171,  -144,     3,    62,
     149,    44,    50,    47,    63,  -144,    68,    70,    73,  -144,
    -144,  -144,    82,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,    86,    80,  -144,    94,  -144,  -144,    99,
    -144,     9,   108,  -144,  -144,   126,  -144,  -144,  -144,   141,
      71,    71,    71,    71,    71,    71,    76,    76,  -144,  -144,
    -144,   171,  -144,   126,  -144,   130,   104,  -144,  -144,  -144,
     133,  -144,  -144,    77,    24,   145,   153,   164,    11,   166,
     163,   168,  -144,  -144,   128,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,   171,
     161,   171,   165,   172,   173,   171,  -144,     6,    35,   169,
     178,  -144,  -144,   171,  -144,   182,  -144,  -144,  -144,   183,
     184,   -30,   185,   188,  -144,   202,  -144,   189,   190,   203,
     204,   200,   217,    35,   171,    35,  -144,  -144,  -144,   208,
     210,   209,   212,   211,    10,   207,   171,   245,   214,  -144,
     193,   213,  -144,  -144,   156,  -144,  -144,   216,   218,    35,
    -144,   215,  -144,    84,  -144,   220,    35,   219,  -144,  -144,
     222,  -144,  -144
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
     221,   -23,   -48,   176,   223,  -144,  -144,  -144,    -3,  -144,
    -144,  -144,  -144,  -144,   179,    72,  -143,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,    74,  -144,  -144,    91,  -144,
    -144,   127,   -25,  -144,   175,   119,   129,    81
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -73
static const yytype_int16 yytable[] =
{
      40,    80,    80,    18,    76,   162,    50,   160,     4,   170,
     105,    61,   142,   171,    62,     7,     8,     9,    10,     3,
      30,    75,    19,   190,   191,   -36,    31,    52,    11,    12,
     181,   161,   183,    58,   143,   144,   113,     5,     7,     8,
     114,    10,   115,    77,   116,   117,   118,   119,   -36,    56,
     120,   192,    21,   121,   103,    80,   207,    20,    35,    36,
      37,    38,    28,   210,    58,     7,     8,     9,    10,    29,
      22,    57,    46,    39,    83,    77,   109,   123,    51,     8,
       9,    10,    52,   -44,   124,   113,    84,     7,     8,   114,
      10,   115,   152,   116,   117,   118,   119,   -72,   -72,   120,
      32,    33,   121,    79,    69,    70,    85,    35,    36,    37,
      38,    71,    72,    86,   149,    87,   155,    31,   102,   138,
     159,    88,    39,   101,    77,   -72,   123,   100,   166,     7,
       8,     9,    10,   124,   113,   102,     7,     8,   114,    10,
     115,   104,   116,   117,   118,   119,   112,    33,   120,   166,
     107,   121,     7,     8,     9,    10,    35,    36,    37,    38,
      34,   197,   150,    60,     7,     8,   114,    10,   148,   190,
     191,    39,   111,    77,   122,   123,    34,    60,    35,    36,
      37,    38,   124,   139,    35,    36,    37,    38,    96,    97,
      82,   140,    34,    39,    35,    36,    37,    38,    34,    39,
      98,    99,   141,   151,   145,   146,   147,   156,   163,    39,
      35,    89,    37,    38,   157,   158,    35,   200,    37,    38,
     164,   167,   168,   169,   179,    39,   172,   180,   173,   175,
     176,    39,    63,    64,    65,    66,    67,    68,    90,    91,
      92,    93,    94,    95,   174,   177,   178,   185,   186,   196,
     187,   188,   189,   198,   199,   205,    59,   106,   206,   202,
     211,   208,   209,   212,   108,   182,   203,   154,   204,    78,
       0,   201
};

static const yytype_int16 yycheck[] =
{
      25,    49,    50,     6,     1,   148,    29,     1,    19,    39,
       1,    36,     1,    43,    39,     3,     4,     5,     6,     0,
      38,    46,    24,    13,    14,     1,    44,    30,    16,    17,
     173,    25,   175,    24,    23,    24,     1,    48,     3,     4,
       5,     6,     7,    40,     9,    10,    11,    12,    24,     1,
      15,    41,    24,    18,    77,   103,   199,    24,    23,    24,
      25,    26,    40,   206,    24,     3,     4,     5,     6,    40,
      42,    23,    40,    38,    24,    40,   101,    42,     3,     4,
       5,     6,    85,    39,    49,     1,    39,     3,     4,     5,
       6,     7,   140,     9,    10,    11,    12,    13,    14,    15,
      42,    43,    18,    41,    33,    34,    43,    23,    24,    25,
      26,    35,    36,    45,   139,    45,   141,    44,    41,    42,
     145,    39,    38,    43,    40,    41,    42,    41,   153,     3,
       4,     5,     6,    49,     1,    41,     3,     4,     5,     6,
       7,    42,     9,    10,    11,    12,    42,    43,    15,   174,
      42,    18,     3,     4,     5,     6,    23,    24,    25,    26,
       5,   186,     1,    22,     3,     4,     5,     6,    40,    13,
      14,    38,    42,    40,    41,    42,     5,    22,    23,    24,
      25,    26,    49,    38,    23,    24,    25,    26,    69,    70,
      41,    38,     5,    38,    23,    24,    25,    26,     5,    38,
      71,    72,    38,    42,    38,    42,    38,    42,    39,    38,
      23,    24,    25,    26,    42,    42,    23,    24,    25,    26,
      42,    39,    39,    39,    24,    38,    41,    10,    40,    40,
      40,    38,    27,    28,    29,    30,    31,    32,    63,    64,
      65,    66,    67,    68,    42,    42,    42,    39,    38,    42,
      41,    39,    41,     8,    40,    39,    33,    81,    40,    46,
      41,    46,    42,    41,    85,   174,   194,   140,   194,    48,
      -1,   190
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    51,    52,     0,    19,    48,    53,     3,     4,     5,
       6,    16,    17,    54,    55,    56,    57,    59,    68,    24,
      24,    24,    42,    63,    64,    69,    70,    71,    40,    40,
      38,    44,    42,    43,     5,    23,    24,    25,    26,    38,
      92,    93,    94,    95,    96,    97,    40,    65,    58,    61,
      61,     3,    68,    72,    73,    74,     1,    23,    24,    64,
      22,    92,    92,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    66,    67,    92,     1,    40,    60,    41,
      62,    68,    41,    24,    39,    43,    45,    45,    39,    24,
      94,    94,    94,    94,    94,    94,    95,    95,    96,    96,
      41,    43,    41,    61,    42,     1,    63,    42,    74,    92,
      75,    42,    42,     1,     5,     7,     9,    10,    11,    12,
      15,    18,    41,    42,    49,    60,    62,    76,    77,    78,
      79,    80,    81,    86,    89,    90,    91,    92,    42,    38,
      38,    38,     1,    23,    24,    38,    42,    38,    40,    92,
       1,    42,    62,    87,    91,    92,    42,    42,    42,    92,
       1,    25,    76,    39,    42,    88,    92,    39,    39,    39,
      39,    43,    41,    40,    42,    40,    40,    42,    42,    24,
      10,    76,    88,    76,    82,    39,    38,    41,    39,    41,
      13,    14,    41,    83,    84,    85,    42,    92,     8,    40,
      24,    97,    46,    75,    85,    39,    40,    76,    46,    42,
      76,    41,    41
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
        case 2:

/* Line 1455 of yacc.c  */
#line 66 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, (yyvsp[(2) - (2)].nd_obj).nd, "program"); head = (yyval.nd_obj).nd; ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 70 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, make_node(NULL, NULL, strdup(yylex)), "include"); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 71 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, make_node(NULL, NULL, strdup(yylex)), "preproc"); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 72 "parse_tree.y"
    { (yyval.nd_obj).nd = NULL; ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 76 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, (yyvsp[(2) - (2)].nd_obj).nd, "external_defs"); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 77 "parse_tree.y"
    { (yyval.nd_obj).nd = NULL; ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 81 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 82 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 83 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 84 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 89 "parse_tree.y"
    {
        struct node *idnode = make_node(NULL, NULL, strdup(yylex));
        (yyval.nd_obj).nd = make_node(idnode, (yyvsp[(4) - (6)].nd_obj).nd, "struct_def");
    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 97 "parse_tree.y"
    {
        struct node *idnode = make_node(NULL, NULL, strdup(yylex));
        (yyval.nd_obj).nd = make_node(idnode, (yyvsp[(4) - (6)].nd_obj).nd, "union_def");
    ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 105 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(2) - (3)].nd_obj).nd, "global_decl"); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 107 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, make_node(NULL,NULL,"ERROR_DECL"), "global_decl_err"); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 111 "parse_tree.y"
    { (yyval.nd_obj).nd = NULL; ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 116 "parse_tree.y"
    {
        (yyval.nd_obj).nd = make_node(make_node((yyvsp[(1) - (4)].nd_obj).nd, (yyvsp[(2) - (4)].nd_obj).nd, "func_head"), (yyvsp[(4) - (4)].nd_obj).nd, "function_def");
    ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 123 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(2) - (4)].nd_obj).nd, (yyvsp[(3) - (4)].nd_obj).nd, "block"); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 125 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL, make_node(NULL,NULL,"ERROR_BLOCK"), "block_err"); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 129 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, (yyvsp[(2) - (2)].nd_obj).nd, "decl_list"); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 130 "parse_tree.y"
    { (yyval.nd_obj).nd = NULL; ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 135 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(2) - (3)].nd_obj).nd, "decl"); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 137 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, make_node(NULL,NULL,"ERROR_DECL"), "decl_err"); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 141 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 142 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "declarator_list"); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 146 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 147 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, (yyvsp[(2) - (2)].nd_obj).nd, "decl_assign"); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 148 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 149 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, (yyvsp[(2) - (2)].nd_obj).nd, "decl_array_init"); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 153 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL, (yyvsp[(2) - (3)].nd_obj).nd, "init_list"); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 157 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 158 "parse_tree.y"
    { (yyval.nd_obj).nd = NULL; ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 162 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 163 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "expr_list"); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 167 "parse_tree.y"
    { insert_type(); (yyval.nd_obj).nd = make_node(NULL, NULL, "int"); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 168 "parse_tree.y"
    { insert_type(); (yyval.nd_obj).nd = make_node(NULL, NULL, "float"); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 169 "parse_tree.y"
    { insert_type(); (yyval.nd_obj).nd = make_node(NULL, NULL, "char"); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 170 "parse_tree.y"
    { insert_type(); (yyval.nd_obj).nd = make_node(NULL, NULL, "void"); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 174 "parse_tree.y"
    { add('V'); (yyval.nd_obj).nd = make_node(NULL, NULL, strdup(yylex)); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 179 "parse_tree.y"
    { add('V'); struct node *id = make_node(NULL,NULL,strdup(yytext)); struct node *num = make_node(NULL,NULL,strdup(yytext)); (yyval.nd_obj).nd = make_node(id, num, "array_decl"); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 181 "parse_tree.y"
    { add('V'); (yyval.nd_obj).nd = make_node(NULL,NULL,"array_decl_err"); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 186 "parse_tree.y"
    { add('V'); struct node *id = make_node(NULL,NULL,strdup(yytext)); (yyval.nd_obj).nd = make_node(id, (yyvsp[(3) - (4)].nd_obj).nd, "func_decl"); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 190 "parse_tree.y"
    { (yyval.nd_obj).nd = NULL; ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 191 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL,NULL,"void"); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 192 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 196 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 197 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "param_decls"); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 201 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, make_node(NULL,NULL,strdup(yytext)), "param"); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 205 "parse_tree.y"
    { (yyval.nd_obj).nd = NULL; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 206 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, (yyvsp[(2) - (2)].nd_obj).nd, "stmts"); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 210 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 211 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 212 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 213 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 214 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 215 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 216 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 217 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 218 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 219 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 220 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 221 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL,NULL,";"); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 222 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL,NULL,"ERROR_SEMI"); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 227 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(3) - (7)].nd_obj).nd, (yyvsp[(6) - (7)].nd_obj).nd, "if"); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 229 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(make_node((yyvsp[(3) - (11)].nd_obj).nd,(yyvsp[(6) - (11)].nd_obj).nd,"if"), (yyvsp[(10) - (11)].nd_obj).nd, "if_else"); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 234 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(3) - (7)].nd_obj).nd, (yyvsp[(6) - (7)].nd_obj).nd, "while"); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 239 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(3) - (9)].nd_obj).nd, (yyvsp[(7) - (9)].nd_obj).nd, "do_while"); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 244 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(make_node((yyvsp[(3) - (10)].nd_obj).nd,(yyvsp[(4) - (10)].nd_obj).nd,"for_init"), make_node((yyvsp[(6) - (10)].nd_obj).nd,(yyvsp[(9) - (10)].nd_obj).nd,"for_body"), "for"); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 249 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(3) - (7)].nd_obj).nd, (yyvsp[(6) - (7)].nd_obj).nd, "switch"); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 253 "parse_tree.y"
    { (yyval.nd_obj).nd = NULL; ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 254 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, (yyvsp[(2) - (2)].nd_obj).nd, "case_list"); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 258 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, (yyvsp[(2) - (2)].nd_obj).nd, "case_block"); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 262 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 263 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (2)].nd_obj).nd, (yyvsp[(2) - (2)].nd_obj).nd, "case_labels"); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 267 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(2) - (3)].nd_obj).nd, NULL, "case"); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 268 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL, NULL, "default"); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 272 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL, NULL, "break"); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 276 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 277 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 278 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL,NULL,";"); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 282 "parse_tree.y"
    { (yyval.nd_obj).nd = NULL; ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 283 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 288 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL, make_node(NULL,NULL,strdup(yytext)), "printf"); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 290 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(make_node(NULL,NULL,strdup(yytext)), make_node(NULL,NULL,strdup(yytext)), "printf2"); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 292 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL,NULL,"printf_err"); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 296 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL, make_node(NULL,NULL,strdup(yytext)), "return"); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 297 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL, make_node(NULL,NULL,strdup(yytext)), "return_id"); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 298 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL,NULL,"return_err"); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 302 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 303 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(NULL,NULL,"expr_err"); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 307 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(make_node(NULL,NULL,strdup(yytext)), (yyvsp[(2) - (2)].nd_obj).nd, "assign"); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 308 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 312 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 313 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "<"); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 314 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, ">"); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 315 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "<="); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 316 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, ">="); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 317 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "=="); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 318 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "!="); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 322 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 323 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "+"); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 324 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "-"); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 328 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 329 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "*"); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 330 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node((yyvsp[(1) - (3)].nd_obj).nd, (yyvsp[(3) - (3)].nd_obj).nd, "/"); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 334 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(1) - (1)].nd_obj).nd; ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 335 "parse_tree.y"
    { (yyval.nd_obj).nd = make_node(make_node(NULL,NULL,strdup(yytext)), NULL, "postfix_unary"); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 339 "parse_tree.y"
    { add('V'); (yyval.nd_obj).nd = make_node(NULL, NULL, strdup(yytext)); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 340 "parse_tree.y"
    { add('C'); (yyval.nd_obj).nd = make_node(NULL, NULL, strdup(yytext)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 341 "parse_tree.y"
    { add('C'); (yyval.nd_obj).nd = make_node(NULL, NULL, strdup(yytext)); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 342 "parse_tree.y"
    { add('C'); (yyval.nd_obj).nd = make_node(NULL, NULL, strdup(yytext)); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 343 "parse_tree.y"
    { add('C'); (yyval.nd_obj).nd = make_node(NULL, NULL, strdup(yytext)); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 344 "parse_tree.y"
    { (yyval.nd_obj).nd = (yyvsp[(2) - (3)].nd_obj).nd; ;}
    break;



/* Line 1455 of yacc.c  */
#line 2429 "parse_tree.tab.c"
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
#line 347 "parse_tree.y"


void yyerror (const char *s) {
    fprintf(stderr, "%s\n", s);
    return;
}

/* symbol table functions left unchanged */
void add (char c) {
    q = search(yylex);
    if (q == 0) {
        if (c == 'H') {
            symbol_table[ind].id_name = strdup(yylex);
            symbol_table[ind].data_type = strdup("N/A");
            symbol_table[ind].line_no = count;
            symbol_table[ind].type = strdup("Header");
            ind++;
        }
        else if (c == 'K') {
            symbol_table[ind].id_name = strdup(yylex);
            symbol_table[ind].data_type = strdup("N/A");
            symbol_table[ind].line_no = count;
            symbol_table[ind].type = strdup("Keyword");
            ind++;
        }
        else if (c == 'V') {
            symbol_table[ind].id_name = strdup(yylex);
            symbol_table[ind].data_type = strdup(type);
            symbol_table[ind].line_no = count;
            symbol_table[ind].type = strdup("Variable");
            ind++;
        }
        else if (c == 'C') {
            symbol_table[ind].id_name = strdup(yylex);
            symbol_table[ind].data_type = strdup("CONST");
            symbol_table[ind].line_no = count;
            symbol_table[ind].type = strdup("Constant");
            ind++;
        }
    }
}

void insert_type () {
    strcpy(type, yylex);
    return;
}

int search (char *type) {
    for (int i = ind - 1 ; i >= 0 ; i--) {
        if (strcmp(symbol_table[i].id_name, type) == 0) {
            return -1;
        }
    }
    return 0;
}



struct node* make_node (struct node* left, struct node* right, char *token) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    char* str = (char*)malloc(strlen(token) + 1);
    strcpy(str, token);
    new_node->left = left;
    new_node->right = right;
    new_node->token = str;
    return new_node;
}

int main () {
    yyparse();
    printf("-----------------------LEXICAL ANALYSIS-------------------------\n");
    printf(" SYMBOL        | DATATYPE      | TYPE          | LINE NUMBER   |\n");
    for (int i = 0 ; i < ind ; i++) {
        printf("%15s|%15s|%15s|%15d\n", symbol_table[i].id_name, symbol_table[i].data_type, symbol_table[i].type, symbol_table[i].line_no);
    }
    for (int i = 0 ; i < ind ; i++) {
        free(symbol_table[i].id_name);
        free(symbol_table[i].type);
    }

    printf("\n\n");
    printf("---------------------SYNTAX ANALYSIS--------------------------\n");
    print_tree(head);
    printf("\n\n");
    return 0;
}

