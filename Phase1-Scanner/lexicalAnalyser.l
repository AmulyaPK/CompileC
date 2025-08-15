// DECLARATION AND DEFINITION SECTION

%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_SYM 1000
#define STRDUP(s) _xstrdup((s))

int lineNumber = 1;

typedef struct {
    char *name;
    char *type;
    char *dimensions; // for an array arr[10][20], the dimensions would be "[10][20]"
    int frequency;
    char *returnType;
    char *parametersList;
    struct SymbolEntry *next;
} SymbolEntry;

typedef struct {
    char *name;
    int lineNums[50];
    int lineInd;
    char *values;
    char *type;
    struct ConstantEntry *next;
} ConstantEntry;

static SymbolEntry *symbols = NULL;
static ConstantEntry *constants = NULL;

static SymbolEntry* findSymbol (const char* name) {
    for (SymbolEntry temp = symbols ; temp != NULL ; temp = temp->next) {
        if (strcmp(temp->name, name) == 0) return temp;
    }
    return NULL;
}

void addSymbol (const char* name, const char* type, const char* dimensions, const char* returnType, const char* parameters) {
    SymbolEntry* s = findSymbol(name);
    if (s) {
        s->frequency++;
        if (dims && strcmp(s->dimensions, "-") != 0) {
            free(s->dimensions);
            s->dimensions = STRDUP(dimensions);
        }
        if (parameters && strcmp(s->parametersList, "-") != 0) {
            free(s->parametersList);
            s->parametersList = STRDUP(parameters);
        }
        if (type && strcmp(type, "function") == 0) {
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
    s->dimensions = STRDUPS(dimensions ? dimensions : "-");
    s->frequency = 1;
    s->returnType = STRDUP(returnType ? returnType : "-");
    s->parametersList = STRDUP(parameters ? parameters : "-");
    s->next = NULL;
    if (!symbols) symbols = s;
    else {
        SymbolEntry* t = symbols;
        while (t->next) {
            t = t->next;
        }
        t->next = s;
    }
    return;
}

static ConstantEntry* findConstant (const char* name) {
    for (ConstantEntry* temp = constants ; temp != NULL ; temp = tem->next) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
    }
    return NULL;
}

void addConstant (const char* name, int line, const char* value, const char* type) {
    ConstantEntry* s = findConstant(name);
    if (s) {
        s->lineNums[lineInd] = line;
        lineInd++;
        return;
    }
    s = (ConstantEntry*)calloc(1, sizeof(ConstantEntry));
    s->name = STRDUP(name);
    lineInd = 0;
    s->lineNums[lineInd++] = line;
    s->value = STRDUP(value);
    s->type = STRDUP(type);
    s->next = NULL;
    if (!constants) {
        constants = s;
    }
    else {
        ConstantEntry* t = constants;
        while (t->next) t = t->next;
        t->next = s;
    }
    return;
}

// To peek into the next character that is not a space.
// This is helpful to check if we have an upcoming parenthesis. This discovery can tell us that the identifier is that of a function.
static int peekNextNonSpace () {
    int c;
    do {
        c = yyinput();
        if (c == EOF) break;
        if (!isspace(c)) {
            unput(c);
            return c;
        }
        unput(c);
        c = yyinput();
    } while (isspace(c));
    if (c != EOF) unput(c);
    return c;
}

static char* captureParenthesis () {
    int c;
    int depth = 0;
    char buffer[2000];
    size_t k = 0;
    do {
        c = yyinput();
    } while (c != EOF && isspace(c));
    if (c != '(') {
        if (c != EOF) unput(c);
        return STRDUP("-");
    }
    depth = 1;
    while ((c = yyinput()) != EOF) {
        if (c =='\n') yylineno++;
        if (k < sizeof(buffer) - 1) buffer[k++] = (char)c;
        if (c == '(') depth++;
        else if (c == ')') {
            depth--;
            if (depth == 0) break;
        }
        else if (c == '\"') { /* string literal safety */
            int d;
            while ((d = yyinput()) != EOF) {
                if (d == '\n') yylineno++;
                if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
                if (d == '\\') {
                    int e = yyinput();
                    if (e == EOF) break;
                    if (k < sizeof(buffer) - 1) {
                        buffer[k++] = (char)e; 
                        continue; 
                    }
                    if (d == '\"') break;
                }
            }
        } else if (c == '\'') { /* char literal safety */
            int d = yyinput();
            if(d == EOF) break;
            if (k < sizeof(buffer) - 1) buf[k++] = (char)d;
            if(d=='\\') { 
                int e=yyinput(); 
                if (e == EOF) break; 
                if (k < sizeof(buffer) - 1) buffer[k++] = (char)e;
            }
            d = yyinput();
            if (d == EOF) break;
            if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
        }
        if (k >= sizeof(buffer) - 2) break;
    }
    buffer[k] = '\0';

    /* remove trailing ) if present */
    if (k > 0 && buffer[k - 1] == ')') buffer[k - 1] = '\0';

    /* strip the leading '(' if present */
    if (k > 0 && buffer[0] == '(') {
        /* shift left one */
        size_t len = strlen(buffer);
        if (len > 0) { memmove(buffer, buffer + 1, len); }
    }
    return STRDUP(buffer);
}

static char* getParameters () {
    int c;
    int depth = 0;
    char buffer[4096];
    size_t k=0;

    do { 
        c = yyinput(); 
    } while (c != EOF && isspace(c));
    if (c != '(') {
        if (c != EOF) unput(c);
        return STRDUP("-");
    }
    depth = 1;

    while ((c = yyinput()) != EOF) {
        if (c == '\n') yylineno++;
        if (k < sizeof(buffer) - 1) buffer[k++] = (char)c;
        if (c == '(') depth++;
        else if (c == ')') {
            depth--;
            if (depth == 0) break;
        } 
        else if (c == '\"') {
            int d;
            while ((d = yyinput()) != EOF) {
                if (d == '\n') yylineno++;
                if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
                if (d == '\\') { 
                    int e = yyinput(); 
                    if (e == EOF) break; 
                    if (k < sizeof(buffer) - 1) {
                        buffer[k++] = (char)e;
                        continue; 
                    }
                if (d == '\"') break;
                }
            } 
        }
        else if (c == '\'') {
            int d = yyinput(); 
            if (d == EOF) break; 
            if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
            if (d == '\\') { 
                int e = yyinput(); 
                if (e == EOF) break; 
                if (k < sizeof(buffer) - 1) buffer[k++] = (char)e; 
            }
            d = yyinput(); 
            if (d == EOF) break; 
            if (k < sizeof(buffer) - 1) buffer[k++] = (char)d;
        }
        if (k >= sizeof(buffer) - 2) break;
    }
    buffer[k] = '\0';

    if (k > 0 && buffer[k - 1] == ')') buffer[k - 1]= '\0';

    if (k > 0 && buffer[0] == '(') {
        size_t len = strlen(buffer);
        if (len > 0) { memmove(buf, buffer + 1, len); }
    }
    return STRDUP(buffer);
}

%}

