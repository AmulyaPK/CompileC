%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex();
extern int line_num;
void print_symbol_table();
%}

%union {
    int num;
    char *str;
}

%token <str> IDENTIFIER NUMBER
%token INT FLOAT CHAR VOID
%token IF ELSE FOR WHILE RETURN
%token PRINTF
%token EQ NEQ LT GT
%token PLUS MINUS MULT DIVIDE
%token ASSIGN
%token OPAREN CPAREN OCURLY CCURLY SEMICOLON COMMA
%token OBRACKET CBRACKET
%token INCLUDE QUOTE

%start program

%%

program:
    includes main_func
    ;

includes:
    includes INCLUDE
    | INCLUDE
    | /* empty */
    ;

main_func:
    INT IDENTIFIER OPAREN CPAREN block
    ;

block:
    OCURLY decl_list stmt_list CCURLY
    ;

decl_list:
    decl_list decl
    | /* empty */
    ;

decl:
    type IDENTIFIER SEMICOLON
    | type IDENTIFIER OBRACKET NUMBER CBRACKET SEMICOLON
    ;

type:
    INT
    | FLOAT
    | CHAR
    | VOID
    ;

stmt_list:
    stmt_list stmt
    | /* empty */
    ;

stmt:
    expr_stmt
    | printf_stmt
    | return_stmt
    ;

expr_stmt:
    expr SEMICOLON
    ;

printf_stmt:
    PRINTF OPAREN QUOTE IDENTIFIER QUOTE CPAREN SEMICOLON
    | PRINTF OPAREN QUOTE IDENTIFIER QUOTE COMMA IDENTIFIER CPAREN SEMICOLON
    ;

return_stmt:
    RETURN NUMBER SEMICOLON
    | RETURN IDENTIFIER SEMICOLON
    ;

expr:
    IDENTIFIER ASSIGN expr
    | NUMBER
    | IDENTIFIER
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", line_num, s);
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
    }

    fclose(file);
    return 0;
}
