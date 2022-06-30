#include <stdio.h>
#include "token.h"

extern int yylex();
extern int yylineno;
extern char* yytext;

int main()
{
    int ntoken;

    ntoken = yylex();
    while (ntoken)
    {
        printf("%d\n", ntoken);
        ntoken = yylex();
    }
}