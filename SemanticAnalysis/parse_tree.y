%{
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

%}

%union{
    struct var_name {
        char name[100];
        struct node* nd;
    } nd_obj;
}

%token VOID
%token INT FLOAT CHAR IF ELSE FOR WHILE RETURN SWITCH CASE DEFAULT BREAK STRUCT UNION PRINTF INCLUDE TRUE FALSE UNARY NUMBER IDENTIFIER STRING_LITERAL CHAR_LITERAL EQ NEQ LE GE LT GT PLUS MINUS MULT DIVIDE ASSIGN OPAREN CPAREN OCURLY CCURLY SEMICOLON COMMA OBRACKET CBRACKET COLON UNMATCHED_COMMENT PREPROCESSOR DO
%type <nd_obj> program preamble external_defs external_def function_definition global_decl struct_definition union_definition decl_list type_specifier declarator_list save_params declarator_func block stmt_list decl declarator declarator_var declarator_array initializer_list optional_expression_list expression_list expr params_list param_decls param_decl statement expr_stmt printf_stmt return_stmt if_stmt while_stmt for_stmt switch_stmt break_stmt do_while_stmt for_init_stmt optional_expr case_list case_block case_labels case_label primary_expr cond_expr add_expr mult_expr postfix_expr


%%

program:
    preamble external_defs { $$.nd = make_node($1.nd, $2.nd, "program"); head = $$.nd; }
    ;

preamble:
    preamble INCLUDE        { $$.nd = make_node($1.nd, make_node(NULL, NULL, strdup(yylex)), "include"); }
    | preamble PREPROCESSOR { $$.nd = make_node($1.nd, make_node(NULL, NULL, strdup(yylex)), "preproc"); }
    | /* empty */           { $$.nd = NULL; }
    ;

external_defs:
    external_defs external_def { $$.nd = make_node($1.nd, $2.nd, "external_defs"); }
    | /* empty */              { $$.nd = NULL; }
    ;

external_def:
    function_definition { $$.nd = $1.nd; }
    | global_decl       { $$.nd = $1.nd; }
    | struct_definition { $$.nd = $1.nd; }
    | union_definition  { $$.nd = $1.nd; }
    ;

struct_definition:
    STRUCT IDENTIFIER OCURLY decl_list CCURLY SEMICOLON
    {
        struct node *idnode = make_node(NULL, NULL, strdup(yylex));
        $$.nd = make_node(idnode, $4.nd, "struct_def");
    }
    ;

union_definition:
    UNION IDENTIFIER OCURLY decl_list CCURLY SEMICOLON
    {
        struct node *idnode = make_node(NULL, NULL, strdup(yylex));
        $$.nd = make_node(idnode, $4.nd, "union_def");
    }
    ;

global_decl:
    type_specifier declarator_list SEMICOLON
    { $$.nd = make_node($1.nd, $2.nd, "global_decl"); }
    | type_specifier /*error*/ SEMICOLON
    { $$.nd = make_node($1.nd, make_node(NULL,NULL,"ERROR_DECL"), "global_decl_err"); }
    ;

save_params:
    { $$.nd = NULL; }
    ;

function_definition:
    type_specifier declarator_func save_params block
    {
        $$.nd = make_node(make_node($1.nd, $2.nd, "func_head"), $4.nd, "function_def");
    }
    ;

block:
    OCURLY decl_list stmt_list CCURLY
    { $$.nd = make_node($2.nd, $3.nd, "block"); }
    | error CCURLY
    { $$.nd = make_node(NULL, make_node(NULL,NULL,"ERROR_BLOCK"), "block_err"); }
    ;

decl_list:
    decl_list decl { $$.nd = make_node($1.nd, $2.nd, "decl_list"); }
    | /* empty */   { $$.nd = NULL; }
    ;

decl:
    type_specifier declarator_list SEMICOLON
    { $$.nd = make_node($1.nd, $2.nd, "decl"); }
    | type_specifier error SEMICOLON
    { $$.nd = make_node($1.nd, make_node(NULL,NULL,"ERROR_DECL"), "decl_err"); }
    ;

declarator_list:
    declarator { $$.nd = $1.nd; }
    | declarator_list COMMA declarator { $$.nd = make_node($1.nd, $3.nd, "declarator_list"); }
    ;

declarator:
    declarator_var { $$.nd = $1.nd; }
    | declarator_var /*op_assign*/ expr { $$.nd = make_node($1.nd, $2.nd, "decl_assign"); }
    | declarator_array { $$.nd = $1.nd; }
    | declarator_array /*op_assign*/ initializer_list { $$.nd = make_node($1.nd, $2.nd, "decl_array_init"); }
    ;

