%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_SYM 1000
#define STRDUP(s) _strdup((s))

int lineNumber = 1;

int yyinput(void) 
{
    return getc(yyin);  
}

// Symbol Table linked list item struct
typedef struct SymbolEntry {
    char *name;
    char *type;
    char *dimensions; // for an array arr[10][20], the dimensions would be "[10][20]"
    int frequency;
    char *returnType;
    char *parametersList;
    struct SymbolEntry *next;
} SymbolEntry;

// Constant Table linked list item struct
typedef struct ConstantEntry{
    char *name;
    int lineNums[50];
    int lineInd;
    char *values;
    char *type;
    struct ConstantEntry *next;
} ConstantEntry;

static SymbolEntry *symbols = NULL;
static ConstantEntry *constants = NULL;

// Sybmol Table Helper Functions
static SymbolEntry* findSymbol (const char* name) 
{
    for (SymbolEntry* temp = symbols ; temp != NULL ; temp = temp->next) 
    {
        if (strcmp(temp->name, name) == 0) 
            return temp;
    }
    return NULL;
}

void addSymbol (const char* name, const char* type, const char* dimensions, const char* returnType, const char* parameters) 
{
    SymbolEntry* s = findSymbol(name);
    if (s) 
    {
        s->frequency++;
        if (dimensions && strcmp(s->dimensions, "-") != 0) 
        {
            free(s->dimensions);
            s->dimensions = STRDUP(dimensions);
        }
        if (parameters && strcmp(s->parametersList, "-") != 0) 
        {
            free(s->parametersList);
            s->parametersList = STRDUP(parameters);
        }
        if (type && strcmp(type, "function") == 0) 
        {
            free(s->type);
            free(s->returnType);
            s->type = STRDUP("function");
            s->returnType = STRDUP(returnType);
        }
        return;
    }

    s = (SymbolEntry*)calloc(1,sizeof(SymbolEntry));
    s->name = STRDUP(name);
    s->type = STRDUP(type);
    s->dimensions = STRDUP(dimensions ? dimensions : "-");
    s->frequency = 1;
    s->returnType = STRDUP(returnType ? returnType : "-");
    s->parametersList = STRDUP(parameters ? parameters : "-");
    s->next = NULL;

    if (!symbols) symbols = s;
    else 
    {
        SymbolEntry* t = symbols;
        while (t->next) 
        {
            t = t->next;
        }
        t->next = s;
    }
    return;
}

// Constant Table Helper Functions
static ConstantEntry* findConstant (const char* name) 
{
    for (ConstantEntry* temp = constants ; temp != NULL ; temp = temp->next) 
    {
        if (strcmp(temp->name, name) == 0) 
        {
            return temp;
        }
    }
    return NULL;
}

void addConstant(const char* name, int line, const char* value, const char* type) 
{
    ConstantEntry* s = findConstant(name);
    if (s) 
    {
        s->lineNums[s->lineInd] = line;
        s->lineInd++;
        return;
    }

    s = (ConstantEntry*)calloc(1, sizeof(ConstantEntry));
    s->name = STRDUP(name);
    s->lineInd = 0;  // initialize index for new constant
    s->lineNums[s->lineInd++] = line;
    s->values = STRDUP(value);
    s->type = STRDUP(type);
    s->next = NULL;

    if (!constants) 
    {
        constants = s;
    }
    else 
    {
        ConstantEntry* t = constants;
        while (t->next) t = t->next;
        t->next = s;
    }
}

// List of keywords 
static const char *keywords[] = {
    "if", "else", "for", "while", "do",
    "int", "float", "char", "double", "void",
    "return", "break", "continue", "switch",
    "case", "default", "struct", "typedef",
    "enum", "goto", "sizeof", "static",
    "const", "unsigned", "signed", "long",
    "short", "volatile", "register", "extern",
    "auto", NULL  /* NULL marks end of list */
};

// Check if a given string is a keyword 
static int isKeyword(const char *s) 
{
    for (int i = 0; keywords[i] != NULL; i++) 
    {
        if (strcmp(s, keywords[i]) == 0) 
        {
            return 1;  
        }
    }
    return 0;  
}

// To peek into the next character that is not a space.
// This is helpful to check if we have an upcoming parenthesis. This discovery can tell us that the identifier is that of a function.
static int peekNextNonSpace() 
{
    int c;
    do {
        c = fgetc(yyin);
        if (c == EOF) return EOF;
    } while (isspace(c));

    ungetc(c, yyin);
    return c;
}


