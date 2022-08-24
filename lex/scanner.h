#include "lexer.h"
#include <vector>
#include "token.h"
using namespace std;

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE* yyin;


vector<TokenType> gettokenlist(char * file);