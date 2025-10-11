%{
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

%}

%union {
    int num;
    char *str;
}

%token <str> IDENTIFIER NUMBER STRING_LITERAL CHAR_LITERAL INCLUDE PREPROCESSOR
%token INT FLOAT CHAR VOID
%token IF ELSE FOR WHILE RETURN
%token PRINTF
%token EQ NEQ LT GT LE GE
%token PLUS MINUS MULT DIVIDE
%token ASSIGN
%token OPAREN CPAREN OCURLY CCURLY SEMICOLON COMMA
%token OBRACKET CBRACKET
%token QUOTE UNMATCHEDSTRING UNMATCHEDCOMMENT

%nonassoc IFX
%nonassoc ELSE
%left EQ NEQ LT GT LE GE
%left PLUS MINUS
%left MULT DIVIDE

%start program
%type <str> type_specifier params_list param_decls param_decl declarator_var declarator_array declarator_func

%%
/* --- Helper rules to capture every token --- */
p_oparen:    OPAREN    { sym_insert_or_update("(", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); };
p_cparen:    CPAREN    { sym_insert_or_update(")", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); };
p_ocurly:    OCURLY    { sym_insert_or_update("{", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); };
p_ccurly:    CCURLY    { sym_insert_or_update("}", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); };
p_semicolon: SEMICOLON { sym_insert_or_update(";", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); };
p_comma:     COMMA     { sym_insert_or_update(",", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); };
p_obracket:  OBRACKET  { sym_insert_or_update("[", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); };
p_cbracket:  CBRACKET  { sym_insert_or_update("]", "punctuation", "punctuation", NULL, NULL, NULL, 0, 0, yylineno); };

op_assign:   ASSIGN    { sym_insert_or_update("=", "operator", "assignment", NULL, NULL, NULL, 0, 0, yylineno); };
op_eq:       EQ        { sym_insert_or_update("==", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); };
op_neq:      NEQ       { sym_insert_or_update("!=", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); };
op_lt:       LT        { sym_insert_or_update("<", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); };
op_gt:       GT        { sym_insert_or_update(">", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); };
op_le:       LE        { sym_insert_or_update("<=", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); };
op_ge:       GE        { sym_insert_or_update(">=", "operator", "comparison", NULL, NULL, NULL, 0, 0, yylineno); };
op_plus:     PLUS      { sym_insert_or_update("+", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); };
op_minus:    MINUS     { sym_insert_or_update("-", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); };
op_mult:     MULT      { sym_insert_or_update("*", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); };
op_divide:   DIVIDE    { sym_insert_or_update("/", "operator", "arithmetic", NULL, NULL, NULL, 0, 0, yylineno); };

/* --- Main Grammar --- */

program:
      preamble external_defs
    ;

preamble:
      /* empty */
    | preamble INCLUDE
        {
            if ($2) {
                sym_insert_or_update("#include", "preprocessor", "directive", NULL, NULL, NULL, 0, 0, yylineno);
                sym_insert_or_update($2, "header", "file", NULL, NULL, NULL, 0, 0, yylineno);
                free($2);
            }
        }
    | preamble PREPROCESSOR
        {
             if ($2) {
                sym_insert_or_update($2, "preprocessor", "directive", NULL, NULL, NULL, 0, 0, yylineno);
                free($2);
            }
        }
    ;

external_defs:
      | external_defs external_def
    ;

external_def:
      function_def
    | global_decl
    ;

global_decl:
      type_specifier declarator_var p_semicolon
        {
            printf("Line %d: Parsed a global variable declaration for '%s'.\n", yylineno, $2);
            sym_insert_or_update($2, $1, "variable", NULL, NULL, NULL, 0, current_nesting, yylineno);
            free($1); free($2);
        }
    | type_specifier declarator_array p_semicolon
        {
            printf("Line %d: Parsed a global array declaration.\n", yylineno);
            free($1);
        }
    | type_specifier error p_semicolon { yyerrok; }
    ;

