%option nounput yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"

#define MAX_SYM 256
#define MAX_NAME 1024 
#define MAX_TABLE 256

// Token defination map
char tokens[100][100] = {
    "KEYWORD",
    "OPERATOR", 
    "IDENTIFIER", 
    "PREPROCESSOR", 
    "SEMICOLON", 
    "LITERAL", 
    "HEADERDIRECTORY", 
    "OPENCURLY",
    "CLOSEDCURLY", 
    "OPENPAREN", 
    "CLOSEDPAREN", 
    "OPENREC", 
    "CLOSEDREC", 
    "COMMA", 
    "SINGLELINECOMMENT", 
    "MULTILINECOMMENT",
    "UNMATCHEDCOMMENT", 
    "UNMATCHEDSTRING" ,
    "UNMATCHEDCOMMENTCLOSED",
    "FRACTION",
    "NEGFRACTION",
    "INTEGER",
    "NEGINTEGER",
    "FUNCTION",
    "ARRAY"
};

// Symbol Table struct definition
typedef struct symbol{
    char name[MAX_NAME]; // symbol name
    char token[MAX_SYM]; // symbol type
    char dimensions[256];
    int frequency;
    char returnType[20];
    char parametersList[512];
    int type;  // token number
} symbolEntry;

// Constant Table struct definition
typedef struct constant {
    char name[MAX_NAME];
    char token[MAX_SYM];
    int lineNum;
} constantEntry;

int symbolInd = 0;
int constantInd = 0;

struct symbol *symbol_table[MAX_TABLE];
struct constant *constant_table[MAX_TABLE];

char currParamsList[512];
char currArrayDims[256];

// Symbol Table Helper Function
void print_symbols(symbolEntry *symbol_table[]) 
{
    printf("\n<===== SYMBOL TABLE ======>\n");

    printf("%-30s %-20s %-12s %-6s %-12s %-20s\n", 
           "Name", "Token", "Dimensions", "Freq", "ReturnType", "Parameters");

    printf("-----------------------------------------------------------------------------------------------\n");

    for (int i = 0 ; i < symbolInd ; i++) {
        printf("%-30s %-20s %-12s %-6d %-12s %-20s\n", 
            symbol_table[i]->name, 
            symbol_table[i]->token, 
            symbol_table[i]->dimensions, 
            symbol_table[i]->frequency, 
            symbol_table[i]->returnType, 
            symbol_table[i]->parametersList
        );
    }
    printf("-----------------------------------------------------------------------------------------------\n\n");
}


// Constant Table Helper Function
void print_constant(constantEntry *constant_table[]) 
{
    printf("<====== CONSTANTS TABLE =====>\n");
    printf("%-30s %-20s %-12s\n", "Name", "Token", "LineNumber");
    printf("-------------------------------------------------------------\n");
    for (int i = 0 ; i < constantInd ; i++) {
        printf("%-30s %-20s %-12d\n", 
            constant_table[i]->name, 
            constant_table[i]->token, 
            constant_table[i]->lineNum);
    }
    printf("-------------------------------------------------------------\n\n");
} 

// Adds a symbol to the symbol table if not already present, else increments frequency
void add_symbol_to_table(char *lexeme, int tokenNumber) {
    int found = 0;
    for (int i = 0 ; i < symbolInd ; i++) {
        if (strcmp(symbol_table[i]->name, lexeme) == 0) {
            symbol_table[i]->frequency++;
            found = 1;
            break;
        }
    }
    if (found == 0) {
        symbolEntry *s = malloc(sizeof(symbolEntry));
        strcpy(s->name, lexeme);
        strcpy(s->token, tokens[tokenNumber - 1]);
        strcpy(s->dimensions, "-");
        s->frequency = 1;
        strcpy(s->returnType, "-");
        strcpy(s->parametersList, "-");
        s->type = tokenNumber;
        symbol_table[symbolInd++] = s;
    }
}

%}

WS      [ \t\r\f\v]+
DATATYPES (int|char|float|double|void|bool)
KEYWORDS (auto|break|const|goto|continue|switch|case|do|default|double|register|static|while|volatile|extern|short|unsigned|main|for|return|sizeof|struct|union|typedef|if|else)
IDENTIFIERS     [A-Za-z_][A-Za-z0-9_]*
DEC             (0|[1-9][0-9]*)
OCT             0[0-7]+
HEX             0[xX][0-9a-fA-F]+
EXP             ([eE][+-]?[0-9]+)
FLOAT1          ([0-9]+\\.[0-9]*{EXP}?|[0-9]+{EXP})
FLOAT2          (\\.[0-9]+{EXP}?)
CHAR            \\'([^\\\\'\\n]|\\\\.)\\'
STR             \\\"([^\\\\\\\"\\n]|\\\\.)*\\\"

