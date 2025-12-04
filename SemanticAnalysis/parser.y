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

int counter = 0;
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
%token<nd_obj> INT FLOAT CHAR IF ELSE FOR WHILE RETURN SWITCH CASE DEFAULT BREAK STRUCT UNION PRINTF INCLUDE TRUE FALSE UNARY NUMBER FLOAT IDENTIFIER STRING_LITERAL CHAR_LITERAL EQ NEQ LE GE LT GT PLUS MINUS MULT DIVIDE ASSIGN OPAREN CPAREN OCURLY CCURLY SEMICOLON COMMA OBRACKET CBRACKET COLON UNMATCHED_COMMENT PREPROCESSOR