function_def:
    type_specifier declarator_func block
    {
        printf("Line %d: Parsed a function definition for '%s'.\n", yylineno, $2);
        if($2) { 
            sym_insert_or_update($2, $1, "function", NULL, NULL, NULL, 1, current_nesting, yylineno);
            free($2);
        }

    free($1);
    };

block:
    p_ocurly { current_nesting++; } decl_list stmt_list p_ccurly { current_nesting--; }
  | p_ocurly { current_nesting++; } error p_ccurly { yyerrok; current_nesting--; }
  | error p_ccurly { yyerrok; }
    ;

decl_list:
    | decl_list decl
    ;

/* MODIFIED: This rule now handles ALL local declarations, including with initializers. */
decl:
    type_specifier declarator_var p_semicolon
        {
            printf("Line %d: Parsed a local variable declaration for '%s'.\n", yylineno, $2);
            sym_insert_or_update($2, $1, "variable", NULL, NULL, NULL, 0, current_nesting, yylineno);
            free($1); free($2);
        }
    | type_specifier declarator_var op_assign expr p_semicolon
        {
            printf("Line %d: Parsed a local variable declaration with initialization for '%s'.\n", yylineno, $2);
            sym_insert_or_update($2, $1, "variable", NULL, NULL, NULL, 0, current_nesting, yylineno);
            free($1); free($2);
        }
    | type_specifier declarator_array p_semicolon
        { 
            printf("Line %d: Parsed a local array declaration.\n", yylineno);
            /* Action handled in declarator_array */ 
        }
    | type_specifier error p_semicolon { yyerrok; }
    ;

type_specifier:
      INT   { sym_insert_or_update("int", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);   $$ = xstrdup("int"); }
    | FLOAT { sym_insert_or_update("float", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno); $$ = xstrdup("float"); }
    | CHAR  { sym_insert_or_update("char", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);  $$ = xstrdup("char"); }
    | VOID  { sym_insert_or_update("void", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);  $$ = xstrdup("void"); }
    ;

declarator_var:
      IDENTIFIER { $$ = $1; }
    ;

declarator_array:
      IDENTIFIER p_obracket NUMBER p_cbracket
        {
            char dims[128]; snprintf(dims,sizeof(dims),"[%s]", $3);
            sym_insert_or_update($1, "array", "variable", NULL, dims, NULL, 0, current_nesting, yylineno);
            const_insert_or_bump($3, "int", yylineno);
            free($3);
            $$ = $1;
        }
    | IDENTIFIER p_obracket error p_cbracket { yyerrok; $$ = $1; }
    ;

/* MODIFIED: This rule now returns the parameter string ($3) in yylval for function_def to use. */
declarator_func:
      IDENTIFIER p_oparen params_list p_cparen
        {
            sym_insert_or_update($1, "function", "function", NULL, NULL, $3, 0, current_nesting, yylineno);
            $$ = $1;   // Pass up function name in $$
            yylval.str = $3; // Pass up param string in yylval
        }
    ;

/* MODIFIED: This structure correctly handles func(), func(void), and func(int a, ...) without conflicts. */
params_list:
      /* For functions like func() */
      { $$ = xstrdup(""); }
    | VOID
      {
          /* For functions like func(void) */
          sym_insert_or_update("void", "keyword", "type", NULL, NULL, NULL, 0, 0, yylineno);
          $$ = xstrdup("void");
      }
    | param_decls
      { $$ = $1; /* Pass up the concatenated string of parameters */ }
    ;

param_decls:
      param_decl
    | param_decls p_comma param_decl
      {
            char *a = $1; char *b = $3;
            if(!a) { $$ = b; } else {
                size_t n = strlen(a) + strlen(b) + 4;
                char *out = malloc(n);
                snprintf(out, n, "%s, %s", a, b);
                free(a); free(b);
                $$ = out;
            }
      }
    ;

param_decl:
      type_specifier IDENTIFIER
        {
            sym_insert_or_update($2, $1, "parameter", NULL, NULL, NULL, 0, current_nesting+1, yylineno);
            size_t n = strlen($1) + strlen($2) + 2;
            char *s = malloc(n);
            snprintf(s, n, "%s %s", $1, $2);
            free($1); free($2);
            $$ = s;
        }
    ;