%%

"auto"|"const"|"break"|"goto"|"continue"|"switch"|"case"|"do"|"default"|"double"              {return KEYWORD;}
"register"|"static"|"while"|"volatile"|"extern"|"short"|"unsigned"|"signed"|"int"|"char"      {return KEYWORD;}
"float"|"void"|"bool"|"main"|"for"|"return"|"sizeof"|"struct"|"union"|"typedef"|"if"|"else"   {return KEYWORD;}

\#"include"|\#"define" {return PREPROCESSOR;}
"<"[_a-zA-Z/]*".h>"    {return HEADERDIRECTORY;}

{IDENTIFIERS} {return IDENTIFIER;}
{DATATYPES}{WS}+{IDENTIFIERS}{WS}*"(" {
    char buf[512];
    int i = 0;
    int c;
    int paren_level = 1;   // Already saw the first "("
    
    // Clear parameter buffer
    currParamsList[0] = '\0';

    while ((c = input()) != 0 && paren_level > 0) {
        if (c == '(') {
            paren_level++;
        } else if (c == ')') {
            paren_level--;
            if (paren_level == 0) break; // stop at matching ')'
        }
        if (paren_level > 0 && i < 512 - 1) {
            buf[i++] = (char)c;
        }
    }
    buf[i] = '\0';
    strcpy(currParamsList, buf);

    //printf("Function params: %s\n", currParamsList);
    return FUNCTION;}

{DATATYPES}{WS}+{IDENTIFIERS}{WS}*"[" {
    char buf[256];
    int i = 0;
    int c;

    buf[i++] = '[';  // first [ already matched

    while ((c = input()) != 0) {
        buf[i++] = (char)c;

        if (c == ';' || c == '=') {
            buf[i - 1] = '\0';   // overwrite ; or = with string terminator
            break;
        }

        if (i >= 256 - 1) {
            buf[i] = '\0';
            break; // avoid overflow
        }
    }

    strcpy(currArrayDims, buf);
    //printf("Array dimensions: %s\n", currArrayDims);

    return ARRAY;
}

\"(\\.|[^"\\])*   {return UNMATCHEDSTRING;}  
"{"               {return OPENCURLY;}
"}"               {return CLOSEDCURLY;}
"("               {return OPENPAREN;}
")"               {return CLOSEDPAREN;}
"["               {return OPENREC;}
"]"               {return CLOSEDREC;}

"++"|"--"|"?"|":"|"="|"+"|"-"|"*"|"/"|">"|"<"|">="|"<="|"=="|"!="|"&&"|"||"|"!"|"->"|"&"|"%"|"+="|"/="|"-="|"*="  {return OPERATOR;}      
","           {return COMMA;}
";"           {return SEMICOLON;}

\"([^\\\"]|\\.)*\"      { return LITERAL; }   // string
\'([^\\\']|\\.)\'       { return LITERAL; }   // char
[0-9]+\.[0-9]+          { return FRACTION; }
-[0-9]+\.[0-9]+         { return NEGFRACTION; }
[0-9]+                  { return INTEGER; }
-[0-9]+                 { return NEGINTEGER; }

\/\/(.*)      {return SINGLELINECOMMENT;}
"/*"([^*]|\*+[^*/])*\*+"/" {return MULTILINECOMMENT;}
"/*"([^*]|\*+[^*/])*       {return UNMATCHEDCOMMENT;}
"*/"                       {return UNMATCHEDCOMMENTCLOSED;}

[ \n\t]     {;}
.           {printf("ERR_UNEXPECTED_CHARACTER: On Line Number %d found %s\n",yylineno, yytext);}

%%

int yywrap(void){
    return 1;
}  

