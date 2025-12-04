%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"lex.yy.c"

void yyerror (const char *s);
int yylex ();
int yywrap ();
void add (char c);
void insert_type ();
int search (char *);
void print_tree (struct node*);
void print_inorder (struct node*);
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
%token<nd_obj> INT FLOAT CHAR IF ELSE FOR WHILE RETURN SWITCH CASE DEFAULT BREAK STRUCT UNION PRINTF INCLUDE TRUE FALSE UNARY NUMBER FLOAT IDENTIFIER STRING_LITERAL CHAR_LITERAL EQ NEQ LE GE LT GT PLUS MINUS MULT DIVIDE ASSIGN OPAREN CPAREN OCURLY CCURLY SEMICOLON COMMA OBRACKET CBRACKET COLON UNMATCHED_COMMENT PREPROCESSOR DO
%token<nd_obj> preamble external_defs external_def function_definition global_decl struct_definition union_definition decl_list type_specifier declarator_list error save_params declarator_func block stmt_list decl declarator declarator_var declarator_array op_assign initializer_list optional_expression_list expression_list expr params_list param_decls param_decl statement expr_stmt printf_stmt return_stmt decl if_stmt while_stmt for_stmt switch_stmt break_stmt do_while_stmt for_init_stmt optional_expr case_list case_block case_labels stmt_list case_label primary_expr cond_expr add_expr mult_expr postfix_expr
%start program

%%

program:
    preamble external_defs
    ;

preamble:
    preamble INCLUDE
    | preamble PREPROCESSOR
    ;

external_defs:
    external_defs external_def
    |
    ;

external_def:
    function_definition
    | global_decl
    | struct_definition
    | union_definition
    ;

struct_definition:
    STRUCT IDENTIFIER OCURLY decl_list CCURLY SEMICOLON
    ;

union_definition:
    UNION IDENTIFIER OCURLY decl_list CCURLY SEMICOLON
    ;

global_decl:
    type_specifier declarator_list SEMICOLON
    | type_specifier error SEMICOLON
    ;

save_params:
    ;

function_definition:
    type_specifier declarator_func save_params block
    ;

block:
    OCURLY decl_list stmtm_list CCURLY
    | error CCURLY
    ;

decl_list:
    decl_list decl
    |
    ;

decl:
    type_specifier declarator_list SEMICOLON
    | type_specifier error SEMICOLON
    ;

declarator_list:
    declarator
    | declarator_list COMMA declarator
    ;

declarator:
    declarator_var
    | declarator_var op_assign expr
    | declarator_array
    | declarator_array op_assign initializer_list
    ;

initializer_list:
    OCURLY optional_expression_list CCURLY
    ;

optional_expression_list:
    expression_list
    |
    ;

expression_list:
    expr
    | expression_list COMMA expr
    ;

type_specifier:
    INT
    | FLOAT
    | CHAR
    | VOID
    ;

declarator_var:
    IDENTIFIER
    ;

declarator_array:
    IDENTIFIER OBRACKET NUMBER CBRACKET
    | IDENTIFIER OBRACKET error CBRACKET
    ;

declarator_func:
    IDENTIFIER OPAREN params_list CPAREN
    ;

params_list:

    | VOID
    | param_decls
    ;

param_decls:
    param_decl
    | param_decls COMMA param_decl
    ;

param_decl:
    type_specifier IDENTIFIER
    ;

stmt_list:

    | stmt_list statement
    ;

statement:
    expr_stmt
    | printf_stmt
    | return_stmt
    | decl
    | if_stmt
    | while_stmt
    | do_while_stmt
    | for_stmt
    | switch_stmt
    | break_stmt
    | block
    | SEMICOLON
    | error SEMICOLON
    ;

if_stmt:
    IF OPAREN expr CPAREN OCURLY statement CCURLY
    | IF OPAREN expr CPAREN OCURLY statement CCURLY ELSE OCURLY statement CCURLY
    ;

while_stmt:
    WHILE OPAREN expr CPAREN OCURLY stamtement CCURLY
    ;

