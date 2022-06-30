%{
    // #include "lexer.h"
    #include "token.h"
    // #include <vector>
    // using namespace std;
    // std::vector<TokenType>tokenList; //Splitting the source code into words separated by spaces

    // enum Token{
    // tok_sep = -1, 
    // tok_open_p = -2, 
    // tok_close_p = -3, 
    // tok_keyword = -4, 
    // tok_operator = -5, 
    // tok_hfloat = -7, 
    // tok_hchar = -8, 
    // tok_hstring = -9, 
    // tok_id = -10, 
    // tok_undef = -6, 
    // tok_open_b = -11, 
    // tok_close_b = -12
// };
%}



%%
"("     return tok_open_p;//open parenthesis
")"     return tok_close_p;//close parenthesis

"hfloat"  return tok_keyword;//keywords
"hchar"  return tok_keyword;//keywords
"hstring"  return tok_keyword;//keywords
"hwhile"  return tok_keyword;//keywords
"hprint"  return tok_keyword;//keywords

"+" return tok_operator;//operator
"-" return tok_operator;//operator
"*" return tok_operator;//operator
"/" return tok_operator;//operator
"=" return tok_operator;//operator
"!" return tok_operator;//operator
"<" return tok_operator;//operator
">" return tok_operator;//operator

"{" return tok_open_b;//braces
"}" return tok_close_b;//braces

[0-9]*"."[0-9]* return tok_hfloat;
"'"."'" return tok_hchar;

[a-zA-Z][a-zA-Z0-9]* return tok_id;//identifier
[ \t\n] ;//ignore whitespace
. ;
%%

int yywrap(void) {
    return 1;
}

