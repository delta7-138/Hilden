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
"hprintln" return tok_keyword; 
"hint"  return tok_keyword;//keywords
"hif" return tok_keyword; 
"helse" return tok_keyword; 
"ret" return tok_keyword; 

"+" return tok_operator;//operator
"-" return tok_operator;//operator
"*" return tok_operator;//operator
"/" return tok_operator;//operator
"=" return tok_operator;//operator
"!" return tok_operator;//operator
"<" return tok_operator;//operator
">" return tok_operator;//operator
"<=" return tok_operator; 
">=" return tok_operator; 
"!=" return tok_operator; 
"==" return tok_operator; 
"&" return tok_operator; 
"|" return tok_operator; 

";" return tok_sep; 
"," return tok_coma; 


"[" return tok_open_b; //block
"]" return tok_close_b; //block

"{" return tok_open_c;//braces
"}" return tok_close_c;//braces

[0-9]* return tok_hint;//ints
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

