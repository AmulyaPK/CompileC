%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hello.tab.h"

int line_num = 1;

// ---------------- SYMBOL TABLE ----------------
typedef struct Symbol {
    char name[50];
    int line;
} Symbol;

Symbol symtab[100];
int sym_count = 0;

void insert_symbol(char *name, int line) {
    for (int i = 0; i < sym_count; i++) {
        if (strcmp(symtab[i].name, name) == 0) return; // already exists
    }
    strcpy(symtab[sym_count].name, name);
    symtab[sym_count].line = line;
    sym_count++;
}

void print_symbol_table() {
    printf("\n======= SYMBOL TABLE =======\n");
    printf("%-20s %-10s\n", "Identifier", "Line No.");
    printf("---------------------------------\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%-20s %-10d\n", symtab[i].name, symtab[i].line);
    }
    printf("=============================\n");
}
%}

DIGIT       [0-9]+
IDENTIFIER  [a-zA-Z_][a-zA-Z0-9_]*

%%

"#include"<[a-zA-Z0-9\.]+>   { return INCLUDE; }

"int"       { return INT; }
"float"     { return FLOAT; }
"char"      { return CHAR; }
"void"      { return VOID; }
"if"        { return IF; }
"else"      { return ELSE; }
"for"       { return FOR; }
"while"     { return WHILE; }
"return"    { return RETURN; }

"printf"    { return PRINTF; }

"=="        { return EQ; }
"!="        { return NEQ; }
"<"         { return LT; }
">"         { return GT; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIVIDE; }
"="         { return ASSIGN; }

"("         { return OPAREN; }
")"         { return CPAREN; }
"{"         { return OCURLY; }
"}"         { return CCURLY; }
";"         { return SEMICOLON; }
","         { return COMMA; }
"["         { return OBRACKET; }
"]"         { return CBRACKET; }
"\""        { return QUOTE; }

{DIGIT}     { yylval.str = strdup(yytext); return NUMBER; }
{IDENTIFIER} { 
                insert_symbol(yytext, line_num);
                yylval.str = strdup(yytext); 
                return IDENTIFIER; 
             }

[\n]        { line_num++; }
[ \t\r]+    { /* skip whitespace */ }
.           { printf("Unknown character: %s at line %d\n", yytext, line_num); }

%%

int yywrap(void) { return 1; }
