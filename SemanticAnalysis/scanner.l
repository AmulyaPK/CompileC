%{
#include"y.tab.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

int count = 0;

%}

%option yylineno

ALPHA   [a-zA-Z]
DIGIT   [0-9]
UNARY   "++"|"--"
WS      [ \t\r\f\v]+

%%

"int"       { return INT; }
"float"     { return FLOAT; }
"char"      { return CHAR; }
"void"      { return VOID; }
"if"        { return IF; }
"else"      { return ELSE; }
"for"       { return FOR; }
"while"     { return WHILE; }
"do"        { return DO; }
"return"    { return RETURN; }
"switch"    { return SWITCH; }
"case"      { return CASE; }
"default"   { return DEFAULT; }
"break"     { return BREAK; }
"struct"    { return STRUCT; }
"union"     { return UNION; }
"printf"    { return PRINTF; }

"#include"[ \t]*<[a-zA-Z0-9._/]+>   { yylval.str = dup_text(yytext); return INCLUDE; }
"true"      { return TRUE; }
"false"     { return FALSE; }
{UNARY}     { return UNARY; }

[-]?[0|[1-9]{DIGIT}*]   { return NUMBER; }
[-]?[0|[1-9]{DIGIT}*]"."[0|{DIGIT}*[1-9]] { return FLOAT; }
{ALPHA}[a-zA-Z0-9_]*    { return IDENTIFIER; }    
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

{WS}    { }
\n      { count++; }
.       { fprintf(stderr, "ERR_UNEXPECTED CHARACTER: Line %d near '%s'\n", yylineno, yytext); }

%%

int yywrap (void) { return 1; }