do_while_stmt:
    DO OCURLY statement CCURLY WHILE OPAREN expr CPAREN SEMICOLON
    ;

for_stmt:
    FOR OPAREN for_init_stmt optional_expr SEMICOLON optional_expr CPAREN OCURLY statement CCURLY
    ;

switch_stmt:
    SWITCH OPAREN expr CPAREN OCURLY case_list CCURLY
    ;

case_list:

    | case_list case_block
    ;

case_block:
    case_labels stmt_list
    ;

case_labels:
    case_label
    | case_labels case_label
    ;

case_label:
    CASE primary_expr COLON
    | DEFAULT COLON
    ;

break_stmt:
    BREAK SEMICOLON
    ;

for_init_stmt:
    expr_stmt
    | decl
    | SEMICOLON
    ;

optional_expr:

    | expr
    ;

printf_stmt:
    PRINTF OPAREN STRING_LITERAL CPAREN SEMICOLON
    | PRINTF OPAREN STRING_LITERAL COMMA IDENTIFIER CPAREN SEMICOLON
    | PRINTF OPAREN error CPAREN SEMICOLON
    ;

return_stmt:
    RETURN NUMBER SEMICOLON
    | RETURN IDENTIFIER SEMICOLON
    | RETURN error SEMICOLON
    ;

expr_stmt:
    expr
    | error SEMICOLON
    ;

expr:
    IDENTIFIER op_assign expr
    | cond_expr
    ;

cond_expr:
    add_expr
    | cond_expr LT add_expr
    | cond_expr GT add_expr
    | cond_expr LE add_expr
    | cond_expr GE add_expr
    | cond_expr EQ add_expr
    | cond_expr NEQ add_expr
    ;

add_expr:
    mul_expr
    | add_expr PLUS mult_expr
    | add_expr MINUS mult_expr
    ;

mult_expr:
    postfix_expr
    | mult_expr MULT postfix_expr
    | mult_expr DIVIDE postfix_expr
    ;

postfix_expr:
    primary_expr
    | IDENTIFIER UNARY
    ;

primary_expr:
    IDENTIFIER
    | NUMBER
    | FLOAT
    | STRING_LITERAL
    | CHAR_LITERAL
    | OPAREN expre CPAREN
    ;

%%

void yyerror (const char *s) {
    fprintf(stderr, "%s\n", s);
    return;
}

void add (char c) {
    q = search(yytext);
    if (q == 0) {
        if (c == 'H') {
            symbol_table[ind].id_name = strdup(yytext);
            symbol_table[ind].data_type = strdup("N/A");
            symbol_table[ind].line_no = count;
            symbol_table[ind].type = strdup("Header");
            ind++;
        }
        else if (c == 'K') {
            symbol_table[ind].id_name = strdup(yytext);
            symbol_table[ind].data_type = strdup("N/A");
            symbol_table[ind].line_no = count;
            symbol_table[ind].type = strdup("Keyword");
            ind++;
        }
        else if (c == 'V') {
            symbol_table[ind].id_name = strdup(yytext);
            symbol_table[ind].data_type = strdup(type);
            symbol_table[ind].line_no = count;
            symbol_table[ind].type = strdup("Variable");
            ind++;
        }
        else if (c == 'C') {
            symbol_table[ind].id_name = strdup(yytext);
            symbol_table[ind].data_type = strdup("CONST");
            symbol_table[ind].line_no = count;
            symbol_table[ind].type = strdup("Constant");
            ind++;
        }
    }
}

void insert_type () {
    strcpy(type, yytext);
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

void print_tree (struct node* nd) {
    printf("Inorder traversal of the parse tree: \n");
    print_inorder(nd);
    printf("\n\n");
    return;
}

void print_inorder (struct node* nd) {
    if (nd->left) print_inorder(nd->left);
    printf("%s, ", nd->token);
    if (nd->right) print_inorder(nd->right);
    return;
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
        printf("%15s|%15s|%15s|%15s\n", symbol_table[i].id_name, symbol_table[i].data_type, symbol_table[i].type, symbol_table[i].line_no);
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