// Capture the contents inside the first balanced parenthesis pair
// Handles nested parathesis and other edge cases
static char* captureParenthesis() {
    int c;
    int depth = 0;
    char buffer[2000];
    size_t k = 0;

    // Skip leading whitespace
    do {
        c = fgetc(yyin);
    } while (c != EOF && isspace(c));

    // If it’s not a parenthesis, push it back and return "-"
    if (c != '(') {
        if (c != EOF) ungetc(c, yyin);
        return STRDUP("-");
    }

    depth = 1;

    while ((c = fgetc(yyin)) != EOF) {
        if (c == '\n') yylineno++;
        if (k < sizeof(buffer) - 1) buffer[k++] = (char)c;

        if (c == '(') depth++;
        else if (c == ')') {
            depth--;
            if (depth == 0) break;
        }
        else if (c == '\"') { // string literal safety
            int d;
            buffer[k++] = c;
            while ((d = fgetc(yyin)) != EOF) {
                if (d == '\n') yylineno++;
                if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
                if (d == '\\') { // escape sequence
                    int e = fgetc(yyin);
                    if (e == EOF) break;
                    if (k < sizeof(buffer) - 1) buffer[k++] = (char)e;
                    continue;
                }
                if (d == '\"') break;
            }
        }
        else if (c == '\'') { // char literal safety
            int d = fgetc(yyin);
            if (d == EOF) break;
            if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
            if (d == '\\') {
                int e = fgetc(yyin);
                if (e == EOF) break;
                if (k < sizeof(buffer) - 1) buffer[k++] = (char)e;
            }
            d = fgetc(yyin);
            if (d == EOF) break;
            if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
        }

        if (k >= sizeof(buffer) - 2) break;
    }

    buffer[k] = '\0';

    // Remove trailing ) if present
    if (k > 0 && buffer[k - 1] == ')') buffer[k - 1] = '\0';

    // Remove leading '(' if present
    if (k > 0 && buffer[0] == '(') {
        size_t len = strlen(buffer);
        if (len > 0) memmove(buffer, buffer + 1, len);
    }

    return STRDUP(buffer);
}

// Helper function to extract function parameter lists
static char* getParameters() {
    int c;
    int depth = 0;
    char buffer[4096];
    size_t k = 0;

    // Skip leading whitespace
    do {
        c = fgetc(yyin);
    } while (c != EOF && isspace(c));

    // If not '(', return "-"
    if (c != '(') {
        if (c != EOF) ungetc(c, yyin);
        return STRDUP("-");
    }

    depth = 1;

    while ((c = fgetc(yyin)) != EOF) {
        if (c == '\n') yylineno++;
        if (k < sizeof(buffer) - 1) buffer[k++] = (char)c;

        if (c == '(') depth++;
        else if (c == ')') {
            depth--;
            if (depth == 0) break;
        }
        else if (c == '\"') { // string literal
            int d;
            buffer[k++] = c;
            while ((d = fgetc(yyin)) != EOF) {
                if (d == '\n') yylineno++;
                if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
                if (d == '\\') {
                    int e = fgetc(yyin);
                    if (e == EOF) break;
                    if (k < sizeof(buffer) - 1) buffer[k++] = (char)e;
                    continue;
                }
                if (d == '\"') break;
            }
        }
        else if (c == '\'') { // char literal
            int d = fgetc(yyin);
            if (d == EOF) break;
            if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
            if (d == '\\') {
                int e = fgetc(yyin);
                if (e == EOF) break;
                if (k < sizeof(buffer) - 1) buffer[k++] = (char)e;
            }
            d = fgetc(yyin);
            if (d == EOF) break;
            if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
        }

        if (k >= sizeof(buffer) - 2) break;
    }

    buffer[k] = '\0';

    // Remove trailing )
    if (k > 0 && buffer[k - 1] == ')') buffer[k - 1] = '\0';

    // Remove leading (
    if (k > 0 && buffer[0] == '(') {
        size_t len = strlen(buffer);
        if (len > 0) memmove(buffer, buffer + 1, len);
    }

    return STRDUP(buffer);
}

%}

%option noyywrap
%option yylineno

%x COMMENT

WHITESPACE      [ \t\r\f\v]+
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

^[ \t]*\#[^\n]* { printf("PREPROCESSOR: %s (line %d)\n", yytext, yylineno); } 
{WHITESPACE}    { }