%option noyywrap
%option yylineno
%option nounput
%option noinput

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

// RULES SECTION

// Preprocessor directives
^[ \t]*\#[^\n]* { } 
{WHITESPACE}    { }

// Comments
//.*            { }
"//".*          { }
"/*"            {
                    int depth=1, c,d;
                    while((c = yyinput()) != EOF) {
                        if (c == '\n') yylineno++;
                        if (c == '/' && (d = yyinput()) == '*') { depth++; }
                        else if (c == '*' && (d = yyinput()) == '/') { depth--; if(depth == 0) break; }
                        else { if (d != EOF) unput(d); }
                    }
                    if (depth > 0){
                        fprintf(stderr, "[line %d] ERROR: Unterminated comment\n", yylineno);
                    }
                }

// Keywords
"auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"inline"|"int"|"long"|"register"|"restrict"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"|"_Bool"|"_Complex"|"_Imaginary"|"_Alignas"|"_Alignof"|"_Atomic"|"_Noreturn"|"_Static_assert"|"_Thread_local"
                { }

// Identifiers
{IDENTIFIERS}   {
                    const char* name = yytext;
                    int c = peekNextNonSpace()
                    if (c == '(') {
                        char *parameters = getParameters();
                        addSymbol(name, "function", "-", "-", parameters);
                        free(parameters);
                    }
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
                    else {
                        addSymbol(name, "identifier", "-", "-", "-");
                    }
                }

// Operators & punctuation
"..."                                                               { }
"->"|"++"|"--"                                                      { }
"<="|">="|"=="|"!="                                                 { }
"&&"|"||"                                                           { }
"<<="|">>="|"<<"|">>"                                               { }
"+="|"-="|"*="|"/="|"%="|"&="|"|="|"^="                             { }
"~"|"!"|"%"|"^"|"&"|"|"|"+"|"-"|"*"|"/"|"="|"<"|">"|"?"|":"|"."     { }

"(" | ")" | "[" | "]" | "{" | "}" | "," | ";" 
                        { print_token("PUNC", yytext); }

// Float and integers
// TODO: fix this part for all kind of numerical values and create functionality to add to constants table
[0-9]*"."[0-9]+             {printf("%s is a fraction at line %d.\n", yytext, lineNumber);}
[-][0-9]*"."[0-9]+          {printf("%s is a negative fraction at line %d.\n", yytext, lineNumber);}
[0-9]+                      {printf("%s is an integer at line %d.\n", yytext, lineNumber);}
"-"[0-9]+                   {printf("%s is a negative integer at line %d.\n", yytext, lineNumber);}

// Empty spaces
.                           { }
[\t\n]+                     { }

%%

// USER SUBROUTINE SECTIONS

int main (int argc, char **argv) {
    printf("\n================= SYMBOL TABLE =================\n");
    printf("%-20s %-12s %-12s %-10s %-12s %-s\n",
        "Name","Type","Dimensions","Frequency","Return Type","Parameters Lists in Function call");
    for(SymbolEntry* p = symbols ; p != NULL ; p = p->next) {
        printf("%-20s %-12s %-12s %-10d %-12s %-s\n",
            p->name,
            p->type,
            p->dimensions,
            p->frequency,
            p->return_type ? p->return_type : "-",
            p->param_list ? p->param_list : "-"
        );
    }

    printf("\n================= CONSTANT TABLE ===============\n");
    printf("%-20s %-12s %-12s %-s\n",
        "Variable Name","Line Number","Value","Type");
    for (ConstantEntry* c = constants ; c != NULL ; c = c->next) {
        printf("%-20s %-12d %-12s %-s\n", c->varname, c->line, c->value, c->type);
    }
}

int yywrap (void) {
    return 1;
}