int main()
{
    FILE *file;
    file =  fopen("test-cases/input1.c","r");
    if(file) 
        yyin = file;
    
    for (int i = 0; i < MAX_TABLE; i++)
    {
        symbol_table[i] = NULL;
        constant_table[i] = NULL;
    }

    char *lexeme;
    int tokenNumber = yylex(); // Start scanning

    while (tokenNumber) 
    {
        lexeme = strdup(yytext);

        symbolEntry *s = malloc(sizeof(symbolEntry));
        constantEntry *c = malloc(sizeof(constantEntry));

        // Literals
        if (tokenNumber == 6) {
            printf("On line number %d found literal %s\n", yylineno, lexeme);
        }

        // Constants
        if (tokenNumber == 20 || tokenNumber == 21 || tokenNumber == 22 || tokenNumber == 23) 
        { 
            // Constants table
            strcpy(c->name, lexeme);
            strcpy(c->token, tokens[tokenNumber - 1]);
            c->lineNum = yylineno;
            constant_table[constantInd++] = c;
        }

        // Error Handling
        else if (tokenNumber == 17) {
            printf("ERR_UNMATCHED_COMMENT: On Line Number %d found %s\n",yylineno, lexeme);
        }
        else if (tokenNumber == 18) {
            printf("ERR_INCOMPLETE_STRING: On Line Number %d found %s\n",yylineno, lexeme);
        }
        else if (tokenNumber == 19) {
            printf("ERR_UNMATCHED_COMMENT_DUE_TO_NESTS: On Line NUmber %d found %s\n",yylineno,lexeme);
        }

        // Functions
        else if (tokenNumber == 24) { 
            char returnType[50] = "";
            int rtInd = 0;
            char name[50] = "";
            int nameInd = 0;
            char parameters[200];
            int parInd = 0;
            int ind = 0;

            // Extract return type
            while (lexeme[ind] != '\0' && lexeme[ind] != ' ') 
            {
                returnType[rtInd++] = lexeme[ind];
                ind++;
            }
            returnType[rtInd] = '\0';
            add_symbol_to_table(returnType, 1); 

            // Skip spaces
            while (lexeme[ind] != '\0' && lexeme[ind] == ' ') ind++;

            // Extract function name
            while (lexeme[ind] != '\0' && lexeme[ind] != ' ' && lexeme[ind] != '(') {
                name[nameInd++] = lexeme[ind];
                ind++;
            }
            name[nameInd] = '\0';

            // Find opening parenthesis
            while (lexeme[ind] != '\0' && lexeme[ind] != '(') ind++;
            if (lexeme[ind] == '(') 
            {
                add_symbol_to_table("(", 10); 
                ind++;
            }

            // Save function entry in symbol table
            strcpy(s->name, name);
            strcpy(s->token, tokens[tokenNumber - 1]);
            strcpy(s->dimensions, "-");
            s->frequency = 1;
            strcpy(s->returnType, returnType);

            strcpy(s->parametersList, currParamsList);

            s->type = tokenNumber;
            symbol_table[symbolInd++] = s;
        }

        // Arrays
        else if (tokenNumber == 25) { 
            char type[32] = "";
            int typeInd = 0;
            char name[64] = "";
            int nameInd = 0;
            char dims[256] = "";
            int dimsInd = 0;
            int ind = 0;

            // Extract type (until first space)
            while (lexeme[ind] != '\0' && lexeme[ind] != ' ') {
                type[typeInd++] = lexeme[ind++];
            }
            type[typeInd] = '\0';

            // Skip spaces
            while (lexeme[ind] == ' ' && lexeme[ind] != '\0') ind++;

            // Extract name (until first '[')
            while (lexeme[ind] != '\0' && lexeme[ind] != '[') {
                name[nameInd++] = lexeme[ind++];
            }
            name[nameInd] = '\0';

            strcpy(s->name, name);
            strcpy(s->token, tokens[tokenNumber - 1]);
            strcpy(s->dimensions, currArrayDims);
            s->frequency = 1;
            strcpy(s->returnType, type);
            strcpy(s->parametersList, "-");
            s->type = tokenNumber;
            symbol_table[symbolInd++] = s;
        }

        // Normal Identifiers
        else {
            // Check if identifier already exist
            int found = 0;
            for (int i = 0 ; i < symbolInd ; i++) {
                if (strcmp(symbol_table[i]->name, lexeme) == 0) {
                    symbol_table[i]->frequency++;
                    found = 1;
                    break;
                }
            }

            // Add new identifier to symbol table
            if (found == 0) {
                strcpy(s->name, lexeme);
                strcpy(s->token, tokens[tokenNumber - 1]);
                strcpy(s->dimensions, "-");
                s->frequency = 1;
                strcpy(s->returnType, "-");
                strcpy(s->parametersList, "-");
                s->type = tokenNumber;
                symbol_table[symbolInd++] = s;
            }
        }

        printf("On Line Number %d found token: %s - %s\n",yylineno, tokens[tokenNumber - 1], lexeme);
       tokenNumber = yylex();
    }

    printf("\n\n");
    print_symbols(symbol_table);
    print_constant(constant_table);

    return 0;
}