"//".*    {
    printf("Single-line comment: %s (line %d)\n", yytext, yylineno);
}
"/*"    {
    char buffer[8192];   
    int i = 0, c, d;

    buffer[i++] = '/';
    buffer[i++] = '*';

    while ((c = yyinput()) != EOF) {
        buffer[i++] = (char)c;

        if (c == '\n') yylineno++;

        if (c == '*' && (d = yyinput()) == '/') {
            buffer[i++] = (char)d;
            buffer[i] = '\0';
            printf("Multi-line comment (line %d):\n%s\n", yylineno, buffer);
            break;
        } else if (d != EOF) {
            unput(d);
        }

        if (i >= sizeof(buffer) - 2) {
            fprintf(stderr, "Comment too long!\n");
            break;
        }
    }

    if (c == EOF) {
        buffer[i] = '\0';
        fprintf(stderr, "[line %d] ERROR: Unterminated comment\n%s\n", yylineno, buffer);
    }
}

{IDENTIFIERS}   {
                    const char* name = yytext;

                    if (isKeyword(name)) {
                        addSymbol(name, "keyword", "-", "-", "-");
                    }
                    else
                    {
                        // To determine variable, array or function name
                        int c = peekNextNonSpace();
                        
                        // Function call detected
                        if (c == '(') {
                            char *parameters = getParameters();
                            addSymbol(name, "function", "-", "-", parameters);
                            free(parameters);
                        }
                        // Array detected
                        else if (c == '[') {
                            int t = yyinput();
                            char dimensionsBuffer[256];
                            size_t k = 0;
                            dimensionsBuffer[k++] = '[';
                            int depth = 1;
                            int ch;
                            while ((ch == yyinput()) != EOF) {
                                if (ch == '\n') yylineno++;
                                dimensionsBuffer[k++] = (char)ch;
                                if (ch == '[') depth++;
                                else if (ch == ']') {
                                    depth--;
                                    if (depth == 0) break;
                                }
                                if (k >= sizeof(dimensionsBuffer) - 2) break;
                            }
                            dimensionsBuffer[k] = '\0';
                            addSymbol(name, "identifier", dimensionsBuffer, "-", "-");
                        }   
                        // Variable name
                        else {
                            addSymbol(name, "identifier", "-", "-", "-");
                        }
                    }
                }

"..."                                                               { }
"->"|"++"|"--"                                                      { }
"<="|">="|"=="|"!="                                                 { }
"&&"|"||"                                                           { }
"<<="|">>="|"<<"|">>"                                               { }
"+="|"-="|"*="|"/="|"%="|"&="|"|="|"^="                             { }
"~"|"!"|"%"|"^"|"&"|"|"|"+"|"-"|"*"|"/"|"="|"<"|">"|"?"|":"|"."     { }

"(" | ")" | "[" | "]" | "{" | "}" | "," | ";"  { print_token("PUNC", yytext); }

[0-9]*"."[0-9]+             {printf("%s is a fraction at line %d.\n", yytext, lineNumber);}
[-][0-9]*"."[0-9]+          {printf("%s is a negative fraction at line %d.\n", yytext, lineNumber);}
[0-9]+                      {printf("%s is an integer at line %d.\n", yytext, lineNumber);}
"-"[0-9]+                   {printf("%s is a negative integer at line %d.\n", yytext, lineNumber);}

.                           { }
[\t\n]+                     { }

%%

// USER SUBROUTINE SECTIONS

int main (int argc, char **argv) {
    // Open input file if provided
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("fopen");
            exit(1);
        }
    } else {
        yyin = stdin; // default to standard input
    }

    // Run the lexer (this fills symbol & constant tables)
    yylex();

    // Print symbol table
    printf("\n================= SYMBOL TABLE =================\n");
    printf("%-20s %-12s %-12s %-10s %-12s %-s\n",
        "Name","Type","Dimensions","Frequency","Return Type","Parameters Lists in Function call");
    for(SymbolEntry* p = symbols ; p != NULL ; p = p->next) {
        printf("%-20s %-12s %-12s %-10d %-12s %-s\n",
            p->name,
            p->type,
            p->dimensions,
            p->frequency,
            p->returnType ? p->returnType : "-",
            p->parametersList ? p->parametersList : "-"
        );
    }

    // Print constant table
    printf("\n================= CONSTANT TABLE ===============\n");
    printf("%-20s %-12s %-12s %-s\n",
        "Variable Name","Line Number","Value","Type");
    for (ConstantEntry* c = constants ; c != NULL ; c = c->next) {
        printf("%-20s %-12d %-12s %-s\n", c->name, c->lineNums[0], c->values, c->type);
    }

    return 0;
}
