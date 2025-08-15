// DECLARATION AND DEFINITION SECTION

%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SYM 1000

int lineNumber = 1;

typedef struct {
    char name [100];
    char type [20];
    int dimensions;
    int frequency;
    char returnType[20];
    char parametersList[20][20];
} SymbolEntry;
typedef struct {
    char name [100];
    int lineNums [100];
    char values [100];
    char type [20];
}

SymbolEntry symbolTable[MAX_SYM];
int symbolCount = 0;
char currentQualifier[20] = "";

void addSymbol (const char* name, const char* type) {
    // TODO: add code to update dimensions, return type, parameters list
    for (int i = 0 ; i < symbolCount ; i++) {
        if (strcmp(symbolTable[i].name == name) == 0 && strcmp(symbolTable[i].type == type) == 0) {
            symbolTable[i].frequency++;
            return;
        }
    }
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
     
    symbolTable[symbolCount].frequency = 1;
    symbolCount++;
}

void clearQualifier () {
    currentQualifier[0] = '\0';
}

void addConstant () {

}


%}

%%

// RULES SECTION

// TODO: add regular expressions for arrays, functions, etc.

// Preprocessor directive
#.*                                         {printf("%s is a pre-processor directive at line %d.\n", yytext, lineNumber);}

// Delimiters
[""|''|,|;|(|)|{|}|.|_]                     {printf("%s is a delimiter at line %d.\n", yytext, lineNumber);}
[[]]                                        {printf("%s is a delimiter at line %d.\n", yytext, lineNumber);}
"["|"]"                                     {printf("%s is a delimiter at line %d.\n", yytext, lineNumber);}

// Special characters
["#"|"@"|"$"|"^"|"%"|"&"]                   {printf("%s is a special character at line %d\n", yytext, lineNumber);}

// Operators
"="                                         {printf("%s is an assignment operator at line %d.\n", yytext, lineNumber);}
"*"|"+"|"-"|"/"                             {printf("%s is an arithmetic operator at line %d.\n", yytext, lineNumber);}
"and"|"or"|"not"|"nor"|"nand"|"xor"|"xnor"  {printf("%s is a logical operator at line %d.\n", yytext, lineNumber);}
"++"|"--"|"=="|"!="|"<"|">"|"<="|">="       {printf("%s is a relational operator at line %d.\n", yytext, lineNumber);}

// Keywords
("int")|("if")|("else")|("while")|("do")|("break")|("continue")|("double")|("float")|("return")|("EOF")                                 {printf("%s is a KEYWORD at line %d.\n", yytext, lineNumber);}
("return")|("char")|("case")|("sizeof")|("long")|("short")|("typedef")|("switch")|("unsigned")|("void")|("static")|("struct")|("goto")  {printf("%s is a KEYWORD at line %d.\n", yytext, lineNumber);}

// Functions and identifiers
[a-zA-Z_][a-zA-Z0-9_]*      {printf("%s is a function at line %d.\n", yytext, lineNumber);}
[a-zA-Z_][a-zA-Z0-9_]*      {printf("%s is a IDENTIFIER at line %d.\n", yytext, lineNumber);}

// Float and integers
[0-9]*"."[0-9]+             {printf("%s is a fraction at line %d.\n", yytext, lineNumber);}
[-][0-9]*"."[0-9]+          {printf("%s is a negative fraction at line %d.\n", yytext, lineNumber);}
[0-9]+                      {printf("%s is an integer at line %d.\n", yytext, lineNumber);}
"-"[0-9]+                   {printf("%s is a negative integer at line %d.\n", yytext, lineNumber);}

// Strings
["]([^"\\\n]|\\.|\\\n)*["]  {printf("%s is a string at line %d.\n", yytext, lineNumber);}

// Comments
"//".*" "*                  {printf("%s is a single line comment at line %d.\n", yytext, lineNumber);}
\/\*(.*\n)*.*\*\/           {printf("%s is a multi line comment at line %d.\n", yytext, lineNumber);}

// Empty spaces
.                           { }
[\t\n]+                     { }

%%

// USER SUBROUTINE SECTIONS

int main (int argc, char **argv) {

}

int yywrap (void) {
    return 1;
}