%{
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

%}

%union {
    int num;
    char *str;
}

%token <str> IDENTIFIER NUMBER STRING_LITERAL CHAR_LITERAL INCLUDE
%token INT FLOAT CHAR VOID
%token IF ELSE FOR WHILE RETURN
%token PRINTF
%token EQ NEQ LT GT LE GE
%token PLUS MINUS MULT DIVIDE
%token ASSIGN
%token OPAREN CPAREN OCURLY CCURLY SEMICOLON COMMA
%token OBRACKET CBRACKET
%token QUOTE UNMATCHEDSTRING PREPROCESSOR UNMATCHEDCOMMENT

%start program
%type <str> type_specifier params_list param_decl declarator_var declarator_array declarator_func

%%  /* GRAMMAR RULES BEGIN HERE */
/* ----------------- PART 2/2: GRAMMAR RULES + yyerror + main ----------------- */

program:
      includes external_defs
    ;

includes:
      /* empty */
    | includes INCLUDE           { /* include token possibly with yylval.str from lexer */ }
    ;

external_defs:
      external_defs external_def
    | /* empty */
    ;

external_def:
      function_def
    | global_decl
    ;

global_decl:
      type_specifier declarator_var SEMICOLON
        {
            sym_insert_or_update($2, $1, "variable", "-", "-", "-", 0, current_nesting, yylineno);
            free($1); free($2);
        }
    | type_specifier declarator_array SEMICOLON
        {
            /* declarator_array inserted symbol already inside its action */
            free($1);
        }
    | type_specifier error SEMICOLON
        {
            fprintf(stderr, "Error at line %d: invalid global declaration\n", yylineno);
            yyerrok;
        }
    ;

function_def:
      type_specifier declarator_func block
        {
            if($2){
                sym_insert_or_update($2, $1, "function", "-", "-", "-", 1, current_nesting, yylineno);
                free($2);
            }
            free($1);
        }
    ;

/* block increases nesting for declarations */
block:
    OCURLY { current_nesting++; } decl_list stmt_list CCURLY { current_nesting--; }
  | OCURLY { current_nesting++; } error CCURLY
        {
            fprintf(stderr, "Error at line %d: invalid block contents\n", yylineno);
            yyerrok;
            current_nesting--;
        }
  | error CCURLY
        {
            fprintf(stderr, "Error at line %d: missing '{' before block\n", yylineno);
            yyerrok;
        }
    ;

decl_list:
    /* empty */
    | decl_list decl
    ;

decl:
    type_specifier declarator_var SEMICOLON
        {
            sym_insert_or_update($2, $1, "variable", "-", "-", "-", 0, current_nesting, yylineno);
            free($1); free($2);
        }
    | type_specifier declarator_array SEMICOLON
        {
            /* declarator_array handled in its action */
            free($1);
        }
    | type_specifier error SEMICOLON
        {
            fprintf(stderr, "Error at line %d: invalid local declaration\n", yylineno);
            yyerrok;
        }
    ;

/* types */
type_specifier:
      INT   { $$ = xstrdup("int"); }
    | FLOAT { $$ = xstrdup("float"); }
    | CHAR  { $$ = xstrdup("char"); }
    | VOID  { $$ = xstrdup("void"); }
    ;

/* declarators */
declarator_var:
      IDENTIFIER { $$ = $1; }   /* caller must free */
    ;

declarator_array:
      IDENTIFIER OBRACKET NUMBER CBRACKET
        {
            char dims[128]; snprintf(dims,sizeof(dims),"[%s]", $3);
            sym_insert_or_update($1, "-", "array", "-", dims, "-", 0, current_nesting, yylineno);
            const_insert_or_bump($3, "int", yylineno);
            free($3);
            $$ = $1;
        }
    | IDENTIFIER OBRACKET error CBRACKET
        {
            /* malformed array size */
            fprintf(stderr, "Error at line %d: invalid array size in declaration of '%s'\n", yylineno, $1);
            yyerrok;
            $$ = $1;
        }
    ;

