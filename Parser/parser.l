%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

static char *dup_text(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (!p) { 
        perror("malloc"); 
        exit(1); 
    }
    memcpy(p, s, n);
    return p;
}

extern YYSTYPE yylval;

int yylineno = 1;

%}

DIGIT       [0-9]+
IDENTIFIER  [a-zA-Z_][a-zA-Z0-9_]*
WT          [ \t\r\f\v]+
NEWLINE     \n
STRING_BODY      ([^\\\"\n]|\\(.))
CHAR_BODY        ([^\\'\n]|\\(.))

%%

"#include"[ \t]*<[a-zA-Z0-9._/]+>   { yylval.str = dup_text(yytext); return INCLUDE; }

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
"<="        { return LE; }
">="        { return GE; }
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
"\""        { 
    {
        char buffer[4096];
        size_t k = 0;
        int c;
        while ((c = input()) != EOF) {
            if (c == '\n') { yylineno++; }
            if (k < sizeof(buffer) - 1) buffer[k++] = (char)c;
            if (c == '\\') {
                int d = input();
                if (d == EOF) break;
                if (d == '\n') yylineno++;
                if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
                continue;
            }
            if (c == '"') break;
        }
        buffer[k] = '\0';
        size_t total_length = 2 + strlen(buffer) + 1;
        char *s = malloc(total_length);
        if (!s) { perror("malloc"); exit(1); }
        s[0] = '"';
        s[1] = '\0';
        strncat(s, buffer, total_length - 2);
        if (strlen(buffer) == 0 || buffer[strlen(buffer) - 1] != '"') {
            fprintf(stderr, "Err_INCOMPLETE_STRING: Line %d near %s\n", yylineno, s);
            free(s);
            return UNMATCHEDSTRING;
        }
        else {
            yylval.str = s;
            return STRING_LITERAL;
        }
    }
    // return QUOTE; 
}
\'{CHAR_BODY}\' { yylval.str = dup_text(yytext); return CHAR_LITERAL; }
{DIGIT}"."{DIGIT}([eE][+-]?{DIGIT})? { yylval.str = dup_text(yytext); return NUMBER; }
{DIGIT}([eE][+-]?{DIGIT})?    { yylval.str = dup_text(yytext); return NUMBER; }
{IDENTIFIER} {
                yylval.str = dup_text(yytext); 
                return IDENTIFIER; 
             }

"//".*  {}
"/*"    {
    int depth = 1;
    int c, d;
    while ((c = input()) != EOF) {
        if (c == '\n') yylineno++;
        if (c == '/' && (d = input()) == '*') { depth++; }
        else if (c == '*' && (d = input()) == '/') { depth--; if (depth == 0) break; }
        else { if (d != EOF) unput(d); }
    }
    if (depth > 0) {
        fprintf(stderr, "ERR_UNTERMINATED-COMMENT: Line %d\n", yylineno);
        return UNMATCHEDCOMMENT;
    }
}

"\#"[^ \t\n]*[^\n]  { yylval.str = dup_text(yytext); return PREPROCESSOR; }
\n        { yylineno++; }
{WT}    { /* skip whitespace */ }
.           { fprintf(stderr, "ERR_UNEXPECTED_CHARACTER: Line %d near '%s'\n", yylineno, yytext); }

%%

int yywrap(void) { return 1; }