initializer_list:
    OCURLY optional_expression_list CCURLY { $$.nd = make_node(NULL, $2.nd, "init_list"); }
    ;

optional_expression_list:
    expression_list { $$.nd = $1.nd; }
    | /* empty */    { $$.nd = NULL; }
    ;

expression_list:
    expr { $$.nd = $1.nd; }
    | expression_list COMMA expr { $$.nd = make_node($1.nd, $3.nd, "expr_list"); }
    ;

type_specifier:
    INT   { insert_type(); $$.nd = make_node(NULL, NULL, "int"); }
    | FLOAT { insert_type(); $$.nd = make_node(NULL, NULL, "float"); }
    | CHAR  { insert_type(); $$.nd = make_node(NULL, NULL, "char"); }
    | VOID  { insert_type(); $$.nd = make_node(NULL, NULL, "void"); }
    ;

declarator_var:
    IDENTIFIER { add('V'); $$.nd = make_node(NULL, NULL, strdup(yylex)); }
    ;

declarator_array:
    IDENTIFIER OBRACKET NUMBER CBRACKET
    { add('V'); struct node *id = make_node(NULL,NULL,strdup(yytext)); struct node *num = make_node(NULL,NULL,strdup(yytext)); $$.nd = make_node(id, num, "array_decl"); }
    | IDENTIFIER OBRACKET error CBRACKET
    { add('V'); $$.nd = make_node(NULL,NULL,"array_decl_err"); }
    ;

declarator_func:
    IDENTIFIER OPAREN params_list CPAREN
    { add('V'); struct node *id = make_node(NULL,NULL,strdup(yytext)); $$.nd = make_node(id, $3.nd, "func_decl"); }
    ;

params_list:
    /* empty */ { $$.nd = NULL; }
    | VOID       { $$.nd = make_node(NULL,NULL,"void"); }
    | param_decls { $$.nd = $1.nd; }
    ;

param_decls:
    param_decl { $$.nd = $1.nd; }
    | param_decls COMMA param_decl { $$.nd = make_node($1.nd, $3.nd, "param_decls"); }
    ;

param_decl:
    type_specifier IDENTIFIER { $$.nd = make_node($1.nd, make_node(NULL,NULL,strdup(yytext)), "param"); }
    ;

stmt_list:
    /* empty */ { $$.nd = NULL; }
    | stmt_list statement { $$.nd = make_node($1.nd, $2.nd, "stmts"); }
    ;

statement:
    expr_stmt { $$.nd = $1.nd; }
    | printf_stmt { $$.nd = $1.nd; }
    | return_stmt { $$.nd = $1.nd; }
    | decl { $$.nd = $1.nd; }
    | if_stmt { $$.nd = $1.nd; }
    | while_stmt { $$.nd = $1.nd; }
    | do_while_stmt { $$.nd = $1.nd; }
    | for_stmt { $$.nd = $1.nd; }
    | switch_stmt { $$.nd = $1.nd; }
    | break_stmt { $$.nd = $1.nd; }
    | block { $$.nd = $1.nd; }
    | SEMICOLON { $$.nd = make_node(NULL,NULL,";"); }
    | error SEMICOLON { $$.nd = make_node(NULL,NULL,"ERROR_SEMI"); }
    ;

if_stmt:
    IF OPAREN expr CPAREN OCURLY statement CCURLY
    { $$.nd = make_node($3.nd, $6.nd, "if"); }
    | IF OPAREN expr CPAREN OCURLY statement CCURLY ELSE OCURLY statement CCURLY
    { $$.nd = make_node(make_node($3.nd,$6.nd,"if"), $10.nd, "if_else"); }
    ;

while_stmt:
    WHILE OPAREN expr CPAREN OCURLY statement CCURLY
    { $$.nd = make_node($3.nd, $6.nd, "while"); }
    ;

do_while_stmt:
    DO OCURLY statement CCURLY WHILE OPAREN expr CPAREN SEMICOLON
    { $$.nd = make_node($3.nd, $7.nd, "do_while"); }
    ;

for_stmt:
    FOR OPAREN for_init_stmt optional_expr SEMICOLON optional_expr CPAREN OCURLY statement CCURLY
    { $$.nd = make_node(make_node($3.nd,$4.nd,"for_init"), make_node($6.nd,$9.nd,"for_body"), "for"); }
    ;

switch_stmt:
    SWITCH OPAREN expr CPAREN OCURLY case_list CCURLY
    { $$.nd = make_node($3.nd, $6.nd, "switch"); }
    ;

case_list:
    /* empty */ { $$.nd = NULL; }
    | case_list case_block { $$.nd = make_node($1.nd, $2.nd, "case_list"); }
    ;

