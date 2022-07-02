%{
   
    #include "lex/token.h"
    
%}



%%
"("     return tok_open_p;//open parenthesis
")"     return tok_close_p;//close parenthesis

"hfloat"  return tok_keyword;//keywords
"hchar"  return tok_keyword;//keywords
"hstring"  return tok_keyword;//keywords
"hwhile"  return tok_keyword;//keywords
"hprint"  return tok_keyword;//keywords
"hint"  return tok_keyword;//keywords

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

[0-9]*          return tok_hint;//ints
[0-9]*"."[0-9]* return tok_hfloat;//floats
"'"."'"         return tok_hchar;//chars

\"(\\.|[^"\\])*\"    return tok_hstring;//https://stackoverflow.com/questions/2039795/regular-expression-for-a-string-literal-in-flex-lex



[a-zA-Z][a-zA-Z0-9_]* return tok_id;//identifier
[ \t\n] ;//ignore whitespace
. ;
%%

int yywrap(void) {
    return 1;
}

