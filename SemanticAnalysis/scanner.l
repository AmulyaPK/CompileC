%{
#include"parse_tree.tab.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

int count = 0;

%}


ALPHA   [a-zA-Z]
DIGIT   [0-9]
UNARY   "++"|"--"
WS      [ \t\r\f\v]+

%%

"int"       { strcpy(yylval.nd_obj.name,(yytext)); return INT; }
"float"     { strcpy(yylval.nd_obj.name,(yytext)); return FLOAT; }
"char"      { strcpy(yylval.nd_obj.name,(yytext)); return CHAR; }
"void"      { strcpy(yylval.nd_obj.name,(yytext)); return VOID; }
"if"        { strcpy(yylval.nd_obj.name,(yytext)); return IF; }
"else"      { strcpy(yylval.nd_obj.name,(yytext)); return ELSE; }
"for"       { strcpy(yylval.nd_obj.name,(yytext)); return FOR; }
"while"     { strcpy(yylval.nd_obj.name,(yytext)); return WHILE; }
"do"        { strcpy(yylval.nd_obj.name,(yytext)); return DO; }
"return"    { strcpy(yylval.nd_obj.name,(yytext)); return RETURN; }
"switch"    { strcpy(yylval.nd_obj.name,(yytext)); return SWITCH; }
"case"      { strcpy(yylval.nd_obj.name,(yytext)); return CASE; }
"default"   { strcpy(yylval.nd_obj.name,(yytext)); return DEFAULT; }
"break"     { strcpy(yylval.nd_obj.name,(yytext)); return BREAK; }
"struct"    { strcpy(yylval.nd_obj.name,(yytext)); return STRUCT; }
"union"     { strcpy(yylval.nd_obj.name,(yytext)); return UNION; }
"printf"    { strcpy(yylval.nd_obj.name,(yytext)); return PRINTF; }

"#include"[ \t]*<[a-zA-Z0-9._/]+>   { strcpy(yylval.nd_obj.name,(yytext)); return INCLUDE; }
"true"      { strcpy(yylval.nd_obj.name,(yytext)); return TRUE; }
"false"     { strcpy(yylval.nd_obj.name,(yytext)); return FALSE; }
{UNARY}     { strcpy(yylval.nd_obj.name,(yytext)); return UNARY; }

[-]?[0|[1-9]{DIGIT}*]   { strcpy(yylval.nd_obj.name,(yytext)); return NUMBER; }
[-]?[0|[1-9]{DIGIT}*]"."[0|{DIGIT}*[1-9]] { strcpy(yylval.nd_obj.name,(yytext)); return FLOAT; }
{ALPHA}[a-zA-Z0-9_]*    { strcpy(yylval.nd_obj.name,(yytext)); return IDENTIFIER; }    
["].*["]                { strcpy(yylval.nd_obj.name,(yytext)); return STRING_LITERAL; }
\'.\' { strcpy(yylval.nd_obj.name,(yytext)); return CHAR_LITERAL; }

"=="        { strcpy(yylval.nd_obj.name,(yytext)); return EQ; }
"!="        { strcpy(yylval.nd_obj.name,(yytext)); return NEQ; }
"<="        { strcpy(yylval.nd_obj.name,(yytext)); return LE; }
">="        { strcpy(yylval.nd_obj.name,(yytext)); return GE; }
"<"         { strcpy(yylval.nd_obj.name,(yytext)); return LT; }
">"         { strcpy(yylval.nd_obj.name,(yytext)); return GT; }
"+"         { strcpy(yylval.nd_obj.name,(yytext)); return PLUS; }
"-"         { strcpy(yylval.nd_obj.name,(yytext)); return MINUS; }
"*"         { strcpy(yylval.nd_obj.name,(yytext)); return MULT; }
"/"         { strcpy(yylval.nd_obj.name,(yytext)); return DIVIDE; }
"="         { strcpy(yylval.nd_obj.name,(yytext)); return ASSIGN; }

"("         { strcpy(yylval.nd_obj.name,(yytext)); return OPAREN; }
")"         { strcpy(yylval.nd_obj.name,(yytext)); return CPAREN; }
"{"         { strcpy(yylval.nd_obj.name,(yytext)); return OCURLY; }
"}"         { strcpy(yylval.nd_obj.name,(yytext)); return CCURLY; }
";"         { strcpy(yylval.nd_obj.name,(yytext)); return SEMICOLON; }
","         { strcpy(yylval.nd_obj.name,(yytext)); return COMMA; }
"["         { strcpy(yylval.nd_obj.name,(yytext)); return OBRACKET; }
"]"         { strcpy(yylval.nd_obj.name,(yytext)); return CBRACKET; }
":"         { strcpy(yylval.nd_obj.name,(yytext)); return COLON; }

"//".*  {}
"/*"    {
    int depth = 1;
    int c, d;
    while ((c = input()) != EOF) {
        if (c == '\n') count++;
        if (c == '/' && (d = input()) == '*') { depth++; }
        else if (c == '*' && (d = input()) == '/') { depth--; if (depth == 0) break; }
        else { if (d != EOF) unput(d); }
    }
    if (depth > 0) {
        fprintf(stderr, "ERR_UNTERMINATED-COMMENT: Line %d\n", count);

        return UNMATCHED_COMMENT;
    }
}
"\#"[^ \t\n]*[^\n]  { strcpy(yylval.nd_obj.name,(yytext));  return PREPROCESSOR; }

{WS}    { }
\n      { count++; }
.       { fprintf(stderr, "ERR_UNEXPECTED CHARACTER: Line %d near '%s'\n", count, yytext); }

%%

int yywrap (void) { return 1; }