case_block:
    case_labels stmt_list { $$.nd = make_node($1.nd, $2.nd, "case_block"); }
    ;

case_labels:
    case_label { $$.nd = $1.nd; }
    | case_labels case_label { $$.nd = make_node($1.nd, $2.nd, "case_labels"); }
    ;

case_label:
    CASE primary_expr COLON { $$.nd = make_node($2.nd, NULL, "case"); }
    | DEFAULT COLON { $$.nd = make_node(NULL, NULL, "default"); }
    ;

break_stmt:
    BREAK SEMICOLON { $$.nd = make_node(NULL, NULL, "break"); }
    ;

for_init_stmt:
    expr_stmt { $$.nd = $1.nd; }
    | decl { $$.nd = $1.nd; }
    | SEMICOLON { $$.nd = make_node(NULL,NULL,";"); }
    ;

optional_expr:
    /* empty */ { $$.nd = NULL; }
    | expr { $$.nd = $1.nd; }
    ;

printf_stmt:
    PRINTF OPAREN STRING_LITERAL CPAREN SEMICOLON
    { $$.nd = make_node(NULL, make_node(NULL,NULL,strdup(yytext)), "printf"); }
    | PRINTF OPAREN STRING_LITERAL COMMA IDENTIFIER CPAREN SEMICOLON
    { $$.nd = make_node(make_node(NULL,NULL,strdup(yytext)), make_node(NULL,NULL,strdup(yytext)), "printf2"); }
    | PRINTF OPAREN error CPAREN SEMICOLON
    { $$.nd = make_node(NULL,NULL,"printf_err"); }
    ;

return_stmt:
    RETURN NUMBER SEMICOLON { $$.nd = make_node(NULL, make_node(NULL,NULL,strdup(yytext)), "return"); }
    | RETURN IDENTIFIER SEMICOLON { $$.nd = make_node(NULL, make_node(NULL,NULL,strdup(yytext)), "return_id"); }
    | RETURN error SEMICOLON { $$.nd = make_node(NULL,NULL,"return_err"); }
    ;

expr_stmt:
    expr { $$.nd = $1.nd; }
    | error SEMICOLON { $$.nd = make_node(NULL,NULL,"expr_err"); }
    ;

expr:
    IDENTIFIER /*op_assign*/ expr { $$.nd = make_node(make_node(NULL,NULL,strdup(yytext)), $2.nd, "assign"); }
    | cond_expr { $$.nd = $1.nd; }
    ;

cond_expr:
    add_expr { $$.nd = $1.nd; }
    | cond_expr LT add_expr { $$.nd = make_node($1.nd, $3.nd, "<"); }
    | cond_expr GT add_expr { $$.nd = make_node($1.nd, $3.nd, ">"); }
    | cond_expr LE add_expr { $$.nd = make_node($1.nd, $3.nd, "<="); }
    | cond_expr GE add_expr { $$.nd = make_node($1.nd, $3.nd, ">="); }
    | cond_expr EQ add_expr { $$.nd = make_node($1.nd, $3.nd, "=="); }
    | cond_expr NEQ add_expr { $$.nd = make_node($1.nd, $3.nd, "!="); }
    ;

add_expr:
    mult_expr { $$.nd = $1.nd; }
    | add_expr PLUS mult_expr { $$.nd = make_node($1.nd, $3.nd, "+"); }
    | add_expr MINUS mult_expr { $$.nd = make_node($1.nd, $3.nd, "-"); }
    ;

mult_expr:
    postfix_expr { $$.nd = $1.nd; }
    | mult_expr MULT postfix_expr { $$.nd = make_node($1.nd, $3.nd, "*"); }
    | mult_expr DIVIDE postfix_expr { $$.nd = make_node($1.nd, $3.nd, "/"); }
    ;

postfix_expr:
    primary_expr { $$.nd = $1.nd; }
    | IDENTIFIER UNARY { $$.nd = make_node(make_node(NULL,NULL,strdup(yytext)), NULL, "postfix_unary"); }
    ;

primary_expr:
    IDENTIFIER { add('V'); $$.nd = make_node(NULL, NULL, strdup(yytext)); }
    | NUMBER     { add('C'); $$.nd = make_node(NULL, NULL, strdup(yytext)); }
    | FLOAT      { add('C'); $$.nd = make_node(NULL, NULL, strdup(yytext)); }
    | STRING_LITERAL { add('C'); $$.nd = make_node(NULL, NULL, strdup(yytext)); }
    | CHAR_LITERAL   { add('C'); $$.nd = make_node(NULL, NULL, strdup(yytext)); }
    | OPAREN expr CPAREN { $$.nd = $2.nd; }
    ;

%%

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
