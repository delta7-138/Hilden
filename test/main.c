#include <stdio.h>
#include "parser.tab.h"

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
extern FILE* yyout;

int main(int argc, char* argv[]) {
    if(argc < 2) {
        return 1;
    }
    yyin = fopen(argv[1], "r");
    yyparse();
    return 0;
}
