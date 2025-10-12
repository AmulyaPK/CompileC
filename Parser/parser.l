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
"switch"    { return SWITCH; }
"case"      { return CASE; }
"default"   { return DEFAULT; }
"break"     { return BREAK; }

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
":"         { return COLON; }
"&"         { return AMPERSAND; }
\" {
    char buffer[4096];
    size_t k = 0;
    int c;

    while ((c = input()) != EOF) {
        if (c == '\n') {
            yylineno++;
            fprintf(stderr, "ERR_UNTERMINATED_STRING: Line %d\n", yylineno);
            return UNMATCHEDSTRING;
        }
        if (c == '"') break; // found closing quote

        if (c == '\\') {
            int d = input();
            if (d == EOF) break;
            if (d == '\n') yylineno++;
            if (k < sizeof(buffer) - 2) {
                buffer[k++] = c;
                buffer[k++] = d;
            }
        } else {
            if (k < sizeof(buffer) - 1) buffer[k++] = c;
        }
    }

    buffer[k] = '\0';

    // Compose the full string with surrounding quotes
    char *s = malloc(k + 3); // for quotes and null terminator
    if (!s) { perror("malloc"); exit(1); }
    s[0] = '"';
    strcpy(&s[1], buffer);
    strcat(s, "\"");

    yylval.str = s;
    return STRING_LITERAL;
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