/* function declarator: IDENTIFIER ( params )  */
declarator_func:
      IDENTIFIER OPAREN params_list CPAREN
        {
            char *params = $3 ? $3 : xstrdup("-");
            sym_insert_or_update($1, "-", "function", "-", "-", params, 0, current_nesting, yylineno);
            free(params);
            $$ = $1;  /* pass function name up (caller will free) */
        }
    | IDENTIFIER OPAREN error CPAREN
        {
            fprintf(stderr, "Error at line %d: malformed parameter list for function '%s'\n", yylineno, $1);
            yyerrok;
            $$ = $1;
        }
    ;

/* parameter list */
params_list:
      /* empty */         { $$ = NULL; }
    | param_decl           { $$ = $1; }
    | params_list COMMA param_decl
        {
            char *a = $1;
            char *b = $3;
            if(!a) { $$ = b; }
            else {
                size_t n = strlen(a) + strlen(b) + 4;
                char *out = malloc(n);
                snprintf(out,n,"%s, %s", a, b);
                free(a); free(b);
                $$ = out;
            }
        }
    | error
        {
            fprintf(stderr, "Error at line %d: invalid parameters in parameter list\n", yylineno);
            yyerrok;
            $$ = NULL;
        }
    ;

param_decl:
      type_specifier IDENTIFIER
        {
            size_t n = strlen($1) + strlen($2) + 4;
            char *s = malloc(n);
            snprintf(s,n,"%s %s",$1,$2);
            free($1); free($2);
            $$ = s;
        }
    ;

/* statements */
stmt_list:
      /* empty */
    | stmt_list statement
    ;

statement:
      expr_stmt
    | printf_stmt
    | return_stmt
    | local_decl
    | error SEMICOLON
        {
            fprintf(stderr, "Error at line %d: invalid statement\n", yylineno);
            yyerrok;
        }
    ;

/* local declarations */
local_decl:
      type_specifier declarator_var SEMICOLON
        {
            sym_insert_or_update($2, $1, "variable", "-", "-", "-", 0, current_nesting, yylineno);
            free($1); free($2);
        }
    | type_specifier declarator_var ASSIGN expr SEMICOLON
        {
            sym_insert_or_update($2, $1, "variable", "-", "-", "-", 0, current_nesting, yylineno);
            free($1); free($2);
        }  
    | type_specifier declarator_array SEMICOLON
        {
            free($1);
        }
    | type_specifier error SEMICOLON
        {
            fprintf(stderr, "Error at line %d: invalid local declaration\n", yylineno);
            yyerrok;
        }
    ;

/* printf forms */
printf_stmt:
      PRINTF OPAREN STRING_LITERAL CPAREN SEMICOLON
        {
            const_insert_or_bump($3, "string", yylineno);
            free($3);
        }
    | PRINTF OPAREN STRING_LITERAL COMMA IDENTIFIER CPAREN SEMICOLON
        {
            const_insert_or_bump($3, "string", yylineno);
            sym_add_reference($5, yylineno);
            free($3); free($5);
        }
    | PRINTF OPAREN error CPAREN SEMICOLON
        {
            fprintf(stderr, "Error at line %d: malformed printf arguments\n", yylineno);
            yyerrok;
        }
    ;

/* return */
return_stmt:
      RETURN NUMBER SEMICOLON
        {
            const_insert_or_bump($2,"int", yylineno);
            free($2);
        }
    | RETURN IDENTIFIER SEMICOLON
        {
            sym_add_reference($2, yylineno);
            free($2);
        }
    | RETURN error SEMICOLON
        {
            fprintf(stderr, "Error at line %d: invalid return value\n", yylineno);
            yyerrok;
        }
    ;

/* expressions */
expr_stmt:
      expr SEMICOLON
    | error SEMICOLON
        {
            fprintf(stderr, "Error at line %d: invalid expression\n", yylineno);
            yyerrok;
        }
    ;

expr:
      IDENTIFIER ASSIGN expr
        {
            sym_add_reference($1, yylineno);
            free($1);
        }
    | NUMBER
        {
            const_insert_or_bump($1,"int", yylineno);
            free($1);
        }
    | IDENTIFIER
        {
            sym_add_reference($1, yylineno);
            free($1);
        }
    | error
        {
            fprintf(stderr, "Error at line %d: incomplete or invalid expression\n", yylineno);
            yyerrok;
        }
    ;

%%

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