stmt_list:
      | stmt_list statement
    ;

statement:
      expr_stmt
    | printf_stmt
    | return_stmt
    | decl /* MODIFIED: Use the unified 'decl' rule for declarations as statements. */
    | if_stmt
    | while_stmt
    | for_stmt
    | block
    | p_semicolon
    | error p_semicolon { yyerrok; }
    ;

if_stmt:
      IF p_oparen expr p_cparen statement %prec IFX
        { 
            sym_insert_or_update("if", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed an if statement.\n", yylineno);
        }
    | IF p_oparen expr p_cparen statement ELSE statement
        { 
            sym_insert_or_update("if", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            sym_insert_or_update("else", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed an if-else statement.\n", yylineno);
        }
    ;

while_stmt:
      WHILE p_oparen expr p_cparen statement
        { 
            sym_insert_or_update("while", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a while loop.\n", yylineno);
        }
    ;

for_stmt:
      FOR p_oparen for_init_stmt optional_expr p_semicolon optional_expr p_cparen statement
        { 
            sym_insert_or_update("for", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a for loop.\n", yylineno);
        }
    ;

for_init_stmt:
      expr_stmt
    | decl /* MODIFIED: Declarations in for loops now use the unified rule. */
    | p_semicolon
    ;

optional_expr:
      | expr
    ;

/* local_decl rule has been removed and merged into the 'decl' rule. */

printf_stmt:
      PRINTF p_oparen STRING_LITERAL p_cparen p_semicolon
        { 
            sym_insert_or_update("printf", "keyword", "function", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a printf statement.\n", yylineno);
            const_insert_or_bump($3, "string", yylineno);
            free($3);
        }
    | PRINTF p_oparen STRING_LITERAL p_comma IDENTIFIER p_cparen p_semicolon
        { 
            sym_insert_or_update("printf", "keyword", "function", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a printf statement with variable '%s'.\n", yylineno, $5);
            const_insert_or_bump($3, "string", yylineno);
            sym_add_reference($5, yylineno);
            free($3); free($5);
        }
    | PRINTF p_oparen error p_cparen p_semicolon { yyerrok; }
    ;

return_stmt:
      RETURN NUMBER p_semicolon
        { 
            sym_insert_or_update("return", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a return statement with value %s.\n", yylineno, $2);
            const_insert_or_bump($2, "int", yylineno);
            free($2);
        }
    | RETURN IDENTIFIER p_semicolon
        { 
            sym_insert_or_update("return", "keyword", "control", NULL, NULL, NULL, 0, 0, yylineno);
            printf("Line %d: Parsed a return statement with variable '%s'.\n", yylineno, $2);
            sym_add_reference($2, yylineno);
            free($2);
        }
    | RETURN error p_semicolon { yyerrok; }
    ;

expr_stmt:
      expr p_semicolon
    | error p_semicolon { yyerrok; }
    ;

expr:
      IDENTIFIER op_assign expr
        { 
            printf("Line %d: Parsed an assignment to variable '%s'.\n", yylineno, $1);
            sym_add_reference($1, yylineno);
            free($1);
        }
    | cond_expr
    ;

cond_expr:
      add_expr
    | cond_expr op_lt add_expr
    | cond_expr op_gt add_expr
    | cond_expr op_le add_expr
    | cond_expr op_ge add_expr
    | cond_expr op_eq add_expr
    | cond_expr op_neq add_expr
    ;

add_expr:
      mul_expr
    | add_expr op_plus mul_expr
    | add_expr op_minus mul_expr
    ;

mul_expr:
      primary_expr
    | mul_expr op_mult primary_expr
    | mul_expr op_divide primary_expr
    ;

primary_expr:
      IDENTIFIER
        { sym_add_reference($1, yylineno); free($1); }
    | NUMBER
        { const_insert_or_bump($1, "int", yylineno); free($1); }
    | STRING_LITERAL
        { const_insert_or_bump($1, "string", yylineno); free($1); }
    | CHAR_LITERAL
        { const_insert_or_bump($1, "char", yylineno); free($1); }
    | p_oparen expr p_cparen
    ;

%%

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